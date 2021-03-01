//
//  landb.cpp
//  landb
//
//  Created by René Descartes Domingos Muala on 10/10/20.
//  Copyright © 2020 landia (René Muala). All rights reserved.
//

#include "landb.hpp"

namespace lan 
{
    char db_bit_table [11] = {  'b' ,   'i' ,
        'l' ,   'x' ,
        'f' ,   'd' ,
        'c' ,   's' ,
        'u' ,   'a' ,
        '#' };
        
        db::db(){
            reset_data();
        }
        
        /* -- */
        
        void db::erase_bits(db_bits * bits){
            while (bits) {
                data = bits;
                bits  = bits->nex;
                if(data->lin)
                    erase_bits(data->lin);
                delete data;
                data = nullptr;
            }
        }
        
        void db::erase_bit(db_bit * bit){
            if(bit){
                if(bit->pre)
                    bit->pre->nex = bit->nex;
                else if(bit->con)
                    bit->con->lin = bit->nex;
                first = (bit == first) ? first->nex : first ;
                last = (bit == last) ? last->pre : last;
                if(bit->lin)
                    erase_bits(bit->lin);
                delete bit;
                bit = nullptr;
            }
        }
        
        void db::reset_data(){
            data = first =
            last = anchor = nullptr;
            file.close();
        }
        
        void db::erase(){
            erase_bits(first);
            reset_data();
        }
        
        bool db::empty(){
            return (not last);
        }
        
        void db::print(size_t tabs, lan::db_bit * bits){
            lan::db_bit * buffer = (bits) ? bits : first;
            while (buffer) {
                for(register_t i=0;i<tabs;i++)printf("\t");
                if(buffer->type < Array){
                    printf("| %s %d 0x%llx\n", buffer->key.data(), buffer->type, (long long)buffer->data);
                } else {
                    if(buffer->type == Array)
                        printf("[ %s ]:\n", buffer->key.data());
                    else printf("( %s ):\n", buffer->key.data());
                    if(buffer->lin)print(tabs+1, buffer->lin);
                } buffer = buffer->nex;
            }
        }
        
        /* file */

        bool db::connect(std::string filename){
            return file.open(filename);
        }
        
        bool db::is_connected(){
            return file.check();
        }
        
        bool db::disconnect(){
            return file.close();
        }
        
        size_t db::str_get_str_end(std::string content, size_t spoint){
            size_t index;
            bool jmp (false);
            do {
                jmp = false;
                index = content.find('\"', spoint);
                if(content[index] == '"' and content[index-1] == '\\' and content[index-2] == '\\' and content[index-3] == '\\') jmp = true;
                spoint = index+1;
            } while(jmp or (index > 0 and index < content.length() and (content[index] == '"' and content[index-1] == '\\' and content[index-2] != '\\')));
            return index;
        }
        
        ssize_t db::str_find_first_of(std::string const target, const std::string content, size_t spoint){
            for(register_t i = spoint ; i < content.length() ; i ++){
                if(content[i] == '\"')
                    i = str_get_str_end(content, i+1);
                if(str_compare(content.data()[i] ,target))
                    return i;
            } return -1;
        }
        
        ssize_t db::str_find_first_not_of(std::string const target, const std::string content, size_t spoint){
            for(register_t i = spoint ; i < content.length() ; i ++){
                if(!str_compare(content.data()[i] ,target))
                    return i;
                if(content[i] == '\"')
                    i = str_get_str_end(content, i+1);
            } return -1;
        }
        
        bool db::str_compare(const char target, const std::string content){
            for(register_t i = 0 ; i < content.length() ; i++)
                if(content.data()[i] == target)
                    return true;
            return false;
        }
        
        std::string db::pop_next(std::string & content){
            ssize_t start_p = str_find_first_not_of(" \n\t\0", content, 0);
            ssize_t end_p = str_find_first_of("=:;()[] \n\t\0", content, start_p);
            end_p = (end_p <= 0 and start_p >= 0) ? content.length() : end_p;
            if (start_p >= 0 and end_p >= 0){
                end_p = (str_compare(content[start_p], ",=:;([])")) ? start_p+1 : end_p;
                std::string str = content.substr(start_p, end_p-start_p);
                content = content.substr(end_p, content.length() - str.length());
                return str;
            } else return "";
        }
        
