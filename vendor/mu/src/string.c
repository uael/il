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

#include <stdio.h>
#include <assert.h>
#include <u/math.h>

#include "u/string.h"

#define ustrhsize(flags) ustrhsizes[(size_t)(flags)]
static const uint8_t ustrhsizes[5] = {
  sizeof(ustrh8_t),
  sizeof(ustrh16_t),
  sizeof(ustrh32_t),
  sizeof(ustrh64_t)
};

static FORCEINLINE uint8_t ustrtype(size_t ustrsize) {
  if (ustrsize < 1 << 8)
    return USTR_TYPE_8;
  if (ustrsize < 1 << 16)
    return USTR_TYPE_16;
#if (LONG_MAX == LLONG_MAX)
  if (ustrsize < 1ll << 32)
    return USTR_TYPE_32;
#endif
  return USTR_TYPE_64;
}

#define USTR_PTYPE(s) (((uint8_t *) (s)) - 1)
#define USTR_TYPE(s) *USTR_PTYPE(s)

#define USTR_SET_LEN(s, n) do { \
    switch (USTR_TYPE(s)) { \
      case USTR_TYPE_8: \
        USTR_HDR(8, s)->length = (uint8_t) (n); \
        break; \
      case USTR_TYPE_16: \
        USTR_HDR(16, s)->length = (uint16_t) (n); \
        break; \
      case USTR_TYPE_32: \
        USTR_HDR(32, s)->length = (uint32_t) (n); \
        break; \
      case USTR_TYPE_64: \
        USTR_HDR(64, s)->length = (n); \
        break; \
      default: \
        break; \
    } \
  } while (false)

#define USTR_SET_CAP(s, c) do { \
    switch (USTR_TYPE(s)) { \
      case USTR_TYPE_8: \
        USTR_HDR(8, s)->capacity = (uint8_t) (c); \
        break; \
      case USTR_TYPE_16: \
        USTR_HDR(16, s)->capacity = (uint16_t) (c); \
        break; \
      case USTR_TYPE_32: \
        USTR_HDR(32, s)->capacity = (uint32_t) (c); \
        break; \
      case USTR_TYPE_64: \
        USTR_HDR(64, s)->capacity = (c); \
        break; \
      default: \
        break; \
    } \
  } while (false)

ustr_t ustrn(const void *str, size_t n) {
  void *ustrh;
  ustr_t ustr;
  uint8_t type, hsize;
  size_t cap;

  if (n < 8) {
    cap = 8;
    type = USTR_TYPE_8;
  } else {
    cap = roundup32(n);
    type = ustrtype(cap);
  }
  hsize = ustrhsize(type);
  ustrh = malloc(hsize + cap + 1);
  if (ustrh == nullptr) {
    return nullptr;
  }
  if (str == nullptr) {
    memset(ustrh, 0, hsize + cap + 1);
  }
  ustr = (char *) ustrh + hsize;
  if (n && str) {
    memcpy(ustr, str, n);
  }
  ustr[n] = '\0';
  USTR_TYPE(ustr) = type;
  switch (type) {
    case USTR_TYPE_8: {
      USTR_HDR_VAR(8, ustr);
      sh->length = (uint8_t) n;
      sh->capacity = (uint8_t) cap;
      break;
    }
    case USTR_TYPE_16: {
      USTR_HDR_VAR(16, ustr);
      sh->length = (uint16_t) n;
      sh->capacity = (uint16_t) cap;
      break;
    }
    case USTR_TYPE_32: {
      USTR_HDR_VAR(32, ustr);
      sh->length = (uint32_t) n;
      sh->capacity = (uint32_t) cap;
      break;
    }
    case USTR_TYPE_64: {
      USTR_HDR_VAR(64, ustr);
      sh->length = n;
      sh->capacity = cap;
      break;
    }
    default:
      break;
  }
  return ustr;
}

/* Create a new ustr_t string starting from a null terminated C ustr. */
ustr_t ustr(const char *init) {
  size_t initlen = (init == nullptr) ? 0 : strlen(init);
  return ustrn(init, initlen);
}

/* Duplicate an ustr_t ustr. */
ustr_t ustrdup(ustr_t s) {
  return ustrn(s, ustrlen(s));
}

