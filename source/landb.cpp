//
//  landb.cpp
//  landb
//
//  Created by René Descartes Domingos Muala on 10/10/20.
//  Copyright © 2020 landia (René Muala). All rights reserved.
//

#include "landb.hpp"

using namespace lan;

db::db(){
    file = nullptr;
    free();
}

bool db::connect(const std::string_view file_name){
    file = fopen(file_name.data(), "r+");
    file = (!file) ? fopen(file_name.data(), "w+") : file;
    return this -> file != nullptr;
}

bool db::is_connected(){
    return file != nullptr;
}

bool db::disconnect(){
    if(file){
        fclose(file);
        file = nullptr;
    } return file == nullptr;
}

bool db::clear_data(){
    data.clear();
    return data.length();
}

std::string_view db::string(size_t spoint, size_t epoint){
    return data;
}

ssize_t db::get_comment_scope(std::string_view content, const size_t spoint, const size_t epoint){
    register_t it = spoint+1;
    size_t in_data_beg, in_data_end;
    bool in_data = false;
    do {
        if(content[it] == '"' and content[it-1] != '\\')
            in_data = !in_data;
        else if (content[it] == '\'')
            it += get_scope(content, '\'', '\'', it, content.length(), in_data_beg, in_data_end);
    } while ((it++) and it < epoint and !(content[it] == '/' and !in_data));
    return it - spoint;
}

ssize_t db::get_scope(std::string_view const content, const char char1, const char char2, const size_t spoint, const size_t epoint, size_t & begin, size_t & end){
    begin = end = 0;
    size_t level = 0;
    for(register_t it = spoint ; it < epoint ; it++){
        it += skip_comments_and_data_scopes_at_get_scope(content, it, epoint, level, char1, char2);
        if(content[it] == char1 and content [it-1] != '\\' and (!begin or char1 != char2)){
            begin = (level == 0) ? it : begin;
            level++;
        } else if(content[it] == char2 and content [it-1] != '\\'){
            if(level == 1){
                end = it+1; break;
            } level--;
        }
    } return (end) ? end - begin : epoint - begin;
}

ssize_t db::get_class_scope(const std::string_view content, std::string_view class_name, const size_t spoint, const size_t epoint, size_t & begin, size_t & end){
    begin = end = 0;
    size_t pos = find_class(content, class_name, spoint, epoint);
    if(pos){
        pos+=class_name.length();
        if(content[find_any_char(content, pos, epoint, ' ')] == '{')
            get_scope(content, '{', '}', pos, epoint, begin, end);
    } return end - begin;
}

ssize_t db::get_entire_class_scope(const std::string_view content, std::string_view class_name, const size_t spoint, const size_t epoint, size_t & begin, size_t & end){
    begin = end = 0;
    size_t pos = find_class(content, class_name, spoint, epoint) - 1;
    if(pos >= 0){
        get_class_scope(content, class_name, pos, epoint, begin, end);
        begin = pos;
    } return end - begin;
}

ssize_t db::get_var_scope(const std::string_view content, std::string_view var_name, const size_t spoint, const size_t epoint, size_t & begin, size_t & end){
    begin = end = 0;
    size_t pos = find_var(content, var_name, spoint, epoint);
    if(pos){
        pos+=var_name.length();
        pos = find_any_char(content, pos, epoint, ' ');
        if(content[pos] == ':'){
            pos = find_any_char(content, pos+1, epoint, ' ');
            switch (content[pos]) {
                case '[': get_scope(content, '[', ']', pos, epoint, begin, end); break;
                default:
                    if((content[pos] >= 48 and content[pos] <= 57) or content[pos] == 64){
                        begin = pos; end = find_char(content, pos, epoint, ';')+1; end = not ( end < 0) ? end : 0;
                    } else get_scope(content, content[pos], content[pos], pos, epoint, begin, end);
                    break;
            }
        }
    } return end - begin;
}

ssize_t db::get_entire_var_scope(const std::string_view content, std::string_view var_name, const size_t spoint, const size_t epoint, size_t & begin, size_t & end) {
    begin = end = 0;
    size_t pos = find_var(content, var_name, spoint, epoint) - 1;
    if(pos >=0){
        get_var_scope(content, var_name, pos, epoint, begin, end);
        begin = pos;
    } return end - begin;
}