        lan::db_bit * db::get_array_data(std::string & content){
            db_bit_type type = Unsafe;
            std::string buffer = pop_next(content);
            db_bit * bit = nullptr;
            type = convert_to_bit_type(buffer[0]);
            if(buffer != "]" and buffer != "(")
                content.insert(0, buffer);
            else if(buffer == "(")
                type = Container;
            if(type != Unsafe){
                switch(type){
                    case Container: bit = read_container_bit(content, true); break;
                    default: bit = read_value_bit(content, true); break;
                } bit->nex = (bit) ? get_array_data(content) : nullptr;
            } return bit;
        }
        
        lan::db_bit * db::get_container_data(std::string & content){
            db_bit * bit = nullptr, * f_bit = nullptr;
            if((bit = read_bit(content))){
                f_bit = bit;
                while((bit->nex = read_bit(content)))
                    bit = bit -> nex;
            } return f_bit;
        }
        
        lan::db_bit * db::read_container_bit(std::string & content, bool in_array){
            db_bit * bit = new db_bit;
            if(!in_array)
                bit->key = pop_next(content);
            bit->type = Container;
            if(pop_next(content) == ":") {
                bit->lin = get_container_data(content);
            } else {
                throw std::logic_error("LANDB: unable read container <" + bit->key + ">, the param <:> was not found.");   
            } return bit;
        }
        
        lan::db_bit * db::read_array_bit(std::string const name, std::string & content){
            db_bit * bit = new db_bit;
            bit->key = name;
            bit->type = Array;
            bit->data = nullptr;
            if(pop_next(content) != "[")
                throw std::logic_error("LANDB: landb: expected <[> before <" + pop_next(content) + "> ... " + pop_next(content));
            else
                bit->lin = get_array_data(content);
            return bit;
        }
        
        lan::db_bit_type db::convert_to_bit_type(char type){
            type = std::tolower(type);
            for(uint i = Bool ; i <= Container ; i++ )
                if(type == db_bit_table[i])
                    return (db_bit_type)i;
            return Unsafe;
        }
        
        void * db::get_var_data(db_bit_type type, std::string data){
            switch(type){
                case Bool: return new bool(atoi(data.data())); break;
                case Int:  return new int(atoi(data.data())); break;
                case Long: return new long(atol(data.data())); break;
                case LongLong: return new long long(atoll(data.data())); break;
                case Float: return new float(atof(data.data())); break;
                case Double: return new double(strtod(data.data(), nullptr));
                case Char: return new char(prepare_string_to_read(data.substr(1, data.length()-2))[0]);       break;
                case String: return new std::string(prepare_string_to_read(data.substr(1, data.length()-2))); break;
                default: return nullptr;
            }
        }
        
        std::string db::prepare_string_to_read(std::string src){
            std::string dst, temp ;
            bool in_escape (false);
            setbuf(stdout, NULL);
            for(auto ch  : src){
                if(ch == '\\' and !in_escape) in_escape = true;
                else in_escape = false;
                if(!in_escape)dst += ch;
            } 
            return dst;
        }
        
        lan::db_bit * db::read_var_bit(std::string name, db_bit_type type , std::string & content){
            db_bit * bit = new db_bit;
            bit->key = name;
            bit->type=type;
            bit->data = get_var_data(type, pop_next(content));
            return bit;
        }
        
        lan::db_bit * db::read_value_bit(std::string & content, bool in_array){
            std::string name;
            char c_type;
            db_bit_type type = Unsafe;
            lan::db_bit * bit = nullptr;
            if(!in_array and !((name = pop_next(content)).length() and (pop_next(content) == "="))){
                throw std::logic_error("LANDB: unable read value bit <" + bit->key + ">, invalid sintax.");   
            } if( (c_type = pop_next(content)[0]) and (pop_next(content) == ":")){
                type = convert_to_bit_type(c_type);
                switch(type){
                    case Array: bit = read_array_bit(name, content); break;
                    default: bit = read_var_bit(name, type, content); break;
                } 
            } return bit;
        }
        