/* Free an ustr_t ustr. No operation is performed if 's' is nullptr. */
void ustrfree(ustr_t s) {
  if (s) {
    free((char *) s - ustrhsize(USTR_TYPE(s)));
  }
}

/* Set the ustr_t string length to the length as obtained with strlen(), so
 * considering as content only up to the first null term character.
 *
 * This function is useful when the ustr_t string is hacked manually in some
 * way, like in the following example:
 *
 * s = ustr("foobar");
 * s[2] = '\0';
 * ustrupdatelen(s);
 * printf("%d\n", ustrlen(s));
 *
 * The output will be "2", but if we comment out the call to ustrupdatelen()
 * the output will be "6" as the string was modified but the logical length
 * remains 6 bytes. */
void ustrupdatelen(ustr_t s) {
  USTR_SET_LEN(s, strlen(s));
}

/* Modify an ustr_t ustr in-place to make it empty (zero length).
 * However all the existing buffer is not discarded but set as free space
 * so that next append operations will not require allocations up to the
 * number of bytes previously available. */
void ustrclear(ustr_t s) {
  USTR_SET_LEN(s, 0);
  s[0] = '\0';
}

//todo rework that horribly designed func body, oh god
/* Enlarge the free space at the end of the ustr_t string so that the caller
 * is sure that after calling this function can overwrite up to addlen
 * bytes after the end of the string, plus one more byte for nul term.
 *
 * Note: this does not change the *length* of the ustr_t ustr as returned
 * by ustrlen(), but only the free buffer space we have. */
ustr_t ustrgrow(ustr_t s, size_t addlen) {
  void *sh, *newsh;
  size_t avail = ustravail(s);
  size_t len, newlen;
  char type, oldtype = (char) (USTR_TYPE(s));
  int hdrlen;

  /* Return ASAP if there is enough space left. */
  if (avail >= addlen)
    return s;

  len = ustrlen(s);
  sh = (char *) s - ustrhsize(oldtype);
  newlen = (len + addlen);
  if (newlen < USTR_MAX_PREALLOC)
    newlen *= 2;
  else
    newlen += USTR_MAX_PREALLOC;

  type = ustrtype(newlen);

  hdrlen = ustrhsize(type);
  if (oldtype == type) {
    newsh = realloc(sh, hdrlen + newlen + 1);
    if (newsh == nullptr)
      return nullptr;
    s = (char *) newsh + hdrlen;
  } else {
    /* Since the header size changes, need to move the ustr forward,
     * and can't use realloc */
    newsh = malloc(hdrlen + newlen + 1);
    if (newsh == nullptr)
      return nullptr;
    memcpy((char *) newsh + hdrlen, s, len + 1);
    free(sh);
    s = (char *) newsh + hdrlen;
    USTR_TYPE(s) = (uint8_t) type;
    USTR_SET_LEN(s, len);
  }
  USTR_SET_CAP(s, newlen);
  return s;
}

/* Reallocate the ustr_t string so that it has no free space at the end. The
 * contained string remains not altered, but next concatenation operations
 * will require a reallocation.
 *
 * After the call, the passed ustr_t ustr is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
ustr_t ustrpack(ustr_t s) {
  void *sh, *newsh;
  char type, oldtype = (char) (USTR_TYPE(s));
  int hdrlen;
  size_t len = ustrlen(s);
  sh = (char *) s - ustrhsize(oldtype);

  type = ustrtype(len);
  hdrlen = ustrhsize(type);
  if (oldtype == type) {
    newsh = realloc(sh, hdrlen + len + 1);
    if (newsh == nullptr)
      return nullptr;
    s = (char *) newsh + hdrlen;
  } else {
    newsh = malloc(hdrlen + len + 1);
    if (newsh == nullptr)
      return nullptr;
    memcpy((char *) newsh + hdrlen, s, len + 1);
    free(sh);
    s = (char *) newsh + hdrlen;
    USTR_TYPE(s) = (uint8_t) type;
    USTR_SET_LEN(s, len);
  }
  USTR_SET_CAP(s, len);
  return s;
}

/* Return the pointer of the actual SDS allocation (normally SDS strings
 * are referenced by the start of the ustr buffer). */
void *ustrhptr(ustr_t s) {
  return (void *) (s - ustrhsize(USTR_TYPE(s)));
}

/* Increment the ustr_t length and decrements the left free space at the
 * end of the string according to 'incr'. Also set the null term
 * in the new end of the string.
 *
 * This function is used in order to fix the ustr length after the
 * user calls ustrgrow(), writes something after the end of
 * the current string, and finally needs to set the new length.
 *
 * Note: it is possible to use a negative increment in order to
 * right-trim the string.
 *
 * Usage example:
 *
 * Using ustrIncrLen() and ustrgrow() it is possible to mount the
 * following schema, to cat bytes coming from the kernel to the end of an
 * ustr_t string without copying into an intermediate buffer:
 *
 * oldlen = ustrlen(s);
 * s = ustrgrow(s, BUFFER_SIZE);
 * nread = read(fd, s+oldlen, BUFFER_SIZE);
 * ... check for nread <= 0 and handle it ...
 * ustrinclen(s, nread);
 */
void ustrinclen(ustr_t s, int incr) {
  uint8_t flags = (uint8_t) USTR_TYPE(s);
  size_t len;
  switch (flags) {
    case USTR_TYPE_8: {
      USTR_HDR_VAR(8, s);
      assert((incr >= 0 && sh->capacity - sh->length >= incr) || (incr < 0 && sh->length >= (unsigned int) (-incr)));
      len = (sh->length += (uint8_t) incr);
      break;
    }
    case USTR_TYPE_16: {
      USTR_HDR_VAR(16, s);
      assert((incr >= 0 && sh->capacity - sh->length >= incr) || (incr < 0 && sh->length >= (unsigned int) (-incr)));
      len = (sh->length += (uint16_t) incr);
      break;
    }
    case USTR_TYPE_32: {
      USTR_HDR_VAR(32, s);
      assert(
        (incr >= 0 && sh->capacity - sh->length >= (unsigned int) incr)
          || (incr < 0 && sh->length >= (unsigned int) (-incr)));
      len = (sh->length += incr);
      break;
    }
    case USTR_TYPE_64: {
      USTR_HDR_VAR(64, s);
      assert(
        (incr >= 0 && sh->capacity - sh->length >= (uint64_t) incr) || (incr < 0 && sh->length >= (uint64_t) (-incr)));
      len = (sh->length += incr);
      break;
    }
    default:
      len = 0; /* Just to avoid compilation warnings. */
  }
  s[len] = '\0';
}

/* Grow the ustr_t to have the specified length. Bytes that were not part of
 * the original length of the ustr_t will be set to zero.
 *
 * if the specified length is smaller than the current length, no operation
 * is performed. */
ustr_t ustrresize(ustr_t s, size_t len) {
  size_t curlen = ustrlen(s);

  if (len <= curlen)
    return s;
  s = ustrgrow(s, len - curlen);
  if (s == nullptr)
    return nullptr;

  /* Make sure added region doesn't contain garbage */
  memset(s + curlen, 0, (len - curlen + 1)); /* also set trailing \0 byte */
  USTR_SET_LEN(s, len);
  return s;
}

/* Append the specified binary-safe ustr pointed by 't' of 'length' bytes to the
 * end of the specified ustr_t string 's'.
 *
 * After the call, the passed ustr_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
ustr_t ustrncat(ustr_t s, const void *t, size_t len) {
  size_t curlen = ustrlen(s);

  s = ustrgrow(s, len);
  if (s == nullptr)
    return nullptr;
  memcpy(s + curlen, t, len);
  len += curlen;
  USTR_SET_LEN(s, len);
  s[len] = '\0';
  return s;
}

/* Append the specified null termianted C string to the ustr_t ustr 's'.
 *
 * After the call, the passed ustr_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
ustr_t ustrcat(ustr_t s, const char *t) {
  return ustrncat(s, t, strlen(t));
}

/* Append the specified ustr_t 't' to the existing ustr_t 's'.
 *
 * After the call, the modified ustr_t ustr is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
ustr_t ustrxcat(ustr_t s, ustr_t t) {
  return ustrncat(s, t, ustrlen(t));
}

/* Destructively modify the ustr_t string 's' to hold the specified binary
 * safe ustr pointed by 't' of length 'length' bytes. */
ustr_t ustrncpy(ustr_t s, const char *t, size_t len) {
  if (ustrcap(s) < len) {
    s = ustrgrow(s, len - ustrlen(s));
    if (s == nullptr)
      return nullptr;
  }
  memcpy(s, t, len);
  s[len] = '\0';
  USTR_SET_LEN(s, len);
  return s;
}

/* Like ustrncpy() but 't' must be a null-termined ustr so that the length
 * of the string is obtained with strlen(). */
ustr_t ustrcpy(ustr_t s, const char *t) {
  return ustrncpy(s, t, strlen(t));
}

/* Helper for ustrcatlonglong() doing the actual number -> string
 * conversion. 's' must point to a string with room for at least
 * USTR_LLSTR_SIZE bytes.
 *
 * The function returns the length of the null-terminated ustr
 * representation stored at 's'. */
#define USTR_LLSTR_SIZE 21
static int ustrll2str(char *s, long long value) {
  char *p, aux;
  unsigned long long v;
  size_t l;

  /* Generate the ustr representation, this method produces
   * an reversed string. */
  v = (unsigned long long) ((value < 0) ? -value : value);
  p = s;
  do {
    *p++ = (char) ('0' + (v % 10));
    v /= 10;
  } while (v);
  if (value < 0)
    *p++ = '-';

  /* Compute length and add null term. */
  l = p - s;
  *p = '\0';

  /* Reverse the ustr. */
  p--;
  while (s < p) {
    aux = *s;
    *s = *p;
    *p = aux;
    s++;
    p--;
  }
  return (int) l;
}

/* Identical ustrll2str(), but for unsigned long long type. */
static int ustrull2str(char *s, unsigned long long v) {
  char *p, aux;
  size_t l;

  /* Generate the ustr representation, this method produces
   * an reversed string. */
  p = s;
  do {
    *p++ = (char) ('0' + (v % 10));
    v /= 10;
  } while (v);

  /* Compute length and add null term. */
  l = p - s;
  *p = '\0';

  /* Reverse the ustr. */
  p--;
  while (s < p) {
    aux = *s;
    *s = *p;
    *p = aux;
    s++;
    p--;
  }
  return (int) l;
}

/* Create an ustr_t ustr from a long long value. It is much faster than:
 *
 * ustrcatprintf(ustrempty(),"%lld\n", value);
 */
ustr_t ustrfromlonglong(long long value) {
  char buf[USTR_LLSTR_SIZE];
  int len = ustrll2str(buf, value);

  return ustrn(buf, (size_t) len);
}

/* Like ustrcatprintf() but gets va_list instead of being variadic. */
ustr_t ustrcatvprintf(ustr_t s, const char *fmt, va_list ap) {
  va_list cpy;
  char staticbuf[1024], *buf = staticbuf, *t;
  size_t buflen = strlen(fmt) * 2;

  /* We try to start using a static buffer for speed.
   * If not possible we revert to heap allocation. */
  if (buflen > sizeof(staticbuf)) {
    buf = malloc(buflen);
    if (buf == nullptr)
      return nullptr;
  } else {
    buflen = sizeof(staticbuf);
  }

  /* Try with buffers two times bigger every time we fail to
   * fit the ustr in the current buffer size. */
  while (1) {
    buf[buflen - 2] = '\0';
    va_copy(cpy, ap);
    vsnprintf(buf, buflen, fmt, cpy);
    va_end(cpy);
    if (buf[buflen - 2] != '\0') {
      if (buf != staticbuf)
        free(buf);
      buflen *= 2;
      buf = malloc(buflen);
      if (buf == nullptr)
        return nullptr;
      continue;
    }
    break;
  }

  /* Finally concat the obtained string to the SDS ustr and return it. */
  t = ustrcat(s, buf);
  if (buf != staticbuf)
    free(buf);
  return t;
}

/* Append to the ustr_t string 's' a string obtained using printf-alike format
 * specifier.
 *
 * After the call, the modified ustr_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call.
 *
 * Example:
 *
 * s = string("Sum is: ");
 * s = ustrcatprintf(s,"%d+%d = %d",a,b,a+b).
 *
 * Often you need to create a ustr from scratch with the printf-alike
 * format. When this is the need, just use ustrempty() as the target string:
 *
 * s = ustrcatprintf(ustrempty(), "... your format ...", args);
 */
