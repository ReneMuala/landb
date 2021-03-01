cmake_minimum_required(VERSION 3.0)

project(landb)

add_compile_options(-std=c++17)

add_executable(landb main.cpp landb.cpp landb.cpp lanfile/file.cpp)

install(TARGETS landb RUNTIME DESTINATION bin)
