/*
 *  MIT License
 *
 * Copyright (c) 2016 .DRY (Don't Repeat Yourself)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* $Id$ */

#include <iostream>
#include <fstream>
#include "driver.h"
#include "gtest/gtest.h"
#include "test.h"

SYNTAX_TEST(func, fibonacci, true, "",
  "fibonacci(n:int):int => {"
  "  if (n==0) {"
  "    return 0;"
  "  } else if (n == 1) {"
  "    return 1;"
  "  } else {"
  "    return fibonacci(n-1)+fibonacci(n-2);"
  "  }"
  "}"
);

SYNTAX_TEST(func, sort, true, "",
  "sort<T>(array : T[]) => {"
  "  var swap(i, j : T) => {"
  "      var t = array[i];"
  "      array[i] = array[j];"
  "      array[j] = t;"
  "    },"
  "    sort(l, r : T) => {"
  "      var p = array[(l + r) / 2], i = l, j = r;"
  "      while (i <= j) {"
  "        while (array[i] < p) i += 1;"
  "        while (array[i] < p) l -= 1;"
  "        if (i <= j) {"
  "          swap(i, j);"
  "          i += 1;"
  "          j -= 1;"
  "        }"
  "      }"
  "      if (l < j) sort(l, j);"
  "      if (j < r) sort(i, r);"
  "    };"
  "  sort1(0, count(array) - 1);"
  "}"
);

SYNTAX_TEST(func, tuple, true, "",
  "func(n : int) : int, *double => { return 1, &2.0; }"
);

SYNTAX_TEST(func, tuple1, true, "",
  "func(n : int) : int, tuple<*double, bool> => { return 1, &2.0, true; }"
);

SYNTAX_TEST(func, tuple3, true, "",
  "func(n : int) : int, tuple<*double, bool> => { return 1, (&2.0, true); }"
);

SYNTAX_TEST(func, initializer, true, "",
  "func() : int[] => {"
  "  return [0, 1, 2, 3, 4, 5, 6 * 5 ? 5 : 8];"
  "}"
);

SYNTAX_TEST(func, initializer2, true, "",
  "func() : int[][] => {"
  "  return [[0, 1, 2, 3],[0, 1, 2, 3],[0, 1, 2, 3],[0, 1, 2, 3]];"
  "}"
);

SYNTAX_TEST(func, initializer3, true, "",
  "func() => {"
  "  var map = {"
  "    foo: [0, 1, 2, 3],"
  "    \"bar\": {"
  "      0: [0, 1, 2, 3],"
  "      1: [0, 1, 2, 3],"
  "      2: [0, 1, 2, 3],"
  "      3 * 1: [0, 1, 2, 3]"
  "    }"
  "  };"
  "}"
);

RUN;

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */