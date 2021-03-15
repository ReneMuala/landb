cmake_minimum_required(VERSION 3.0)

project(landb)

add_compile_options(-std=c++17)

add_library(landb STATIC  landb.cpp)

add_executable(example main.cpp)

target_link_libraries(example landb)

install(TARGETS landb RUNTIME DESTINATION bin)
