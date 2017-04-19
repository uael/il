# Jay - Toy programming language
[![Build Status](https://img.shields.io/travis/jayl/jay/master.svg)](https://travis-ci.org/jayl/jay)
[![Build status](https://ci.appveyor.com/api/projects/status/fb59sbx09mgpqyjv/branch/master?svg=true)](https://ci.appveyor.com/project/uael/jay/branch/master)
[![codecov](https://codecov.io/gh/jayl/jay/branch/master/graph/badge.svg)](https://codecov.io/gh/jayl/jay)

**For fun**

```bash
include stdio;

main() : int => {
  puts("Hello World");
  return 0
}
```

### Build
```bash
$ git clone --recursive https://github.com/jayl/jay.git && cd jay
$ mkdir build && cd build
$ cmake .. && make
```