ustr_t ustrcatprintf(ustr_t s, const char *fmt, ...) {
  va_list ap;
  char *t;
  va_start(ap, fmt);
  t = ustrcatvprintf(s, fmt, ap);
  va_end(ap);
  return t;
}

/* This function is similar to ustrcatprintf, but much faster as it does
 * not rely on sprintf() family functions implemented by the libc that
 * are often very slow. Moreover directly handling the ustr_t string as
 * new data is concatenated provides a performance improvement.
 *
 * However this function only handles an incompatible subset of printf-alike
 * format specifiers:
 *
 * %s - C String
 * %S - SDS ustr
 * %i - signed int
 * %I - 64 bit signed integer (long long, int64_t)
 * %u - unsigned int
 * %U - 64 bit unsigned integer (unsigned long long, uint64_t)
 * %% - Verbatim "%" character.
 */
ustr_t ustrcatfmt(ustr_t s, char const *fmt, ...) {
  size_t initlen = ustrlen(s);
  const char *f;
  int i;
  va_list ap;

  va_start(ap, fmt);
  f = fmt;    /* Next format specifier byte to process. */
  i = (int) initlen; /* Position of the next byte to write to dest str. */
  while (*f) {
    char next, *str;
    size_t l;
    long long num;
    unsigned long long unum;

    /* Make sure there is always space for at least 1 char. */
    if (ustravail(s) == 0) {
      s = ustrgrow(s, 1);
    }

    switch (*f) {
      case '%':
        next = *(f + 1);
        f++;
        switch (next) {
          case 's':
          case 'S':
            str = va_arg(ap, char*);
            l = (next == 's') ? strlen(str) : ustrlen(str);
            if (ustravail(s) < l) {
              s = ustrgrow(s, l);
            }
            memcpy(s + i, str, l);
            initlen += l;
            USTR_SET_LEN(s, initlen);
            i += l;
            break;
          case 'i':
          case 'I':
            if (next == 'i')
              num = va_arg(ap, int);
            else
              num = va_arg(ap, long long);
            {
              char buf[USTR_LLSTR_SIZE];
              l = (size_t) ustrll2str(buf, num);
              if (ustravail(s) < l) {
                s = ustrgrow(s, l);
              }
              memcpy(s + i, buf, l);
              initlen += l;
              USTR_SET_LEN(s, initlen);
              i += l;
            }
            break;
          case 'u':
          case 'U':
            if (next == 'u')
              unum = va_arg(ap, unsigned int);
            else
              unum = va_arg(ap, unsigned long long);
            {
              char buf[USTR_LLSTR_SIZE];
              l = (size_t) ustrull2str(buf, unum);
              if (ustravail(s) < l) {
                s = ustrgrow(s, l);
              }
              memcpy(s + i, buf, l);
              initlen += l;
              USTR_SET_LEN(s, initlen);
              i += l;
            }
            break;
          default: /* Handle %% and generally %<unknown>. */
            s[i++] = next;
            ++initlen;
            USTR_SET_LEN(s, initlen);
            break;
        }
        break;
      default:
        s[i++] = *f;
        ++initlen;
        USTR_SET_LEN(s, initlen);
        break;
    }
    f++;
  }
  va_end(ap);

  /* Add null-term */
  s[i] = '\0';
  return s;
}

/* Remove the part of the string from left and from right composed just of
 * contiguous characters found in 'cset', that is a null terminted C string.
 *
 * After the call, the modified ustr_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call.
 *
 * Example:
 *
 * s = ustr("AA...AA.a.aa.aHelloWorld     :::");
 * s = ustrtrim(s,"Aa. :");
 * printf("%s\n", s);
 *
 * Output will be just "Hello World".
 */
ustr_t ustrtrim(ustr_t s, const char *cset) {
  char *start, *end, *sp, *ep;
  size_t len;

  sp = start = s;
  ep = end = s + ustrlen(s) - 1;
  while (sp <= end && strchr(cset, *sp))
    sp++;
  while (ep > sp && strchr(cset, *ep))
    ep--;
  len = (sp > ep) ? 0 : ((ep - sp) + 1);
  if (s != sp)
    memmove(s, sp, len);
  s[len] = '\0';
  USTR_SET_LEN(s, len);
  return s;
}

