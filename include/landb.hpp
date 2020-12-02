//
//  landb.hpp
//  landb
//
//  Created by René Descartes Domingos Muala on 10/10/20.
//  Copyright © 2020 landia (René Muala). All rights reserved.
//

#ifndef landb_hpp
#define landb_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "lanconv/lanconv.hpp"

namespace lan {
    namespace db_consts {
        const std::string_view nil       = "@nil";
        const std::string_view undefined = "@undef";
    }
    class db {
        
        std::string data;
        
        FILE * file;
        
    public:
        
        db();
        
        bool connect(std::string_view const);
        
        bool disconnect();
        
        bool clear_data();
        
        std::string_view string(size_t = 0 , size_t = 0);

        ssize_t get_comment_scope(std::string_view, size_t const, size_t const);
        
        ssize_t get_scope(std::string_view const, char const, char const, size_t const, size_t const, size_t&, size_t&);
        
        ssize_t get_class_scope(std::string_view const, std::string_view, size_t const, size_t const, size_t&, size_t&);
        
        ssize_t get_entire_class_scope(std::string_view const, std::string_view, size_t const, size_t const, size_t&, size_t&);
        
        ssize_t get_var_scope(std::string_view const, std::string_view, size_t const, size_t const, size_t&, size_t&);
        
        ssize_t get_entire_var_scope(std::string_view const, std::string_view, size_t const, size_t const, size_t&, size_t&);
        
        static bool compare(char const, std::string_view const);

        static bool compare(std::string_view const, std::string_view const);
        
        size_t find(std::string_view const, std::string_view const, char const, std::string_view const, size_t const, size_t const);
        
        size_t find_class(std::string_view const, std::string_view const, size_t const, size_t const);
        
        size_t find_var(std::string_view const, std::string_view const, size_t const, size_t const);
        
        size_t find_any_char(std::string_view const, size_t const, size_t const, char const = 0);
        
        ssize_t find_char(std::string_view const, size_t const, size_t const, char const);
                
        bool is_array(std::string_view const);
        
        static ssize_t set_array_level(char const);
        
        size_t skip_comments_and_data_scopes(std::string_view const, size_t const, size_t const, size_t = 0);
        
        size_t skip_comments_and_data_scopes_at_get_scope(std::string_view const, size_t const, size_t const, size_t const, char const, char const);
        
        size_t get_array_length(std::string_view const, char const);
        
        ssize_t get_array_member_scope(std::string_view const, size_t const, char const, size_t &, size_t &);
        
        ssize_t get_array_member_scope(std::string , std::vector<ssize_t> const, char const, size_t &, size_t &);
        
        std::string_view get_array_member(std::string_view const, size_t const, char const = ',');
        
        std::string_view get_array_member(std::string, std::vector<ssize_t> const, char const = ',');
        
        std::vector<ssize_t> get_array_indices(std::string_view const);
        
        size_t count_char(std::string_view const, char const);
        
        std::string_view follow_name(std::string_view const , size_t const, size_t const, size_t &, size_t &);
        
        /* get */
        
        std::string_view get_class(std::string_view const);
        
        std::string_view get_var(std::string_view const);
        
        /* any.get_array("var_name[i]"); */
        std::string_view get_array(std::string_view const);
        
        std::string get_string(std::string_view const);
        
        char const * get_cstring(std::string_view const);
        
        int get_int(std::string_view const);
        
        float get_float(std::string_view const);
        
        double get_double(std::string_view const);
        
        long get_long(std::string_view const);
        
        long long get_long_long(std::string_view const);
        
        std::string_view operator [](std::string_view);
        
        /* set */

        bool set_class(std::string_view const, std::string_view const = "", bool = false);
        
        bool set_var(std::string_view const, std::string_view const, bool = false);
        
        /* any.set_array("var_name", "... , ..., ..."); */
        bool set_array(std::string_view const, std::string_view const, bool = false);
        
        /* any.set_array_memb("var_name[i]", "..."); */
        bool set_array_memb(std::string_view const, std::string_view const);
        
        bool set_array(std::string_view const, std::string const [], size_t const, bool = false);
        
        bool set_string_array(std::string_view const, std::string [], size_t const, bool = false);
        
        bool set_char_array(std::string_view const, char const [], size_t const, bool = false);
        
        bool set_int_array(std::string_view const, int const [], size_t const, bool = false);
        
        bool set_float_array(std::string_view const, float const [], size_t const, bool = false);
        
        bool set_double_array(std::string_view const, double const [], size_t const, bool = false);
        
        bool set_long_array(std::string_view const, long const [], size_t const, bool = false);

        bool set_long_long_array(std::string_view const, long long const [], size_t const, bool = false);
        
        bool set_string(std::string_view const, std::string_view const, bool = false);
        
        bool set_int(std::string_view const, int const, bool = false);
        
        bool set_float(std::string_view const, float const, bool = false);
        
        bool set_double(std::string_view const, double const, bool = false);
        
        bool set_long(std::string_view const, long const, bool = false);
        
        bool set_long_long(std::string_view const, long long const, bool = false);
        
        /* remove */
        
        bool rm_class(std::string_view const);
        
        bool rm_var(std::string_view const);
        
        /* -- */
        
        size_t push();
        
        size_t pull();
        
        void free();
        
        ~db();
    };
}

#endif /* landb_hpp */
