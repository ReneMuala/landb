# Landb 🗃
Landb is a C++ library to read and save variables in an <i>easy to understand</i> file structure.

## What's new in version 2.1?
> improved security
> 
> various bug fixes

## Features 🪧
<b> Speed </b>🤖

<b> Integrity </b> 🛡 

<b> Weakly typed </b> ✏

## Types 📑
> <b>Bool</b> - <i> Boolean </i>

> <b>Int</b> - <i> Integer </i>

> <b>Long</b> - <i>  Big integer </i>

> <b>LongLong</b> - <i>  Big integer (unsigned) </i>

> <b>FLoat</b> - <i>  Real number </i>

> <b>Double</b> - <i>  Big real number </i>

> <b>Char</b> - <i>  Character </i>

> <b>String</b> - <i>  Character sequence </i>

> <b>Unsafe</b> - <i>  Unsafe data, only exists at runtime and cannot be pushed </i>

> <b>Array</b> - <i>  Variable sequence </i>

> <b>Container</b> - <i> (Similar to <b>namespace in c++</b>) Allows object oriented variables </i>

## Stucture examples 📋

A string containing "hello world".

> message = s : "Hello world"

An array with numbers and strings:

> numbers = a : [  i : 1  s : "one"  i : 2  s : "two" ]

## Compiling 🔨

<b> Clone this repo </b>
> git clone https://github.com/ReneMuala/landb.git

<b> Build with cmake </b> <i> (inside of the project dir)</i>

> cmake ./

> cmake --build ./

## Linking with your project⛓

The file <b>liblandb.a</b> can be used to link statically with landb.

<b>example:</b>

> g++ main.cpp liblandb.a

<<<<<<< HEAD
<b>Attention: </b>Make sure to copy <b>liblandb.a</b> and <b>landb.hpp</b> to your project source directory.
=======
<b>Attention: </b>Make sure to copy <b>liblandb.a</b> and <b>landb.hpp</b> to your project's source directory.
>>>>>>> 2ebdf2f2bb0f0433eb3b9cd2169396b009287619

<b> CMakeLists.txt example: </b>

> cmake_minimum_required(VERSION 3.0)
>
> project(sgep)
>
> add_compile_options(-std=c++17)
> 
> add_executable(sgep main.cpp)
>
> target_link_libraries(sgep ${CMAKE_SOURCE_DIR}/liblandb.a) # linking with liblandb.a in project source dir
>
> install(TARGETS sgep RUNTIME DESTINATION bin)