/* Turn the string into a smaller (or equal) ustr containing only the
 * substring specified by the 'start' and 'end' indexes.
 *
 * start and end can be negative, where -1 means the last character of the
 * string, -2 the penultimate character, and so forth.
 *
 * The interval is inclusive, so the start and end characters will be part
 * of the resulting string.
 *
 * The string is modified in-place.
 *
 * Example:
 *
 * s = string("Hello World");
 * ustrrange(s,1,-1); => "ello World"
 */
void ustrrange(ustr_t s, int start, int end) {
  size_t newlen, len = ustrlen(s);

  if (len == 0)
    return;
  if (start < 0) {
    start = (int) (len + start);
    if (start < 0)
      start = 0;
  }
  if (end < 0) {
    end = (int) (len + end);
    if (end < 0)
      end = 0;
  }
  newlen = (size_t) ((start > end) ? 0 : (end - start) + 1);
  if (newlen != 0) {
    if (start >= (signed) len) {
      newlen = 0;
    } else if (end >= (signed) len) {
      end = (int) (len - 1);
      newlen = (size_t) ((start > end) ? 0 : (end - start) + 1);
    }
  } else {
    start = 0;
  }
  if (start && newlen)
    memmove(s, s + start, newlen);
  s[newlen] = 0;
  USTR_SET_LEN(s, newlen);
}

/* Apply tolower() to every character of the ustr_t ustr 's'. */
void ustrtolower(ustr_t s) {
  size_t len = ustrlen(s), j;

  for (j = 0; j < len; j++)
    s[j] = (char) tolower(s[j]);
}

/* Apply toupper() to every character of the ustr_t ustr 's'. */
void ustrtoupper(ustr_t s) {
  size_t len = ustrlen(s), j;

  for (j = 0; j < len; j++)
    s[j] = (char) toupper(s[j]);
}

/* Compare two ustr_t strings s1 and s2 with memcmp().
 *
 * Return value:
 *
 *     positive if s1 > s2.
 *     negative if s1 < s2.
 *     0 if s1 and s2 are exactly the same binary string.
 *
 * If two strings share exactly the same prefix, but one of the two has
 * additional characters, the longer ustr is considered to be greater than
 * the smaller one. */
int ustrcmp(const ustr_t s1, const ustr_t s2) {
  size_t l1, l2, minlen;
  int cmp;

  l1 = ustrlen(s1);
  l2 = ustrlen(s2);
  minlen = (l1 < l2) ? l1 : l2;
  cmp = memcmp(s1, s2, minlen);
  if (cmp == 0)
    return (int) (l1 - l2);
  return cmp;
}

/* Split 's' with separator in 'sep'. An array
 * of ustr_t strings is returned. *count will be set
 * by reference to the number of tokens returned.
 *
 * On out of memory, zero length string, zero length
 * separator, nullptr is returned.
 *
 * Note that 'sep' is able to split a ustr using
 * a multi-character separator. For example
 * ustrsplit("foo_-_bar","_-_"); will return two
 * elements "foo" and "bar".
 *
 * This version of the function is binary-safe but
 * requires length arguments. ustrsplit() is just the
 * same function but for zero-terminated strings.
 */
ustr_t *ustrsplitlen(const char *s, int len, const char *sep, int seplen, int *count) {
  int elements = 0, slots = 5, start = 0, j;
  ustr_t *tokens;

  if (seplen < 1 || len < 0)
    return nullptr;

  tokens = malloc(sizeof(ustr_t) * slots);
  if (tokens == nullptr)
    return nullptr;

  if (len == 0) {
    *count = 0;
    return tokens;
  }
  for (j = 0; j < (len - (seplen - 1)); j++) {
    /* make sure there is room for the next element and the final one */
    if (slots < elements + 2) {
      ustr_t *newtokens;

      slots *= 2;
      newtokens = realloc(tokens, sizeof(ustr_t) * slots);
      if (newtokens == nullptr)
        goto cleanup;
      tokens = newtokens;
    }
    /* search the separator */
    if ((seplen == 1 && *(s + j) == sep[0]) || (memcmp(s + j, sep, (size_t) seplen) == 0)) {
      tokens[elements] = ustrn(s + start, (size_t) (j - start));
      if (tokens[elements] == nullptr)
        goto cleanup;
      elements++;
      start = j + seplen;
      j = j + seplen - 1; /* skip the separator */
    }
  }
  /* Add the final element. We are sure there is room in the tokens array. */
  tokens[elements] = ustrn(s + start, (size_t) (len - start));
  if (tokens[elements] == nullptr)
    goto cleanup;
  elements++;
  *count = elements;
  return tokens;

    cleanup:
  {
    int i;
    for (i = 0; i < elements; i++)
      ustrfree(tokens[i]);
    free(tokens);
    *count = 0;
    return nullptr;
  }
}

