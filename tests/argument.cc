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

using namespace dyc;
using namespace dyc::ast;

SYNTAX_TEST(args, empty_args, true, "",
  "foo()=>{}"
);

SYNTAX_TEST(args, untyped_one_arg, false, "1.6: syntax error\n",
  "foo(x)=>{}"
);

SYNTAX_TEST(args, untyped_multiple_arg, false, "1.8: syntax error\n",
  "foo(x,y)=>{}"
);

SYNTAX_TEST(args, one_arg, true, "",
  "foo(x:int)=>{}"
);

SYNTAX_TEST(args, multiple_arg, true, "",
  "foo(x,y:int)=>{}"
);

SYNTAX_TEST(args, valued_one_arg, true, "",
  "foo(x=0)=>{}"
);

SYNTAX_TEST(args, valued_multiple_arg, true, "",
  "foo(x,y=0)=>{}"
);

SYNTAX_TEST(args, valued_typed_one_arg, true, "",
  "foo(x:int=0)=>{}"
);

SYNTAX_TEST(args, valued_typed_multiple_arg, true, "",
  "foo(x,y:int=0)=>{}"
);

SYNTAX_TEST(args, typed_callable_arg, true, "",
  "foo(x:int())=>{}"
);

SYNTAX_TEST(args, typed_callable_arg_with_arg, true, "",
  "foo(x:int(int))=>{}"
);

SYNTAX_TEST(args, typed_callable_arg_with_args, true, "",
  "foo(x:int(int,int))=>{}"
);

SYNTAX_TEST(args, valued_typed_callable_arg, true, "",
  "foo(x:int(int,int)=(i,y)=>i*j)=>{}"
);

SYNTAX_TEST(args, valued_typed_callable_args, true, "",
  "foo(x:int(int,int)=(i,y)=>i*j,y:int(int,int)=(i,y)=>i*j)=>{}"
);

SYNTAX_TEST(args, valued_typed_callable_arg2, true, "",
  "foo(x(i:int,j:int):int=>i*j)=>{}"
);

SYNTAX_TEST(args, valued_typed_callable_args2, true, "",
  "foo(x(i:int,j:int):int=>i*j,y(i:int,j:int):int=>i*j)=>{}"
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