bool db::compare(const char target, const std::string_view content){
    for(auto let : content)
        if(let == target)
            return true;
    return false;
}

bool db::compare(const std::string_view target, const std::string_view content){
    for(auto tlet : target)
        for(auto let : content)
            if(tlet == let)
                return true;
    return false;
}

size_t db::find(std::string_view const content, std::string_view target, const char trigger, const std::string_view chars_ignore, size_t const spoint, size_t const epoint){
    for(register_t it = spoint ; it < epoint ; it++){
        it += skip_comments_and_data_scopes(content, it, epoint);
        if(content[it] == trigger){
            if(content.substr(++it, target.length()) == target and compare(content[it + target.length()], chars_ignore))
                return it;
        }
    } return 0;
}

size_t db::find_class(std::string_view const content, std::string_view target, size_t const spoint, size_t const epoint){
    return find(content, target, '#', " {", spoint, epoint);
}

size_t db::find_var(std::string_view const content, std::string_view target, size_t const spoint, size_t const epoint){
    return find(content, target, '#', " :", spoint, epoint);
}

size_t db::find_any_char(const std::string_view content, const size_t spoint, const size_t epoint, char const ignore){
    for(register_t it = spoint ; it < epoint ; it++){
        if(content[it] == '/'){
            it += get_comment_scope(string(), it, epoint);
        } else if(content[it] != ignore)
            return it;
    } return 0;
}

ssize_t db::find_char(const std::string_view content, const size_t spoint, const size_t epoint, const char target){
    for(register_t it = spoint ; it < epoint ; it++){
        if(content[it] == '/'){
            it += get_comment_scope(string(), it, epoint);
        } else if(content[it] == target)
            return it;
    } return -1;
}

bool db::is_array(const std::string_view content){
    size_t obrackets = find_char(content, 0, content.length(), '['),
    cbrackets = find_char(content, 0, content.length(), ']');
    return not (obrackets <0 or cbrackets <0)
    and obrackets < cbrackets;
}

ssize_t db::set_array_level(const char content){
    if(content == '[')
        return 1;
    else if(content == ']')
        return -1;
    else
        return 0;
}

size_t db::skip_comments_and_data_scopes(const std::string_view content, const size_t spoint, const size_t epoint, const size_t level){
    size_t in_data_beg, in_data_end,
    length = 0;
    if(content[spoint] == '/' and !level){
        length += get_comment_scope(content, spoint, epoint);
    } else if(content[spoint] == '"'){
        length += get_scope(content, '"', '"', spoint, epoint, in_data_beg, in_data_end);
    } else if(content[spoint] == '\'') {
        length += get_scope(content, '\'', '\'', spoint, epoint, in_data_beg, in_data_end);
    }
    return length;
}

size_t db::skip_comments_and_data_scopes_at_get_scope(const std::string_view content, const size_t spoint, const size_t epoint, size_t level, const char char1, const char char2){
    size_t in_data_beg, in_data_end,
    length = 0;
    if(content[spoint] == '/' and !level){
        length += get_comment_scope(content, spoint, epoint);
    } else if(content[spoint] == '"' and (char1 != '"' and char2 != '"')){
        length += get_scope(content, '"', '"', spoint, epoint, in_data_beg, in_data_end);
    } else if(content[spoint] == '\'' and (char1 != '\'' and char2 != '\''))
        length += get_scope(content, '\'', '\'', spoint, epoint, in_data_beg, in_data_end);
    return length;
}

size_t db::get_array_length(const std::string_view array_content, const char separator){
    size_t count = 0;
    size_t level = 0;
    if(is_array(array_content)){
        for(register_t it = 0 ; it < array_content.length() ; it++){
            it += skip_comments_and_data_scopes(array_content, it, array_content.length());
            level += set_array_level(array_content[it]);
            if(array_content[it] == separator and level == 1)
                count++;
        }
    } return count;
}

