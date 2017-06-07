/*
 * MIT License
 *
 * Copyright (c) 2017 Lucas Abel <www.github.com/uael>
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

/*!\file string.h
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_STRING_H__
# define U_STRING_H__

#include "types.h"

#ifdef __cplusplus
# include <cstring>
#else
# include <string.h>
#endif

#define USTR_MAX_PREALLOC (1024*1024)

typedef char *ustr_t;
typedef struct ustrh8 ustrh8_t;
typedef struct ustrh16 ustrh16_t;
typedef struct ustrh32 ustrh32_t;
typedef struct ustrh64 ustrh64_t;

PACKED(struct ustrh8 {
  uint8_t length; /* used */
  uint8_t capacity; /* excluding the header and null terminator */
  uint8_t flags; /* 3 lsb of type, 5 unused bits */
  char buffer[];
});

PACKED(struct ustrh16 {
  uint16_t length; /* used */
  uint16_t capacity; /* excluding the header and null terminator */
  uint8_t flags; /* 3 lsb of type, 5 unused bits */
  char buffer[];
});

PACKED(struct ustrh32 {
  uint32_t length; /* used */
  uint32_t capacity; /* excluding the header and null terminator */
  uint8_t flags; /* 3 lsb of type, 5 unused bits */
  char buffer[];
});

PACKED(struct ustrh64 {
  size_t length; /* used */
  size_t capacity; /* excluding the header and null terminator */
  uint8_t flags; /* 3 lsb of type, 5 unused bits */
  char buffer[];
});

#define USTR_TYPE_8  0U
#define USTR_TYPE_16 1U
#define USTR_TYPE_32 2U
#define USTR_TYPE_64 3U
#define USTR_HDR_VAR(T, s) ustrh##T##_t *sh = (void*)((s)-(sizeof(ustrh##T##_t)));
#define USTR_HDR(T, s) ((ustrh##T##_t *)((s)-(sizeof(ustrh##T##_t))))

static FORCEINLINE size_t ustrlen(const ustr_t s) {
  switch ((uint8_t) s[-1]) {
    case USTR_TYPE_8:
      return USTR_HDR(8, s)->length;
    case USTR_TYPE_16:
      return USTR_HDR(16, s)->length;
    case USTR_TYPE_32:
      return USTR_HDR(32, s)->length;
    case USTR_TYPE_64:
      return USTR_HDR(64, s)->length;
    default:
      break;
  }
  return 0;
}

static FORCEINLINE size_t ustravail(const ustr_t s) {
  switch ((uint8_t) s[-1]) {
    case USTR_TYPE_8: {
      USTR_HDR_VAR(8, s);
      return sh->capacity - sh->length;
    }
    case USTR_TYPE_16: {
      USTR_HDR_VAR(16, s);
      return sh->capacity - sh->length;
    }
    case USTR_TYPE_32: {
      USTR_HDR_VAR(32, s);
      return sh->capacity - sh->length;
    }
    case USTR_TYPE_64: {
      USTR_HDR_VAR(64, s);
      return sh->capacity - sh->length;
    }
    default:
      break;
  }
  return 0;
}

static FORCEINLINE size_t ustrcap(const ustr_t s) {
  switch ((uint8_t) s[-1]) {
    case USTR_TYPE_8:
      return USTR_HDR(8, s)->capacity;
    case USTR_TYPE_16:
      return USTR_HDR(16, s)->capacity;
    case USTR_TYPE_32:
      return USTR_HDR(32, s)->capacity;
    case USTR_TYPE_64:
      return USTR_HDR(64, s)->capacity;
    default:
      break;
  }
  return 0;
}

#define ustrempty() ustrn("", 0)

ustr_t  ustrn(const void *str, size_t n);
ustr_t  ustr(const char *init);
ustr_t  ustrdup(ustr_t s);
void    ustrfree(ustr_t s);
ustr_t  ustrresize(ustr_t s, size_t len);
ustr_t  ustrncat(ustr_t s, const void *t, size_t len);
ustr_t  ustrcat(ustr_t s, const char *t);
ustr_t  ustrxcat(ustr_t s, ustr_t t);
ustr_t  ustrncpy(ustr_t s, const char *t, size_t len);
ustr_t  ustrcpy(ustr_t s, const char *t);
ustr_t  ustrcatvprintf(ustr_t s, const char *fmt, va_list ap);
ustr_t  ustrcatprintf(ustr_t s, const char *fmt, ...) PRINTFCALL(2, 3);
ustr_t  ustrcatfmt(ustr_t s, char const *fmt, ...);
ustr_t  ustrtrim(ustr_t s, const char *cset);
void    ustrrange(ustr_t s, int start, int end);
void    ustrupdatelen(ustr_t s);
void    ustrclear(ustr_t s);
int     ustrcmp(ustr_t s1, ustr_t s2);
ustr_t *ustrsplitlen(const char *s, int len, const char *sep, int seplen, int *count);
void    ustrfreesplitres(ustr_t *tokens, int count);
void    ustrtolower(ustr_t s);
void    ustrtoupper(ustr_t s);
ustr_t  ustrfromlonglong(long long value);
ustr_t  ustrcatrepr(ustr_t s, const char *p, size_t len);
ustr_t *ustrsplitargs(const char *line, int *argc);
ustr_t  ustrmapchars(ustr_t s, const char *from, const char *to, size_t setlen);
ustr_t  ustrjoin(char **argv, int argc, char *sep);
ustr_t  ustrjoinsds(ustr_t *argv, int argc, const char *sep, size_t seplen);
ustr_t  ustrgrow(ustr_t s, size_t addlen);
void    ustrinclen(ustr_t s, int incr);
ustr_t  ustrpack(ustr_t s);
void   *ustrhptr(ustr_t s);

#endif /* U_STRING_H__ */
