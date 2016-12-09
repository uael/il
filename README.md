# dyc - under development 
[![Build Status](https://img.shields.io/travis/dotdry/dyc/master.svg)](https://travis-ci.org/dotdry/dyc)

**For fun**

.dry compiler.
An ultra processing language, .dry generate c files from an high level language syntax.
Currently support functionnal parsing only :
- Multiple name
- Auto type
- Lambda
- Generics
- Tuple, multiple return type
- Strongly typed

##Deps
```bash
$ sudo apt install build-essential make autoconf automake libtool g++ bison flex 
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
