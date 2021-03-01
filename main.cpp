//
//  main.cpp
//  landb
//
//  Created by René Descartes Domingos Muala on 10/10/20.
//  Copyright © 2020 landia (René Muala). All rights reserved.
//

#include <iostream>
#include <string>
#include "landb.hpp"
#include <chrono>
#include "lanfile/file.hpp"

int main (int argc, const char * argv []) {
    
    lan::db students;
    
    students.declare("List", lan::Array);
    students.declare("Carlos", lan::Array);
    students.declare("Anna", lan::Array);
    students.declare("Antony", lan::Array);
    
    students.iterate<std::string>("List", "Carlos", lan::String);
    students.iterate<std::string>("List", "Anna", lan::String);
    students.iterate<std::string>("List", "Antony", lan::String);
    
    students.iterate("Carlos", 0, lan::Container);
    students.iterate("Anna", 0, lan::Container);
    students.iterate("Antony", 0, lan::Container);
    
    students.set_anchor("Carlos", 0);
    
    students.set<std::string>("@", "Full_name", "Carlos Eduard" ,lan::String, true);
    students.set<long long>("@", "Average", 13 ,lan::LongLong, true);
    students.set<bool>("@", "Passed", true ,lan::Bool, true);
    
    students.set_anchor("Anna", 0);
    
    students.set<std::string>("@", "Full_name", "Anna Cristin" ,lan::String, true);
    students.set<long long>("@", "Average", 16 ,lan::LongLong, true);
    students.set<bool>("@", "Passed", true ,lan::Bool, true);
    
    students.set_anchor("Antony", 0);
    
    students.set<std::string>("@", "Full_name", "Antony Jeff" ,lan::String, true);
    students.set<long long>("@", "Average", 9 ,lan::LongLong, true);
    students.set<bool>("@", "Passed", false ,lan::Bool, true);
    
    students.connect("generated_example.ldb");
    
    students.push();
    
}
