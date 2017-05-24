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
#include <assert.h>

#include "xmalloc.h"

#include "attr.h"

#ifdef WITH_LTALLOC
#include "ltalloc.h"
#endif

static WULK_NORETURN xnomem(void) {
  fputs("out of memory", stderr);
  abort();
}

void *xmalloc(size_t size) {
#ifdef WITH_LTALLOC
  return ltalloc(size);
#else
  void *ret = malloc(size);
  if (!ret) xnomem();
  return ret;
#endif
}

void *xrealloc(void *ptr, size_t size) {
#ifdef WITH_LTALLOC
  if (ptr) {
    size_t uSize = ltalloc_usable_size(ptr);
    if (size <= uSize)
      return ptr;
    void *newp = ltalloc(size);
    memcpy(newp, ptr, uSize);
    ltfree(ptr);
    return newp;
  }
  return ltalloc(size);
#else
  void *ret = realloc(ptr, size);
  if (!ret) xnomem();
  return ret;
#endif
}

void *xcalloc(size_t n, size_t esize) {
#ifdef WITH_LTALLOC
  size_t size = n * esize;
  if (esize == 0 || size / esize == n) {
    xnomem();
  }
  void *result = ltalloc(size);
  if (result && size <= (64*1024))//memory obtained directly from the system are already zero filled
    memset(result, 0, size);
  return result;
#else
  void *ret = calloc(n, esize);
  if (!ret) xnomem();
  return ret;
#endif
}

void xfree(void *ptr) {
#ifdef WITH_LTALLOC
  ltfree(ptr);
#else
  free(ptr);
#endif
}