ssize_t db::get_array_member_scope(const std::string_view array_content, const size_t index, const char separator, size_t &begin, size_t &end){
    size_t count = 0, level = 0;
    begin = end = 0;
    if(index <= get_array_length(array_content, separator)){
        if(is_array(array_content)){
            for(register_t it = 0 ; it < array_content.length() ; it++){
                it += skip_comments_and_data_scopes(array_content, it, array_content.length());
                level += set_array_level(array_content[it]);
                count += (array_content[it] == separator and level == 1) ? 1 : 0;
                if(count == index and not begin){
                    begin = (it > 0) ? it : 1;
                    while (begin < array_content.length() and (compare(array_content[begin], " \t\n,."))) begin++;
                } else if(count > index or it+1 >= array_content.length()){
                    end = (it > begin) ? it : begin;
                    while (end > begin and (compare(array_content[end-1], " \t\n"))) end--;
                    break;
                }
            }
        } else return 0;
    } return end - begin;
}

ssize_t db::get_array_member_scope(std::string array_content, const std::vector<ssize_t> indices, const char separator, size_t &begin, size_t &end){
    begin = end = 0;
    size_t spoint = 0, epoint = array_content.length();
    if (is_array(array_content))
        for(auto index : indices) {
            get_array_member_scope(array_content, index, separator, spoint, epoint);
            array_content = array_content.substr(spoint, epoint-spoint);
            begin+=spoint;
        } end = begin + (epoint-spoint);
    return end - begin;
}

std::string_view db::get_array_member(const std::string_view arrary_content, const size_t index, const char separator){
    size_t spoint, epoint;
    get_array_member_scope(arrary_content, index, separator, spoint, epoint);
    return (epoint > spoint) ? arrary_content.substr(spoint, epoint - spoint) : lan::db_consts::nil;
}

std::string_view db::get_array_member(std::string array_content, const std::vector<ssize_t> indices, const char separator){
    if(is_array(array_content)){
        for(auto index : indices)
            array_content = get_array_member(array_content, index);
        return array_content;
    } else return db_consts::nil;
}

std::vector<ssize_t> db::get_array_indices(const std::string_view array_path){
    std::vector<ssize_t> indices;
    ssize_t spoint, epoint;
    for (register_t it = 0 ; it < array_path.length(); it++) {
        it = spoint = find_char(array_path, it, array_path.length(), '[') + 1;
        it = epoint = find_char(array_path, it, array_path.length(), ']');
        indices.resize(indices.size()+1);
        indices.at(indices.size()-1) = (!compare(array_path.substr(spoint, epoint - spoint), "[]")) ? to_int(array_path.substr(spoint, epoint - spoint)) : -1;
    } return indices;
}

std::string_view db::follow_name(const std::string_view name, const size_t spoint, const size_t epoint, size_t &begin, size_t &end){
    begin = spoint; end = epoint;
    std::string_view array_name = '[' + (std::string)name + ']';
    const size_t length = get_array_length(array_name, '.');
    for(register_t it = 0 ; it < length; it++){
        if(!get_class_scope(string(), get_array_member(array_name, it, '.'), begin, end, begin, end)) break;
    } return (length) ? (begin+end != 0) ? get_array_member(array_name, length, '.') : "" : name;
}

/* get */

std::string_view db::get_class(const std::string_view class_name){
    size_t spoint, epoint, beg, end;
    std::string_view class_short_name = follow_name((std::string)class_name, 0, string().length(), spoint, epoint);
    if(get_class_scope(string(), class_short_name, spoint, epoint, beg, end)){
        return string().substr(beg, end - beg);
    } return db_consts::undefined;
}

std::string_view db::get_var(const std::string_view var_name){
    size_t spoint, epoint, beg, end;
    std::string var_short_name = (std::string)follow_name((std::string)var_name, 0, string().length(), spoint, epoint);
    if(get_var_scope(string(), var_short_name, spoint, epoint, beg, end)){
        return string().substr(beg, end - beg);
    } return db_consts::undefined;
}

std::string_view db::get_array(const std::string_view array_name){
    if(is_array(array_name)){
        std::string_view array_content = get_var(array_name.substr(0, array_name.find('[')));
        return (array_content != db_consts::nil and array_content != db_consts::undefined) ? get_array_member((std::string)array_content, get_array_indices(array_name)).substr(0) : array_content;
    } return db_consts::undefined;
}

