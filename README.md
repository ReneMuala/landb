# Landb 🗃
C++ library to save & load variables easily.
 
## Why Landb? 🪧
- It saves time: <i>with landb you don't have to worry about writing and loading your program metadata, just `database.connect("filename.ldb")` then `database.pull()` to read and `database.push()` to write your variables.</I>
- It's fast: <i>it's really fast</i>😄
- multi-type arrays: <i>Landb Arrays may contain multiple types of data, even another arrays.</i>
- type-oriented variables: <i> In Landb `number=i:1` and `number=s:"one"` can exist in the same database without conflicts, because they have different types</i> 
- object-oriented variables: <i> Containers allows us to implement object oriented variables such as:</i>
```
(PersonA:
    Name=s:"Ty"
    Age=i:9
)

(PersonB: 
    Name=s:"Renato"
    Age=i:17
)
```



## What's new in version 2.5? 🎁
> <strong> 533: set_anchor(std::string const context) </strong> <i>fixed🔧</I>

## Examples ⚙️

#### Hello world 🤓

```
#include <iostream>
#include "landb.hpp"

int main(int argc, const char * argv[]) {
    
    lan::db database;
    
    database.set<std::string>("message", "hello world", lan::String);
    
    std::cout << database.get<std::string>("message", lan::String) << std::endl;
    
    return 0;
}
```

## Types 📑

| Type | Description |
|:--|:--|
|  Bool | Boolean |
| Int | Integer |
| Long |  Big integer |
| LongLong | Big integer (unsigned) |
|  Float  | Real number  |
| Double | Big real number  |
| Char | Character |
| String | Character sequence |
| Unsafe | Unsafe data, only exists at runtime and cannot be pushed |
| Array | Variable sequence |
| Container | (Similar to <b>namespace in c++</b>) Allows object oriented variables |

## Landb structure examples 📋
 
A string containing "hello world".

```
message = s : "Hello world"
```

An array with numbers and strings:

```
numbers = a : [  i : 1  s : "one"  i : 2  s : "two" ]
```

## Compiling 🔨

<b>1. Clone this repo </b>

```
# 1.1 clone using git:
git clone https://github.com/ReneMuala/landb.git
```

<b> 2. Build with cmake </b> <i> (inside of the project dir)</i>


```
# 2.1 getin landb foder :
cd landb 
```

```
# 2.2 configure cmake :
cmake ./ 
```

```
# 2.3 build with cmake
cmake --build ./
```

## Linking with your project⛓

| File | Description | Notes |
|:--|:--|:--|
| liblandb.a | Static library | Can be used to link binaries statically with landb |
| liblandbD.so | Shared library |  Can be used to link binaries dynamically with landb |

Note: to include landb as `#include "landb.hpp"` you need to copy <i>landb.hpp</I> to your source's dir.

<b> CMakeLists.txt example: </b>

```
cmake_minimum_required(VERSION 3.0)
project(sgep)
add_compile_options(-std=c++17)
add_executable(sgep main.cpp)
target_link_libraries(sgep ${CMAKE_SOURCE_DIR}/liblandb.a) # linking with liblandb.a in project source dir
install(TARGETS sgep RUNTIME DESTINATION bin)

```