        lan::db_bit * db::read_bit(std::string & content){
            std::string str;
            lan::db_bit * bit = nullptr;
            if((str = pop_next(content)).length()) {
                if(str == "(") {
                    bit = read_container_bit(content);
                } else if(str == "]" or str == ")") {
                    return nullptr;
                } else {
                    content.insert(0, str);
                    bit = read_value_bit(content);
                }
            } return bit;
        }
        
        lan::db_bits * db::read_all_bits(std::string content){
            std::string str;
            db_bit * bit = nullptr, * f_bit;
            f_bit = bit = read_bit(content);
            while(bit) {
                bit->nex = read_bit(content);
                bit = bit->nex;
            } return f_bit;
        }
        
        bool db::pull(){
            if(first)
                erase_bits(first);
            first = nullptr;
            std::string data_str = file.pull(); 
            return (first = read_all_bits(data_str));
        }
        
        std::string db::write_container_bit(db_bit * bits){
            db_bit * buffer = bits->lin;
            std::string bits_str = '(' + bits->key + ": ";
            while(buffer){
                bits_str += write_bit(buffer);
                buffer = buffer->nex;
            } bits_str += ')';
            return bits_str;
        }
        
        std::string db::write_array_bit(db_bit * bits){
            db_bit * buffer = bits->lin;
            std::string bits_str = ((bits->key.length()) ? bits->key + "=" : "") + "a:[";
            while(buffer){
                bits_str += write_bit(buffer, true);
                buffer = buffer->nex;
                if(buffer)bits_str += (' ');
            } bits_str += "]";
            return bits_str;
        }
        
        std::string db::prepare_char_to_write(char src){
            std::string dst, temp ;
            if(src == '\"' or src == '\\') dst += "\\";
                dst += src;
            return dst;
        }
        
        std::string db::prepare_string_to_write(std::string src){
            std::string dst, temp ;
            for(auto ch  : src){
                if(ch == '\"' or ch == '\\') dst += "\\";
                dst += ch;
            } return dst;
        }
        
        std::string db::write_var_bit(db_bit * bit, bool in_array){
            if(!bit->data) return "";
            std::string bit_str = bit->key + ((!in_array) ? '=' : '\0') + db_bit_table [bit->type] + ':';
            switch (bit->type) {
                case Bool:      bit_str += std::to_string(get<bool>(bit));      break;
                case Int:       bit_str += std::to_string(get<int>(bit));       break;
                case Long:      bit_str += std::to_string(get<long>(bit));      break;
                case LongLong:  bit_str += std::to_string(get<long long>(bit)); break;
                case Float:     bit_str += std::to_string(get<float>(bit));     break;
                case Double:    bit_str += std::to_string(get<double>(bit));    break;
                case Char:      bit_str += '"' + prepare_char_to_write(get<char>(bit)) + '"'; break;
                case String:    bit_str += '"' + prepare_string_to_write(get<std::string>(bit)) + '"';    break;
                default:        bit_str = ""; break;
            } return bit_str + (' ');
        }
        
        std::string db::write_bit(db_bit * bit, bool in_array){
            std::string bit_str;
            if(bit->type <= Unsafe){
                bit_str = write_var_bit(bit, in_array);
            } else if(bit->type == Array){
                bit_str = write_array_bit(bit);
            } else {
                bit_str = write_container_bit(bit);
            } return bit_str;
        }
        
        std::string db::write_all_bits(db_bits * bits){
            db_bit * buffer = bits;
            std::string bits_str = "";
            while(buffer){
                bits_str += write_bit(buffer);
                buffer = buffer->nex;
            }
            return bits_str;
        }
        
        bool db::push(){
            std::string data_str = write_all_bits(first);
            return file.push(data_str);
        }
        
        /* ... */
        
        std::string db::error_string(errors::_private::error_type type, std::string const name){
            switch (type) {
                case errors::_private::_bit_name_error:
                    return ("LANDB (bit_name_error): Unable to find bit \""+name+"\"."); break;
                case errors::_private::_anchor_name_error:
                    return ("LANDB (anchor_name_error): Invalid anchor address \""+name+"\"."); break;
                default: return (name); break;
            }
        }
        
