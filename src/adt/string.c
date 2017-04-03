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
#include "string.h"

char *xstrdup(const char *str) {
  size_t len = strlen(str) + 1;
  return (char *) memcpy(xmalloc(len), str, len);
}

char *xstrndup(const char *s, size_t n) {
  char *result;
  size_t len = strlen(s);
  if (len > n) {
    len = n;
  }

  result = (char *) xmalloc(len + 1);
  if (!result)
    return 0;

  result[len] = '\0';
  return (char *) memcpy(result, s, len);
}

int asprintf(char **str, const char *fmt, ...) {
  int size = 0;
  va_list args;

  va_start(args, fmt);
  size = vasprintf(str, fmt, args);
  va_end(args);
  return size;
}

int vasprintf(char **str, const char *fmt, va_list args) {
  int size = 0;
  va_list tmpa;

  va_copy(tmpa, args);
  size = vsnprintf(NULL, 0, fmt, tmpa);
  va_end(tmpa);
  if (size < 0) { return -1; }
  *str = (char *) xmalloc((size_t) (size + 1));
  if (NULL == *str) { return -1; }
  size = vsprintf(*str, fmt, args);
  return size;
}