std::string db::get_string(const std::string_view var_name){
    std::string_view string = (is_array(var_name) ? get_array(var_name) : get_var(var_name));
    return (std::string)
    ((string != db_consts::nil and string != db_consts::undefined) ?
     ((string[0] == '"' and string[string.length()-1] == '"')  ? string.substr(1, string.length()-2) : db_consts::nil)
     : string);
}

const char * db::get_cstring(const std::string_view var_name){
    return get_string(var_name).c_str();
}

int db::get_int(const std::string_view var_name){
    return lan::to_int((is_array(var_name) ? get_array(var_name) : get_var(var_name)));
}

float db::get_float(const std::string_view var_name){
    return lan::to_float((is_array(var_name) ? get_array(var_name) : get_var(var_name)));
}

double db::get_double(const std::string_view var_name){
    return lan::to_double((is_array(var_name) ? get_array(var_name) : get_var(var_name)));
}

long db::get_long(const std::string_view var_name){
    return lan::to_long((is_array(var_name) ? get_array(var_name) : get_var(var_name)));
}

long long db::get_long_long(const std::string_view var_name){
    return lan::to_long_long((is_array(var_name) ? get_array(var_name) : get_var(var_name)));
}

std::string_view db::operator[](std::string_view address){
    return get_var(address);
}

/* set */

bool db::set_class(const std::string_view class_name, const std::string_view content, bool overwrite){
    size_t spoint, epoint, beg, end;
    std::string class_short_name = (std::string)follow_name((std::string)class_name, 0, string().length(), spoint, epoint);
    if(!get_var_scope(string(), class_short_name, spoint, epoint, beg, end) and class_short_name.length()){
        data.insert(epoint and compare('.', class_name) ? epoint-1 : string().length(), '#'+class_short_name+'{'+(std::string)content+'}');
        return true;
    } else if(overwrite){
        data.replace(beg, end-beg, (std::string)content);
    } return false;
}

bool db::set_var(const std::string_view var_name, const std::string_view content, bool overwrite){
    size_t spoint, epoint, beg, end;
    std::string var_short_name = (std::string)follow_name((std::string)var_name, 0, string().length(), spoint, epoint);
    if(!get_var_scope(string(), var_short_name, spoint, epoint, beg, end) and var_short_name.length()){
        data.insert(epoint and compare('.', var_name) ? epoint-1 : string().length(), '#'+var_short_name+':'+(std::string)content);
        return true;
    } else if(overwrite){
        data.replace(beg, end-beg, (std::string)content);
    } return false;
}

bool db::set_array(const std::string_view var_name, const std::string_view content, bool overwrite){
    return set_var(var_name, '['+(std::string)content+']', overwrite);
}

bool db::set_array_memb(std::string_view const array_name, std::string_view const content){
    size_t beg, end;
    std::string_view array_short_name = array_name.substr(0, array_name.find('['));
    std::string array_content = (std::string)get_var(array_short_name);
    if(is_array(array_name)){
        if(get_array_member_scope(array_content, get_array_indices(array_name), ',', beg, end)){
            array_content.replace(beg, end-beg, content);
            set_var(array_short_name, array_content, true);
            return true;
        }
    }
    return false;
}

bool db::set_array(const std::string_view var_name, const std::string * content, const size_t length, bool overwrite){
    std::string contents_str;
    for(register_t it = 0 ; it < length ; it++)
        contents_str += content[it] + ',';
    contents_str.pop_back();
    return set_array(var_name, contents_str, overwrite);
}

bool db::set_string_array(const std::string_view var_name, std::string * content, const size_t length, bool overwrite){
    for(register_t it = 0 ; it < length ; it++){
        content[it].insert(0, "\"");
        content[it].append("\"");
    }
    return set_array(var_name, content, length, overwrite);
}

bool db::set_char_array(const std::string_view var_name, char const * content, const size_t length, bool overwrite){
    std::string content_str [length];
    for(register_t it = 0 ; it < length ; it++ ){
        content_str[it] = content[it];
        content_str[it].insert(0, "'");
        content_str[it].append("'");
    }
    return set_array(var_name, content_str, length-1, overwrite);
}