/* Free the result returned by ustrsplitlen(), or do nothing if 'tokens' is nullptr. */
void ustrfreesplitres(ustr_t *tokens, int count) {
  if (!tokens)
    return;
  while (count--)
    ustrfree(tokens[count]);
  free(tokens);
}

/* Append to the ustr_t ustr "s" an escaped string representation where
 * all the non-printable characters (tested with isprint()) are turned into
 * escapes in the form "\n\r\a...." or "\x<hex-number>".
 *
 * After the call, the modified ustr_t string is no longer valid and all the
 * references must be substituted with the new pointer returned by the call. */
ustr_t ustrcatrepr(ustr_t s, const char *p, size_t len) {
  s = ustrncat(s, "\"", 1);
  while (len--) {
    switch (*p) {
      case '\\':
      case '"':
        s = ustrcatprintf(s, "\\%c", *p);
        break;
      case '\n':
        s = ustrncat(s, "\\n", 2);
        break;
      case '\r':
        s = ustrncat(s, "\\r", 2);
        break;
      case '\t':
        s = ustrncat(s, "\\t", 2);
        break;
      case '\a':
        s = ustrncat(s, "\\a", 2);
        break;
      case '\b':
        s = ustrncat(s, "\\b", 2);
        break;
      default:
        if (isprint(*p))
          s = ustrcatprintf(s, "%c", *p);
        else
          s = ustrcatprintf(s, "\\x%02x", (uint8_t) *p);
        break;
    }
    p++;
  }
  return ustrncat(s, "\"", 1);
}

/* Helper function for ustrsplitargs() that returns non zero if 'c'
 * is a valid hex digit. */
static int is_hex_digit(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
    (c >= 'A' && c <= 'F');
}

/* Helper function for ustrsplitargs() that converts a hex digit into an
 * integer from 0 to 15 */
static int hex_digit_to_int(char c) {
  switch (c) {
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case 'a':
    case 'A':
      return 10;
    case 'b':
    case 'B':
      return 11;
    case 'c':
    case 'C':
      return 12;
    case 'd':
    case 'D':
      return 13;
    case 'e':
    case 'E':
      return 14;
    case 'f':
    case 'F':
      return 15;
    default:
      return 0;
  }
}

/* Split a line into arguments, where every argument can be in the
 * following programming-language REPL-alike form:
 *
 * foo bar "newline are supported\n" and "\xff\x00otherstuff"
 *
 * The number of arguments is stored into *argc, and an array
 * of ustr_t is returned.
 *
 * The caller should free the resulting array of ustr_t strings with
 * ustrfreesplitres().
 *
 * Note that ustrcatrepr() is able to convert back a ustr into
 * a quoted string in the same format ustrsplitargs() is able to parse.
 *
 * The function returns the allocated tokens on success, even when the
 * input string is empty, or nullptr if the input contains unbalanced
 * quotes or closed quotes followed by non space characters
 * as in: "foo"bar or "foo'
 */
