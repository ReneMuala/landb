# Landb 🗃
Landb is a C++ library to read and save variables in an <i>easy to understand</i> file structure.

## Features 🪧
<b> Speed </b>🤖
>  This version of landia::database is 10 times faster than the old one!

<b> Integrity </b> 🛡 
>  With landb your data wont get corrupted

<b> Weakly typed </b> ✏
>  Allows variable casting.


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

> <b>Container</b> - <i> (Similar to <b>namesmace in c++</b>) Allows object oriented variables </i>

## Stucture examples 📋

A string containing "hello world".

> message = s : "Hello world"

An numbers array

> numbers = a : [  i : 1  s : "one"  i : 2  s : "two" ]

## Compiling 🔨

<b> Clone this repo </b>
> git clone https://github.com/ReneMuala/landb.git

<b> Build with cmake </b> <i> (inside of the project dir)</i>
> cmake --build ./

## Linking ⛓

The file <b>liblandb.a</b> can be used to link statically with landb.
