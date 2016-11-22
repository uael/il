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

SYNTAX_TEST(function000, false, "1.8-10: syntax error\n",
  "main():int"
);

SYNTAX_TEST(function001, false, "1.11: syntax error\n",
  "main():int;"
);

SYNTAX_TEST(function002, false, "1.14: syntax error\n",
  "main():int { }"
);

SYNTAX_TEST(function003, true, "",
  "main():int => { }"
);

SYNTAX_TEST(function004, false, "1.15: syntax error\n",
  "main():int => 0"
);

SYNTAX_TEST(function005, false, "1.15: syntax error\n",
  "main():int => ;"
);

SYNTAX_TEST(function006, true, "",
  "main():int => 0;"
);

SYNTAX_TEST(function007, false, "1.15-20: syntax error\n",
  "main():int => return 0;"
);

SYNTAX_TEST(function008, true, "",
  "main():int => 0 * 0;"
);

SYNTAX_TEST(function009, false, "1.7: syntax error\n",
  "main(x):int => 0;"
);

SYNTAX_TEST(function010, true, "",
  "main(x:double):int => 0;"
);

SYNTAX_TEST(function011, false, "1.9: syntax error\n",
  "main(x,y):int => 0;"
);

SYNTAX_TEST(function012, true, "",
  "main(x,y:double):int => 0;"
);

SYNTAX_TEST(function013, true, "",
  "main(x,y:double = 0.0):int => x*y;"
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