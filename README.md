# landb
Landb is a simple C++ library to manage _offline variables_  in landb file structure, it can store data in the following types:
- int and int []
- char and char []
- float and float []
- double and double []
- long and long []
- long long and long long []
- string and string []
- array and array []

## example

-- example.ldb --

/ class /

#stuffs {
  
  /int • long • long long value, ';' is necessary/
  
  #year : 2020;
  
  /float • double value, ';' is necessary/
  
  #pi : 3.14;
  
  /string value/
  
  #© : "copyright"
  
  /array/
  
  #languages : ["C++", "Makefile"]
  
}

-- main.cpp -- 

main(){

  lan::db database;
  
  database.connect("example.ldb");
  
  std::cout << database.get_string("languages[0]");
  
  // "C++"
  
}

## how to build
_clone_ the source code and type _make_
