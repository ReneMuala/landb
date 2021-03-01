//
//  file.cpp
//  file
//
//  Created by René Descartes Domingos Muala on 28/01/21.
//  Copyright © 2021 landia (René Muala). All rights reserved.
//

#include "file.hpp"

namespace lan
{
    safe_file::safe_file(){
        file = nullptr;
        filename = "";
    }
    
    bool safe_file::open(std::string filename){
        close_fd();
        return (this->filename = filename).length() and check();
    }

    bool safe_file::check(){
        close_fd();
        file = fopen(filename.data(), "r+");
        file = (!file) ? fopen(filename.data(), "w+") : file;
        return (file);
    }
    
    bool safe_file::push(std::string data){
        close_fd();
        if((file = fopen(filename.data(), "w"))){
            return fwrite(data.data(), sizeof(char), data.length(), file);
        } return false;
    }
    
    std::string safe_file::pull(){
        close_fd();
        std::string data = ("");
        if((file = fopen(filename.data(), "r"))){
            char data_c = 0;
            while(!feof(file)){
                if(data_c != 0) data+=data_c; fread(&data_c, sizeof(char), 1, file); 
            }
        } return data;
    }

    size_t safe_file::length(){
        close_fd();
        size_t length = 0;
        if((file = fopen(filename.data(), "r"))){
            char data_c = 0;
            while(!feof(file)){
                fread(&data_c, sizeof(char), 1, file); length++;
            }
        } return length;
    }
    
    bool safe_file::close(){
        filename = "";
        close_fd();
        return !(filename).length();
    }

    bool safe_file::close_fd() {
        if(file) fclose(file);
            file = nullptr;
        return (file == nullptr);
    }
    
    safe_file::~safe_file(){
        close_fd();
    }
}
