# Jay - Ultra portable and fast programming language
[![Build Status](https://img.shields.io/travis/jayl/jay/master.svg)](https://travis-ci.org/jayl/jay)

**For fun**

An ultra processing language, .jay generate c files from an high level language syntax.
Currently support functional parsing only :
- Multiple name
- Auto type
- Lambda
- Generics
- Tuple, multiple return type
- Strongly typed

##Deps
```bash
$ sudo apt install build-essential make autoconf automake libtool g++ bison flex 
$ git submodule update --init
```

##Installation
```bash
$ ./configure
$ make
$ sudo make install 
```

##Development 
```bash
$ mkdir build && cd build
$ ../configure
$ make
```

##Test
```bash
$ make check
```
