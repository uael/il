# Jay - Ultra portable and fast programming language
[![Build Status](https://img.shields.io/travis/jayl/jay/master.svg)](https://travis-ci.org/jayl/jay)

**For fun**

An ultra processing language, .jay generate c files from a high level language syntax.
Currently supports functional parsing only :
- Multiple names
- Auto type
- Lambda
- Generics
- Tuple, multiple return types
- Strongly typed

```bash
include stdio;

main() : int => {
  puts("Hello World");
  return 0
}
```

##Deps
```bash
$ sudo apt install build-essential make autoconf automake libtool g++ bison flex 
```

##Build
```bash
$ git clone --recursive https://github.com/jayl/jay.git && cd jay
$ mkdir build && cd build

autotools
$ ../configure && make

cmake
$ cmake .. && make
```

##Test
```bash
autotools
$ make check

cmake
$ make check && test/check
```
