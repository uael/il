# Jay - Toy programming language
[![Build Status](https://img.shields.io/travis/jayl/jay/master.svg)](https://travis-ci.org/jayl/jay)

**For fun**

```bash
include stdio;

main() : int => {
  puts("Hello World");
  return 0
}
```

## Deps
```bash
$ sudo apt install build-essential make cmake gcc
```

## Build
```bash
$ git clone --recursive https://github.com/jayl/jay.git && cd jay
$ mkdir build && cd build
$ cmake .. && make
```
