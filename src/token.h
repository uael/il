/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef   JL_TOKEN_H__
# define  JL_TOKEN_H__

#include <adt/deque.h>

typedef struct jl_token_t jl_token_t;
typedef struct jl_loc_t jl_loc_t;

typedef adt_deque_of(jl_token_t) jl_token_r;

enum jl_token_n {
  JL_TOKEN_KEYWORD = 0,
  JL_TOKEN_SYNTAX,
  JL_TOKEN_NUMBER,
  JL_TOKEN_IDENTIFIER,
  JL_TOKEN_STRING,
  JL_TOKEN_FLOAT,
  JL_TOKEN_INT
};

struct jl_loc_t {
  uint32_t lineno;
  uint32_t colno;
  uint32_t position;
  uint32_t file_id;
};

struct jl_token_t {
  char type;
  jl_loc_t loc;
  const char *name;
  uint32_t length;
  unsigned kind : 8;
  size_t cursor;
  union {
    const char *s;
    float f;
    int i;
  } u;
};

void jl_token_dtor(jl_token_t *self);

#endif /* JL_TOKEN_H__ */
