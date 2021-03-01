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
    students.set<double>("@", "Average", 13 ,lan::Double, true);
    students.set<bool>("@", "Passed", true ,lan::Bool, true);
    
    students.set_anchor("Anna", 0);
    
    students.set<std::string>("@", "Full_name", "Anna Cristin" ,lan::String, true);
    students.set<double>("@", "Average", 16 ,lan::Double, true);
    students.set<bool>("@", "Passed", true ,lan::Bool, true);
    
    students.set_anchor("Antony", 0);
    
    students.set<std::string>("@", "Full_name", "Antony Jeff" ,lan::String, true);
    students.set<double>("@", "Average", 9 ,lan::Double, true);
    students.set<bool>("@", "Passed", false ,lan::Bool, true);
    
    students.connect("generated_example.ldb");
    
    // students.print();
    
    std::cout << "Landia::db version: " << lan::db_version << "; Hello world!\n";
    
    std::string current_student;
    
    for( auto i : { 0, 1, 2}){
                current_student = students.get<std::string>("List", i, lan::String);
                students.set_anchor(current_student, 0);
                std::cout << "info: Printing description for student <" << current_student << ">"
                << std::endl
                << i + 1 <<".\tName: " << students.get<std::string>("@", "Full_name", lan::String) 
                << std::endl
                << "\tAverage : " <<  students.get<double>("@", "Average", lan::Double)
                << std::endl
                << "\tPassed : " <<  ((students.get<bool>("@", "Passed", lan::Bool) ? "Yes" : "No"))
                << std::endl;
        }
    
    students.push();
    
}
