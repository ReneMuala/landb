//
//  file.hpp
//  file
//
//  Created by René Descartes Domingos Muala on 28/01/21.
//  Copyright © 2021 landia (René Muala). All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <fstream>

namespace lan {
    
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
}