ustr_t *ustrsplitargs(const char *line, int *argc) {
  const char *p = line;
  char *current = nullptr;
  char **vector = nullptr;

  *argc = 0;
  while (1) {
    /* skip blanks */
    while (*p && isspace(*p))
      p++;
    if (*p) {
      /* get a token */
      int inq = 0;  /* set to 1 if we are in "quotes" */
      int insq = 0; /* set to 1 if we are in 'single quotes' */
      int done = 0;

      if (current == nullptr)
        current = ustrempty();
      while (!done) {
        if (inq) {
          if (*p == '\\' && *(p + 1) == 'x' &&
            is_hex_digit(*(p + 2)) &&
            is_hex_digit(*(p + 3))) {
            uint8_t byte;

            byte = (uint8_t) ((hex_digit_to_int(*(p + 2)) * 16) +
              hex_digit_to_int(*(p + 3)));
            current = ustrncat(current, (char *) &byte, 1);
            p += 3;
          } else if (*p == '\\' && *(p + 1)) {
            char c;

            p++;
            switch (*p) {
              case 'n':
                c = '\n';
                break;
              case 'r':
                c = '\r';
                break;
              case 't':
                c = '\t';
                break;
              case 'b':
                c = '\b';
                break;
              case 'a':
                c = '\a';
                break;
              default:
                c = *p;
                break;
            }
            current = ustrncat(current, &c, 1);
          } else if (*p == '"') {
            /* closing quote must be followed by a space or
             * nothing at all. */
            if (*(p + 1) && !isspace(*(p + 1)))
              goto err;
            done = 1;
          } else if (!*p) {
            /* unterminated quotes */
            goto err;
          } else {
            current = ustrncat(current, p, 1);
          }
        } else if (insq) {
          if (*p == '\\' && *(p + 1) == '\'') {
            p++;
            current = ustrncat(current, "'", 1);
          } else if (*p == '\'') {
            /* closing quote must be followed by a space or
             * nothing at all. */
            if (*(p + 1) && !isspace(*(p + 1)))
              goto err;
            done = 1;
          } else if (!*p) {
            /* unterminated quotes */
            goto err;
          } else {
            current = ustrncat(current, p, 1);
          }
        } else {
          switch (*p) {
            case ' ':
            case '\n':
            case '\r':
            case '\t':
            case '\0':
              done = 1;
              break;
            case '"':
              inq = 1;
              break;
            case '\'':
              insq = 1;
              break;
            default:
              current = ustrncat(current, p, 1);
              break;
          }
        }
        if (*p)
          p++;
      }
      /* add the token to the vector */
      vector = realloc(vector, ((*argc) + 1) * sizeof(char *));
      vector[*argc] = current;
      (*argc)++;
      current = nullptr;
    } else {
      /* Even on empty input ustr return something not nullptr. */
      if (vector == nullptr)
        vector = malloc(sizeof(void *));
      return vector;
    }
  }

    err:
  while ((*argc)--)
    ustrfree(vector[*argc]);
  free(vector);
  if (current)
    ustrfree(current);
  *argc = 0;
  return nullptr;
}

/* Modify the ustr substituting all the occurrences of the set of
 * characters specified in the 'from' string to the corresponding character
 * in the 'to' array.
 *
 * For instance: ustrmapchars(mystring, "ho", "01", 2)
 * will have the effect of turning the string "hello" into "0ell1".
 *
 * The function returns the ustr_t string pointer, that is always the same
 * as the input pointer since no resize is needed. */
ustr_t ustrmapchars(ustr_t s, const char *from, const char *to, size_t setlen) {
  size_t j, i, l = ustrlen(s);

  for (j = 0; j < l; j++) {
    for (i = 0; i < setlen; i++) {
      if (s[j] == from[i]) {
        s[j] = to[i];
        break;
      }
    }
  }
  return s;
}

/* Join an array of C strings using the specified separator (also a C string).
 * Returns the result as an ustr_t ustr. */
ustr_t ustrjoin(char **argv, int argc, char *sep) {
  ustr_t join = ustrempty();
  int j;

  for (j = 0; j < argc; j++) {
    join = ustrcat(join, argv[j]);
    if (j != argc - 1)
      join = ustrcat(join, sep);
  }
  return join;
}

/* Like ustrjoin, but joins an array of SDS strings. */
ustr_t ustrjoinsds(ustr_t *argv, int argc, const char *sep, size_t seplen) {
  ustr_t join = ustrempty();
  int j;

  for (j = 0; j < argc; j++) {
    join = ustrxcat(join, argv[j]);
    if (j != argc - 1)
      join = ustrncat(join, sep, seplen);
  }
  return join;
}
