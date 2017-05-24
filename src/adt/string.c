/*
 * Wulk - Wu uniform language kit
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
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