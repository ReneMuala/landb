/*!
 * project = s : "landb"
 *
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
        
        /* opens a file */
        bool open(std::string);
        /* checks if the file is opened */
        bool check();
        /* pushes a string into the current file */
        bool push(std::string);
        /* pulls a string from the current file */
        std::string pull();
        /* gets the length of the current file */
        size_t length();
        /* closes the current file */
        bool close();
        /* closes the current file descriptor*/
        bool close_fd();

        ~safe_file();
    };
    
    const std::string safe_file_version = "1.0 (stable)";
    
    /* lan::db */
    
    const std::string db_version = "2.5 (public)";
    
    namespace errors {
        //! @brief catch parameter to handle errors with bit names
        typedef std::invalid_argument bit_name_error;
        typedef std::out_of_range anchor_name_error;
        typedef std::out_of_range empty_anchor_error;
        typedef std::logic_error pull_error;
        typedef std::runtime_error overriding_bit_error;
        
        namespace _private {
            enum error_type {_bit_name_error, _anchor_name_error, _empty_anchor_error, _overriding_bit_error};
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
            if(lin) {delete lin; lin = nullptr;}
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
        
        /* erases all the bits in the context */
        void erase_bits(db_bits *);
        
        /* erases a bit */
        void erase_bit(db_bit *);
        
        /* resets the pointers and variables of the class*/
        void reset_data();
        
        /* Erases the current loaded bits and frees memory*/
        void erase();
        
        /* the database is empty */
        bool empty();
        
        /*! prints the bits in the current context (default: main from *first)
         *  @param tabs used by the system.
         *  @param  bit the bit to start printing from.
         */
        void print(size_t tabs = 0 , lan::db_bit * bit = nullptr);
        
        /* file */
        
        /* connects the database to a file, to pull and push from */
        bool connect(std::string);
        
        /* database is connected to a file */
        bool is_connected();
        
        /* disconnects the database from the current file */
        bool disconnect();
        
        /*! @brief pull dependece */
        size_t  str_get_str_end(std::string, size_t = 0);
        
        /*! @brief pull dependece */
        ssize_t str_find_first_of(std::string const , std::string const, size_t = 0);
        
        /*! @brief pull dependece */
        ssize_t str_find_first_not_of(std::string const, std::string const, size_t = 0);
        
        /*! @brief pull dependece */
        bool str_compare(const char , const std::string );
        
        /*! @brief pull dependece */
        std::string pop_next(std::string &);
        
        /*! @brief pull dependece */
        lan::db_bit * get_container_data(std::string &);
        
        /*! @brief pull dependece */
        lan::db_bit  * read_container_bit(std::string &, bool = false);
        
        /*! @brief pull dependece */
        lan::db_bit * get_array_data(std::string &);
        
        /*! @brief pull dependece */
        lan::db_bit  * read_array_bit(std::string const, std::string &);
        
        /*! @brief pull dependece */
        lan::db_bit_type convert_to_bit_type(char);
        
        /*! @brief pull dependece */
        std::string prepare_string_to_read(std::string);
        
        /*! @brief pull dependece */
        void * get_var_data(db_bit_type, std::string);
        
        /*! @brief pull dependece */
        lan::db_bit  * read_var_bit(std::string const, db_bit_type const, std::string &);
        
        /*! @brief pull dependece */
        lan::db_bit  * read_value_bit(std::string &, bool = false);
        
        /*! @brief pull dependece */
        lan::db_bit  * read_bit(std::string &);
        
        /*! @brief pull dependece */
        lan::db_bits * read_all_bits(std::string);
        
        /*! @brief pulls data from the connected file. Note: This operaion will erase all bits */
        bool pull();
        
        /*! @brief push dependece */
        std::string write_container_bit(db_bit *);
        
        /*! @brief push dependece */
        std::string write_array_bit(db_bit *);
        
        /*! @brief push dependece */
        std::string prepare_char_to_write(char);
        
        /*! @brief push dependece */
        std::string prepare_string_to_write(std::string);
        
        /*! @brief push dependece */
        std::string write_var_bit(db_bit *, bool = false);
        
        /*! @brief push dependece */
        std::string write_bit(db_bit *, bool = false);
        
        /*! @brief push dependece */
        std::string write_all_bits(db_bits *);
        
        /*! @brief pushes data to the current file, writing all data in human-readable form.*/
        bool push();
        
        /* error handling */
        
        /*! @brief general dependece */
        std::string error_string(errors::_private::error_type, std::string const);
        
        /* db general */
        
        /*! @brief updates the *last pointer */
        bool update_last(){return (last = (first and last) ? last : get_last_bit(first));}
        
        /*! @brief sets a variable bit.
         @param context the context of the variable.
         @param var     the bit that will be set.
         @param name    the name of the bit.
         @param type    the lan::type of the bit.
         @param value   the literal value of the bit.
         */
        template<typename any>
        bool set_bit(db_bit * context, db_bit * var,std::string const name, db_bit_type const type, any const value){
            set_bit(context, var, name, type);
            var->data = new any (value);
            return (var->data);
        }
        
        /*! @brief sets a bit.
         @param context the context of the variable.
         @param var     the bit that will be set.
         @param name    the name of the bit.
         @param type    the lan::type of the bit.
         */
        bool set_bit(db_bit * context, db_bit * var,std::string const name, db_bit_type const type){
            var->~db_bit();
            var->key  = name;
            var->type = type;
            var->con = context;
            return  (var);
        }
        
        /*! @brief inits the *first bit, used by the system when the first bit is declared.
         @param name    the name of the bit.
         @param value   the literal value of the bit.
         @param type    the lan::type of the bit.
         */
        template<typename any>
        bool init(std::string const name, any const value, db_bit_type const type){
            data = new db_bit;
            return (first = last = data) and ((type < lan::Array) ? set_bit(nullptr, data, name, type, value) : set_bit(nullptr ,data, name, type));
        }
        
        /*! @brief inits the *first bit of a context, used by the system when the first bit of a certain context is declared.
         @param context the context.
         @param name    the name of the bit.
         @param value   the literal value of the bit.
         @param type    the lan::type of the bit.
         */
        template<typename any>
        bool init(lan::db_bit * context, std::string const name, any const value, db_bit_type const type){
            data = context;
            data->lin = new db_bit;
            return ((type < lan::Array) ? set_bit(data, data->lin, name, type, value) : set_bit(data, data->lin, name, type));
        }
        
        /*! @brief appends a bit in the main context, used by the system to append a bit in the main context.
         @param name    the name of the bit.
         @param value   the literal value of the bit.
         @param type    the lan::type of the bit.
         */
        template<typename any>
        bool append(std::string const name, any const value, db_bit_type const type){
            last = get_last_bit(first);
            last->nex = new db_bit; last->nex->pre = last;
            return (last=last->nex) and ((type < lan::Array) ? set_bit(nullptr, last, name, type, value) : set_bit(nullptr, last, name, type));
        }
        
        /*! @brief appends a bit in a context, used by the system to append bit in a certain context.
         @param context the context.
         @param name    the name of the bit.
         @param value   the literal value of the bit.
         @param type    the lan::type of the bit.
         */
        template<typename any>
        bool append(lan::db_bit * context, std::string const name, any const value, db_bit_type const type){
            data = context;
            if(data-> type == lan::Container and (data = data->lin)){
                data = get_last_bit(data);
                data->nex = new db_bit; data->nex->pre = data; data = data->nex;
                return ((type < lan::Array) ? set_bit(context, data, name, type, value) : set_bit(context, data, name, type));
            } return false;
        }
        
        /*! @brief inits a bit in a context, used by the system to init a bit in a certain context.
         @param context the context.
         @param name    the name of the bit.
         @param value   the literal value of the bit.
         @param type    the lan::type of the bit.
         */
        template<typename any>
        bool init_iter(lan::db_bit * context, std::string const name, any const value, db_bit_type const type){
            data = context;
            data->lin = new db_bit; data = data->lin;
            return ((type < lan::Array) ? set_bit(context, data, name, type, value) : set_bit(context, data, name, type));
        }
        
        /*! @brief appends a bit in a context, used by the system to append a bit in a certain context.
         @param context the context.
         @param name    the name of the bit.
         @param value   the literal value of the bit.
         @param type    the lan::type of the bit.
         */
        template<typename any>
        bool append_iter(lan::db_bit * context, std::string const name, any const value, db_bit_type const type){
            data = context;
            if(context->type == lan::Array and (data = data->lin)){
                data = get_last_bit(data);
                data->nex = new db_bit; data->nex->pre = data; data = data->nex;
                return ((type < lan::Array) ? set_bit(context, data, name, type, value) : set_bit(context, data, name, type));
            } return false;
        }
        
        /*! @brief appends a bit in an array.
         @param target  the array.
         @param value   the literal value of the bit.
         @param type    the lan::type of the bit.
         */
        template<typename any>
        bool iterate(std::string const target, any const value, db_bit_type const type){
            if((data = find_rec(target, lan::Container, lan::Array, first))) {
                return (data->lin) ? append_iter(data, "", value, type) :
                init_iter(data, "", value, type);
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, target+"{a}"));
        }
        
        /*! @brief declares an array or container bit in the main context.
         @param name    the name of the bit.
         @param type    the lan::type of the bit.
         */
        bool declare(std::string const name, db_bit_type const type);
        
        /*! @brief declares an array or container bit in a certain context.
         @param name    the name of the bit.
         @param type    the lan::type of the bit.
         */
        bool declare(std::string const target, std::string const name, db_bit_type const type);
        
        /*! @brief global dependece */
        std::string find__pop_address(std::string &);
        
        /*! @brief global dependece */
        lan::db_bit * find(std::string const, lan::db_bit *);
        
        /*! @brief global dependece */
        lan::db_bit * find_rec(std::string, lan::db_bit_type const , lan::db_bit *);
        
        /*! @brief global dependece */
        lan::db_bit * find_rec(std::string, lan::db_bit_type const , lan::db_bit_type const, lan::db_bit *);
        
        /*! @brief global dependece */
        lan::db_bit * find_var(std::string const, lan::db_bit *);
        
        /*! @brief global dependece */
        lan::db_bit * find_non_var(std::string const, lan::db_bit *);
        
        /*! @brief global dependece */
        lan::db_bit * find_any(std::string const, lan::db_bit_type const, lan::db_bit *);
        
        /* get */
        
        /*! @brief gets data a variable bit in the main context.
         @param name    the name of the bit.
         @param type    the type of the bit.
         Eg: any.get<int>(...);
         */
        template<typename any>
        any get(std::string const name, const lan::db_bit_type type){
            if((data = find_any(name, type, first)) and data->data and type < lan::Array){
                any * data_p = (any*)data->data;
                return ((any&)*data_p);
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name));
        }
        
        /*! @brief gets data from a variable bit from an array in the main context.
         @param name    the name of the bit.
         @param index   the index of the bit.
         @param type    the type of the bit.
         Eg: any.get<int>(...);
         */
        template<typename any>
        any get(std::string const name, size_t index, const lan::db_bit_type type){
            if((data = find_any(name, lan::Array, first))){
                data=data->lin;
                for(register_t it = 0 ; (it < index) and data ; it++, data=data->nex);
                if(data and data->type == type){
                    any * data_p = (any*)data->data;
                    return ((any&)*data_p);
                } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name+"["+std::to_string(index)+"]"));
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name));
        }
        
        /*! @brief gets a variable bit in a certain context.
         @param context the context.
         @param name    the name of the bit.
         @param type    the type of the bit.
         Eg: any.get<int>(...);
         */
        template<typename any>
        any get(std::string context, std::string const name, const lan::db_bit_type type){
            if ((data = find_rec(context, lan::Container, first))) {
                if((data = find_any(name, type, data->lin)) and data->data){
                    any * data_p = (any*)data->data;
                    return ((any&)*data_p);
                }
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name));
        }
        
        /*! @brief gets a variable bit in a certain context.
         @param bit   the bit.
         Eg: any.get<int>(...);
         */
        template<typename any>
        any get(db_bit * bit){
            if(bit){
                any * data_p = (any*)bit->data;
                return ((any&)*data_p);
            } return 0;
        }
        
        /*! @brief get dependece */
        lan::db_bit * get_array_bit(lan::db_bits *, size_t);
        
        /*! @brief get dependece */
        lan::db_bit * get_last_bit(lan::db_bits *);
        
        /*! @brief get dependece */
        lan::db_bit_type get_var_type(std::string const);
        
        /*! @brief returns the *data of a specific bit in the main context
         * i recomend to use anchor aka @ to get data of another context*/
        void * operator [](std::string);
        
        /* set */
        
        /*! @brief sets a variable bit in the main context.
         @param name    the name of the bit.
         @param value   the value of the bit.
         @param type    the type of the bit.
         @param overwrite   flag to override an existing bit with the same name and type.
         Eg: any.get<int>(...);
         */
        template<typename any>
        bool set(std::string const name, any const value, lan::db_bit_type type, bool overwrite = false){
            if(type >= lan::Array) return false;
            if((data = find_any(name, type, first))){
                if(not overwrite) throw lan::errors::overriding_bit_error(error_string(errors::_private::_overriding_bit_error, data->key));
                return set_bit(nullptr,data, name, type, value);
            } return (not first) ? init(name, value, type) : append(name, value, type);
        }
        
        /*! @brief sets a variable bit in an array.
         @param value   the value of the bit.
         @param index   the index of the bit.
         @param type    the type of the bit.
         Eg: any.get<int>(...);
         */
        template<typename any>
        bool set(std::string array, size_t index,  any const value, lan::db_bit_type type){
            if((data = find_rec(array, lan::Container, lan::Array, first))){
                data=data->lin;
                for(register_t it = 0 ; (it < index) and data ; it++, data=data->nex);
                if(data){
                    if(data->data)
                        data->~db_bit();
                    data->type = type;
                    if(type < lan::Array)
                        data->data = new any (value);
                }
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, array+"{Array}"));
        }
        
        /*! @brief sets a variable bit in a context.
         @param context the context.
         @param name    the name of the bit.
         @param value   the value of the bit.
         @param type    the type of the bit.
         @param overwrite   flag to override an existing bit with the same name and type.
         Eg: any.get<int>(...);
         */
        template<typename any>
        bool set(std::string const context, std::string const name, any const value, lan::db_bit_type type, bool overwrite = false){
            if(type >= lan::Array) return false;
            if((data = find_rec(context, lan::Container, first))){
                lan::db_bit * buffer = data;
                if((data = find_any(name, type, data->lin))) {
                    if(!overwrite) throw lan::errors::overriding_bit_error(error_string(errors::_private::_overriding_bit_error, data->key));
                    return set_bit(buffer, data, name, type, value);
                } return (buffer->lin) ? append(buffer, name, value, type) : init(buffer, name, value, type);
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, context+"{Container}"));;
        }
        
        /*! @brief Sets the anchor, aka "@", to a specific bit. Depending in how it's used, an anchor may potentialy speed up the program.
         @param array   the array.
         @param index   the index that we will be pointing to.
         */
        lan::anchor_t * set_anchor(std::string const array, size_t index){
            if ((data = find_rec(array, lan::Container, lan::Array, first)) and
                (anchor = get_array_bit(data, index))) return anchor;
            else throw lan::errors::anchor_name_error(error_string(errors::_private::_anchor_name_error, array+"["+std::to_string(index)+"]"));
        }
        
        /*! @brief Sets the anchor, aka "@", to a specific bit. Depending in how it's used, an anchor may potentialy speed up the program.
         @param context the context.
         */
        lan::anchor_t * set_anchor(std::string const context){
            if ((data = find_rec(context, lan::Container, first))) return (anchor = data);
            else throw lan::errors::anchor_name_error(error_string(errors::_private::_anchor_name_error, context));
        }
        
        /*! @brief Sets the anchor, aka "@", to a specific bit. Depending in how it's used, an anchor may potentialy speed up the program.
         @param anchor an bit pointer to use as anchor.
         */
        lan::anchor_t * set_anchor(lan::anchor_t * anchor){
            if ((this->anchor = anchor)) return anchor;
            else throw lan::errors::anchor_name_error(error_string(errors::_private::_anchor_name_error, "nullptr"));
        }
        
        /* remove */
        
        /*! @brief removes a bit.
         @param name the name of the bit.
         @param type the type of the bit.
         */
        bool remove(std::string const name, db_bit_type const type);
        
        /*! @brief removes a bit.
         @param context the context.
         @param name the name of the bit.
         @param type the type of the bit.
         */
        bool remove(std::string const context, std::string const name, db_bit_type const type);
        
        /*! @brief removes a bit.
         @param array the array.
         @param index the index of the bit in array.
         */
        bool remove(std::string const array, size_t index);
        
        /* -- */
        
        ~db();
    };
} /* namespace lan */
