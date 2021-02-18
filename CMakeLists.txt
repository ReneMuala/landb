cmake_minimum_required(VERSION 3.0)

project(landb)

add_executable(landb main.cpp)

install(TARGETS landb RUNTIME DESTINATION bin)