        /* find */
        
        std::string db::find__pop_address(std::string & address){
            size_t length = address.find('.');
            length = (length < address.length()) ? length : address.length();
            std::string string = address.substr(0,length);
            address.erase(0, length+1);
            return string;
        }
        
        lan::db_bit * db::find(const std::string name, lan::db_bit * ref){
            lan::db_bit * buf = ref;
            if(name == "@") return anchor;
            while (buf) {
                if(buf->key == name) return buf;
                buf = buf->nex;
            } return nullptr;
        }
        
        lan::db_bits * db::find_rec(std::string address, lan::db_bit_type const type, lan::db_bit * ref){
            std::string string = find__pop_address(address);
            if(address.empty()) {
                return find_any(string, type, ref);
            } else if(not string.empty()) {
                if((ref = find_any(string, type, ref))){
                    return find_rec(address, type, ref->lin);
                }
            } return nullptr;
        }
        
        lan::db_bits * db::find_rec(std::string address, lan::db_bit_type const type, lan::db_bit_type const final_type, lan::db_bit * ref){
            std::string string = find__pop_address(address);
            if(address.empty()) {
                return find_any(string, final_type, ref);
            } else if(not string.empty()) {
                if((ref = find_any(string, type, ref))){
                    return find_rec(address, type, final_type, ref->lin);
                }
            } return nullptr;
        }
        
        lan::db_bit * db::find_var(const std::string name, lan::db_bit * ref){
            lan::db_bit * buf = nullptr;
            if(name == "@") return anchor;
            while ((buf = find(name, ref))) {
                if(buf->type < lan::Array)
                    return buf;
                ref = buf->nex;
            } return nullptr;
        }
        
        lan::db_bit * db::find_any(const std::string name, const lan::db_bit_type type, lan::db_bit * ref){
            lan::db_bit * buf = nullptr;
            if(name == "@") return anchor;
            while ((buf = find(name, ref))) {
                if(buf->type == type) return buf;
                ref = buf->nex;
            } return nullptr;
        }
        
        /* db general */
        
        bool db::declare(std::string const name, db_bit_type const type){
            return (last) ? append(name, 0, type) : init(name, 0, type);
        }
        
        bool db::declare(std::string const target, std::string const name, db_bit_type const type){
            return ((data = find_rec(target, lan::Container, first)) and data->lin)
            ? append(data, name, 0, type) : init(data, name, 0, type);
        }
        
        /* get */
        
        lan::db_bit * db::get_array_bit(lan::db_bits * array, size_t index){
            lan::db_bit * buffer = array;
            if(array->type == lan::Array and (buffer = buffer->lin)){
                for(register_t i = 0 ; i < index ; i++, buffer = buffer->nex){
                    if(!buffer)
                        return nullptr;
                }
            } return buffer;
        }
        
        lan::db_bit * db::get_last_bit(lan::db_bits * bits){
            while (bits->nex){
                bits = bits->nex;
            } return bits;
        }
        
        lan::db_bit_type db::get_var_type(const std::string name){
            if((data = find_var(name, first)))
                return data->type;
            else
                throw std::invalid_argument(error_string(errors::_private::_bit_name_error, name));
        }
        
        void * db::operator[](std::string const name){
            return find(name, first)->data;
        }
        
        /* set */
        
        /* remove */
        
        bool db::remove(const std::string target, const db_bit_type type){
            if ((data = find_rec(target, type, first))) {
                erase_bit(data);
                return true;
            } return false;
        }
        
        bool db::remove(const std::string address, const std::string target, const db_bit_type type){
            if ((data = find_rec(address, lan::Container, first))) {
                if ((data = find_rec(target, type, data->lin))) {
                    erase_bit(data);
                    return true;
                }
            } return false;
        }
        
        bool db::remove(const std::string target, size_t index){
            if ((data = find_rec(target, lan::Container, lan::Array, first)) and
                (data = get_array_bit(data, index))) {
                erase_bit(data);
                    return true;
            } return false;
        }
        
        db::~db(){
            if(last)
                erase();
            reset_data();
        }
        
} 