bool db::set_int_array(const std::string_view var_name, const int * content, const size_t length, bool overwrite){
    std::string content_str [length];
    for(register_t it = 0 ; it < length ; it++ )
        content_str[it] = lan::to_string(content[it]);
    return set_array(var_name, content_str, length, overwrite);
}

bool db::set_float_array(const std::string_view var_name, const float * content, const size_t length, bool overwrite){
    std::string content_str [length];
    for(register_t it = 0 ; it < length ; it++ )
        content_str[it] = lan::to_string(content[it]);
    return set_array(var_name, content_str, length, overwrite);
}

bool db::set_double_array(const std::string_view var_name, const double * content, const size_t length, bool overwrite){
    std::string content_str [length];
    for(register_t it = 0 ; it < length ; it++ )
        content_str[it] = lan::to_string(content[it]);
    return set_array(var_name, content_str, length, overwrite);
}

bool db::set_long_array(const std::string_view var_name, const long * content, const size_t length, bool overwrite){
    std::string content_str [length];
    for(register_t it = 0 ; it < length ; it++ )
        content_str[it] = lan::to_string(content[it]);
    return set_array(var_name, content_str, length, overwrite);
}

bool db::set_long_long_array(const std::string_view var_name, const long long * content, const size_t length, bool overwrite){
    std::string content_str [length];
    for(register_t it = 0 ; it < length ; it++ )
        content_str[it] = lan::to_string(content[it]);
    return set_array(var_name, content_str, length, overwrite);
}

bool db::set_string(const std::string_view var_name, const std::string_view content, bool overwrite){
    return (is_array(var_name)) ? set_array_memb(var_name, '"'+(std::string)content+'"') : set_var(var_name, '"'+(std::string)content+'"', overwrite);
}

bool db::set_int(const std::string_view var_name, const int content, bool overwrite){
    return (is_array(var_name)) ? set_array_memb(var_name, lan::to_string(content)) : set_var(var_name, lan::to_string(content)+';', overwrite);
}

bool db::set_float(const std::string_view var_name, const float content, bool overwrite){
    return (is_array(var_name)) ? set_array_memb(var_name, lan::to_string(content)) : set_var(var_name, lan::to_string(content)+';', overwrite);
}

bool db::set_double(const std::string_view var_name, const double content, bool overwrite){
    return (is_array(var_name)) ? set_array_memb(var_name, lan::to_string(content)) : set_var(var_name, lan::to_string(content)+';', overwrite);
}

bool db::set_long(const std::string_view var_name, const long content, bool overwrite){
    return (is_array(var_name)) ? set_array_memb(var_name, lan::to_string(content)) : set_var(var_name, lan::to_string(content)+';', overwrite);
}

bool db::set_long_long(const std::string_view var_name, const long long content, bool overwrite){
    return (is_array(var_name)) ? set_array_memb(var_name, lan::to_string(content)) : set_var(var_name, lan::to_string(content)+';', overwrite);
}
/* remove */

bool db::rm_class(const std::string_view class_name){
    size_t spoint, epoint;
    if(get_entire_class_scope(string(), class_name, 0, string().length(), spoint, epoint)){
        data.erase(spoint, epoint - spoint);
        return true;
    } return false;
}

bool db::rm_var(const std::string_view var_name){
    size_t spoint, epoint;
    if(get_entire_var_scope(string(), var_name, 0, string().length(), spoint, epoint)){
        data.erase(spoint, epoint - spoint);
        return true;
    } return false;
}

/* -- */

size_t db::push(){
    ::fseek(file, 0, SEEK_SET);
    fwrite(data.data(), sizeof(char), data.length(), file);
    return data.length();
}

size_t db::pull(){
    char let = 0;
    ::fseek(file, 0, SEEK_SET);
    while (!feof(file)) {
        data+=let; fread(&let, sizeof(char), 1, file);
    }return data.length();
}

void db::free(){
    disconnect();
    clear_data();
}

db::~db(){
    free();
}
