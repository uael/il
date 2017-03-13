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

#include <stdio.h>
#include <stdlib.h>
#include <types.h>

#include "compiler.h"

#define STR_N_SIZE(s) s, sizeof(s)-1

int main(int argc, char *argv[]) {
  jl_compiler_t compiler;
  jl_lexer_t lexer;
  jl_token_t token;

  jl_compiler_init(&compiler, argc, argv);

  jl_frontend_t c_fe;
  jl_frontend_init(&c_fe, JL_FRONTEND_C, &compiler);
  jl_lexer_init(&lexer, &c_fe, 0,
    STR_N_SIZE(
      "main autoo auto break case char const continue default do double \n "
      "else enum extern float for goto if inline int long register return short signed \n "
      "sizeof static struct switch typedef union unsigned void ! volatile # while % _Alignof ( ) * + , - . / ; < = > \n "
      "? ... || && >= <= << >> >>= <<= -> -- ++ -= += *= /= %= |= &= == != /****** int */ 1.2 1"
    )
  );

  while ((token = lexer.next(&lexer)).type != 0) {
    printf("token[%d, %s]\n", token.kind, token.name ? token.name : token.s);
  }

  jl_compiler_dtor(&compiler);
  jl_lexer_dtor(&lexer);

  return EXIT_SUCCESS;
}
