/*
 * version = d : 2.0
 * file = s : "main.cpp"
 * project = s : "landb"
 *
 * (credits:
 *          message = s : "Created by René Descartes Domingos Muala on 10/10/20."
 *          Copyright = s : "© 2021 landia (René Muala). All rights reserved."
 *          Contact = s : "renemuala@icloud.com"
 * )
 */

#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace lan
{
    /* lan::safe_file */
    class safe_file {
        FILE * file;
        std::string filename;

    public:
        
        safe_file();
        
        bool open(std::string);
        bool check();
        bool push(std::string);
        std::string pull();
        size_t length();
        bool close();
        bool close_fd();

        ~safe_file();
    };
    
    const std::string safe_file_version = "1.0 (stable)";
    
    /* lan::db */
    
    const std::string db_version = "2.1 (stable)";
    
    namespace errors {
        //! @brief catch parameter to handle errors with bit names
        typedef std::invalid_argument bit_name_error;
        typedef std::out_of_range anchor_name_error;
        typedef std::logic_error pull_error;
        
        namespace _private {
            enum error_type {_bit_name_error, _anchor_name_error};
        }
    }
    
    //! @brief database bit type
    enum db_bit_type {Bool , Int, Long, LongLong, Float, Double, Char, String, Unsafe, Array, Container};
    
    /*
     *     namespace _private {
     *        char db_bit_table [11] = {  'b' ,   'i' ,
     *                                    'l' ,   'x' ,
     *                                    'f' ,   'd' ,
     *                                    'c' ,   's' ,
     *                                    'u' ,   'a' ,
     *                                            '#' };
} */
    
    //! @brief database bit: used to criate linked lists that store variables, arrays and containers dynamically
    struct db_bit {
        std::string     key;
        db_bit_type     type;
        void *          data;
        struct db_bit * pre, * nex, * lin, * con;
        db_bit(){
            key.clear();
            data = nullptr;
            pre  = nullptr;
            nex  = nullptr;
            lin  = nullptr;
            con  = nullptr; 
        } ~ db_bit (){
            if(data or not key.empty()) { ::free(data) ; db_bit();}
        }
    };
    
    typedef db_bit db_bits;
    typedef db_bit anchor_t;
    
    class db {
        
        lan::db_bits * data;
        lan::db_bit  * first, * last;
        lan::anchor_t * anchor;
        lan::safe_file file;
        
    public:
        
        db();
        
        /* -- */
        
        void erase_bits(db_bits *);
        
        void erase_bit(db_bit *);
        
        void reset_data();
        
        /*! @brief Erases the current loaded bits and frees memory*/
        void erase();
        
        bool empty();
        
        /*! @brief Prints the main bits list in format: ["<bit name>"] @<type id> <bit data address>*/
        void print(size_t = 0 , lan::db_bit * = nullptr);
        
        /* file */
        
        bool connect(std::string);
        
        bool is_connected();
        
        bool disconnect();
        
        size_t  str_get_str_end(std::string, size_t = 0);
        
        ssize_t str_find_first_of(std::string const , std::string const, size_t = 0);
        
        ssize_t str_find_first_not_of(std::string const, std::string const, size_t = 0);
        
        bool str_compare(const char , const std::string );
        
        std::string pop_next(std::string &);
        
        lan::db_bit * get_container_data(std::string &);
        
        lan::db_bit  * read_container_bit(std::string &, bool = false);
        
        lan::db_bit * get_array_data(std::string &);
        
        lan::db_bit  * read_array_bit(std::string const, std::string &);
        
        lan::db_bit_type convert_to_bit_type(char);
        
        std::string prepare_string_to_read(std::string);
        
        void * get_var_data(db_bit_type, std::string);
        
        lan::db_bit  * read_var_bit(std::string const, db_bit_type const, std::string &);
        
        lan::db_bit  * read_value_bit(std::string &, bool = false);
        
        lan::db_bit  * read_bit(std::string &);
        
        lan::db_bits * read_all_bits(std::string);
        
        bool pull();
        
        std::string write_container_bit(db_bit *);
        
        std::string write_array_bit(db_bit *);
        
        std::string prepare_char_to_write(char);
        
        std::string prepare_string_to_write(std::string);
        
        std::string write_var_bit(db_bit *, bool = false);
        
        std::string write_bit(db_bit *, bool = false);
        
        std::string write_all_bits(db_bits *);
        
        bool push();
        
        /* error handling */
        
        std::string error_string(errors::_private::error_type, std::string const);
        
        /* db general */
        
        bool update_last(){return (last = (first and last) ? last : get_last_bit(first));}
        
        template<typename any>
        bool set_bit(db_bit * context, db_bit * var,std::string const name, db_bit_type const type, any const value){
            if(var->data)
                var->~db_bit();
            var->key  = name;
            var->data = new any (value);
            var->type = type;
            var->con  = context; 
            return  (var->data);
        }
        
        bool set_bit(db_bit * context, db_bit * var,std::string const name, db_bit_type const type){
            if(var->data)
                var->~db_bit();
            var->key  = name;
            var->type = type;
            var->con = context;
            return  (var);
        }
        
        template<typename any>
        bool init(std::string const name, any const value, db_bit_type const type){
            data = new db_bit;
            return (first = last = data) and ((type < lan::Array) ? set_bit(nullptr, data, name, type, value) : set_bit(nullptr ,data, name, type));
        }
        
        template<typename any>
        bool init(lan::db_bit * target, std::string const name, any const value, db_bit_type const type){
            data = target;
            data->lin = new db_bit;
            return ((type < lan::Array) ? set_bit(data, data->lin, name, type, value) : set_bit(data, data->lin, name, type));
        }
        
        template<typename any>
        bool append(std::string const name, any const value, db_bit_type const type){
            last = get_last_bit(first);
            last->nex = new db_bit; last->nex->pre = last;
            return (last=last->nex) and ((type < lan::Array) ? set_bit(nullptr, last, name, type, value) : set_bit(nullptr, last, name, type));
        }
        
        template<typename any>
        bool append(lan::db_bit * target, std::string const name, any const value, db_bit_type const type){
            data = target;
            if((data = data->lin)){
                data = get_last_bit(data);
                data->nex = new db_bit; data->nex->pre = data; data = data->nex;
                return ((type < lan::Array) ? set_bit(target, data, name, type, value) : set_bit(target, data, name, type));
            } return false;
        }
        
        template<typename any>
        bool init_iter(lan::db_bit * target, std::string const name, any const value, db_bit_type const type){
            data = target;
            data->lin = new db_bit; data = data->lin;
            return ((type < lan::Array) ? set_bit(target, data, name, type, value) : set_bit(target, data, name, type));
        }
        
        template<typename any>
        bool append_iter(lan::db_bit * target, std::string const name, any const value, db_bit_type const type){
            data = target;
            if((data = data->lin)){
                data = get_last_bit(data);
                data->nex = new db_bit; data->nex->pre = data; data = data->nex;
                return ((type < lan::Array) ? set_bit(target, data, name, type, value) : set_bit(target, data, name, type));
            } return false;
        }
        
        template<typename any>
        bool iterate(std::string const target, any const value, db_bit_type const type){
            if((data = find_rec(target, lan::Container, lan::Array, first))) {
                return (data->lin) ? append_iter(data, "", value, type) :
                init_iter(data, "", value, type);
            }
            return false;
        }
        
        bool declare(std::string const name, db_bit_type const type);
        
        bool declare(std::string const target, std::string const name, db_bit_type const type);
        
        std::string find__pop_address(std::string &);
        
        lan::db_bit * find(std::string const, lan::db_bit *);
        
        /*! @brief Find db_bit recursivelly inside of container(s)*/
        
        lan::db_bit * find_rec(std::string, lan::db_bit_type const , lan::db_bit *);
        
        lan::db_bit * find_rec(std::string, lan::db_bit_type const , lan::db_bit_type const, lan::db_bit *);
        
        lan::db_bit * find_var(std::string const, lan::db_bit *);
        
        lan::db_bit * find_non_var(std::string const, lan::db_bit *);
        
        lan::db_bit * find_any(std::string const, lan::db_bit_type const, lan::db_bit *);
        
        /* get */
        
        template<typename any>
        any get(std::string const name, const lan::db_bit_type type){
            if((data = find_any(name, type, first)) and data->data and type < lan::Array){
                any * data_p = (any*)data->data;
                return ((any&)*data_p);
            } throw std::invalid_argument(error_string(errors::_private::_bit_name_error, name));
        }
        
        template<typename any>
        any get(std::string const name, size_t index, const lan::db_bit_type type){
            if((data = find_any(name, lan::Array, first))){
                data=data->lin;
                for(register_t it = 0 ; (it < index) and data ; it++, data=data->nex);
                if(data and data->type == type){
                    any * data_p = (any*)data->data;
                    return ((any&)*data_p);
                } throw std::invalid_argument(error_string(errors::_private::_bit_name_error, name+"["+std::to_string(index)+"]"));
            } throw std::invalid_argument(error_string(errors::_private::_bit_name_error, name));
        }
        
        template<typename any>
        any get(std::string target, std::string const name, const lan::db_bit_type type){
            if ((data = find_rec(target, lan::Container, first))) {
                if((data = find_any(name, type, data->lin)) and data->data){
                    any * data_p = (any*)data->data;
                    return ((any&)*data_p);
                }
            } throw std::invalid_argument(error_string(errors::_private::_bit_name_error, name));
        }
        
        template<typename any>
        any get(db_bit * bit){
            if(bit){
                any * data_p = (any*)bit->data;
                return ((any&)*data_p);
            } return 0;
        }
        
        lan::db_bit * get_array_bit(lan::db_bits *, size_t);
        
        lan::db_bit * get_last_bit(lan::db_bits *);
        
        lan::db_bit_type get_var_type(std::string const);
        
        void * operator [](std::string);
        
        /* set */
        
        template<typename any>
        bool set(std::string const name, any const value, lan::db_bit_type type, bool overwrite = false){
            if(type >= lan::Array) return false;
            if((data = find_any(name, type, first))){
                if(not overwrite) return false;
                return set_bit(nullptr,data, name, type, value);
            } return (not first) ? init(name, value, type) : append(name, value, type);
        }
        
        template<typename any>
        bool set(std::string target, size_t index,  any const value, lan::db_bit_type type){
            if((data = find_rec(target, lan::Container, lan::Array, first))){
                data=data->lin;
                for(register_t it = 0 ; (it < index) and data ; it++, data=data->nex);
                if(data){
                    if(data->data)
                        data->~db_bit();
                    data->type = type;
                    if(type < lan::Array)
                        data->data = new any (value);
                } 
            } return false;
        }
        
        template<typename any>
        bool set(std::string const target, std::string const name, any const value, lan::db_bit_type type, bool overwrite = false){
            if(type >= lan::Array) return false;
            if((data = find_rec(target, lan::Container, first))){
                lan::db_bit * buffer = data;
                if((data = find_any(name, type, data))) {
                    if(not overwrite) return false;
                    return set_bit(buffer, data, name, type, value);
                } return (buffer->lin) ? append(buffer, name, value, type) : init(buffer, name, value, type);
            } return false;
        }
        
        /*! @brief Sets the anchor, aka "@", to a specific bit, used to handle operations in arrays */
        lan::anchor_t * set_anchor(std::string const target, size_t index){
            if ((data = find_rec(target, lan::Container, lan::Array, first)) and
                (anchor = get_array_bit(data, index))) return anchor;
            else throw std::out_of_range(error_string(errors::_private::_anchor_name_error, target+"["+std::to_string(index)+"]"));
        }
        
        lan::anchor_t * set_anchor(std::string const target){
            if ((data = find_rec(target, lan::Container, first))) return anchor;
            else throw std::out_of_range(error_string(errors::_private::_anchor_name_error, target));
        }
        
        lan::anchor_t * set_anchor(lan::anchor_t * anchor){
            if ((this->anchor = anchor)) return anchor;
            else throw std::out_of_range(error_string(errors::_private::_anchor_name_error, "nullptr"));
        }
        
        /* remove */
        
        bool remove(std::string const target, db_bit_type const type);
        
        bool remove(std::string const address, std::string const target, db_bit_type const type);
        
        bool remove(std::string const target, size_t index);
        
        /* -- */
        
        ~db();
    };
} /* namespace lan */
