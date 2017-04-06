# Jay - Toy programming language
[![Build Status](https://img.shields.io/travis/jayl/jay/master.svg)](https://travis-ci.org/jayl/jay)
[![Build status](https://ci.appveyor.com/api/projects/status/fb59sbx09mgpqyjv/branch/master?svg=true)](https://ci.appveyor.com/project/uael/jay/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/jayl/jay/badge.svg?branch=master)](https://coveralls.io/github/jayl/jay?branch=master)
[![Code Health](https://landscape.io/github/jayl/jay/master/landscape.svg?style=flat)](https://landscape.io/github/jayl/jay/master)

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
