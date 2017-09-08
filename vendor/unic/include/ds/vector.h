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

/*!@file ds/vector.h
 * @author uael
 */
#ifndef __DS_VECTOR_H
# define __DS_VECTOR_H

#include <stdlib.h>

#include "unic/types.h"
#include "unic/config.h"
#include "unic/math.h"
#include "unic/err.h"

#define DS_MIN_CAP 8

#define vecof(TItem, TSizeBits) struct { \
    u##TSizeBits##_t cap, len; \
    TItem *buf; \
  }

#define VEC_DEFINE_ALLOC(ID, TItem, TSizeBits, MALLOC_FN, REALLOC_FN, FREE_FN) \
  typedef vecof(TItem, TSizeBits) ID##_t; \
  static inline void \
  ID##_ctor(ID##_t *restrict self) { \
    *self = (ID##_t) {.cap = 0, .len = 0, .buf = nil}; \
  } \
  static inline void \
  ID##_dtor(ID##_t *restrict self) { \
    self->cap = 0; \
    self->len = 0; \
    if (self->buf) { \
      FREE_FN(self->buf); \
      self->buf = nil; \
    } \
  } \
  static inline err_t \
  ID##_growth(ID##_t *restrict self, const u##TSizeBits##_t nmin) { \
    if (nmin > 0) { \
      if (self->cap) { \
        if (self->cap < nmin) { \
          if (nmin == U##TSizeBits##_MAX || ISPOW2(nmin)) { \
            self->cap = nmin; \
          } else { \
            do self->cap *= 2; while (self->cap < nmin); \
          } \
          if ((self->buf = (TItem *) \
            REALLOC_FN(self->buf, sizeof(TItem) * (size_t) self->cap)) \
            == nil) { \
            self->cap = 0; \
            return (err_t) errno; \
          } \
        } \
      } else { \
        if (nmin == DS_MIN_CAP || (nmin > DS_MIN_CAP && \
          (nmin == U##TSizeBits##_MAX || ISPOW2(nmin)))) { \
          self->cap = nmin; \
        } else { \
          self->cap = DS_MIN_CAP; \
          while (self->cap < nmin) self->cap *= 2; \
        } \
        if ((self->buf = \
          (TItem *) MALLOC_FN(sizeof(TItem) * (size_t) self->cap)) == nil) { \
          self->cap = 0; \
          return (err_t) errno; \
        } \
      } \
    } \
    return SUCCESS; \
  } \
  static inline err_t \
  ID##_decay(ID##_t *restrict self, const u##TSizeBits##_t nmax) { \
    u##TSizeBits##_t nearest_pow2; \
    nearest_pow2 = pow2_next##TSizeBits(nmax); \
    if (self->cap > nearest_pow2) { \
      self->cap = nearest_pow2; \
      if ((self->buf = (TItem *) \
        REALLOC_FN(self->buf, sizeof(TItem) * (size_t) self->cap)) == nil) { \
        self->cap = 0; \
        return (err_t) errno; \
      } \
    } \
    if (self->len > nmax) { \
      memset((i8_t *) self->buf + nmax * sizeof(TItem), 0, \
        ((size_t) (self->len - nmax)) * sizeof(TItem) \
      ); \
      self->len = nmax; \
    } \
    return SUCCESS; \
  } \
  static inline err_t \
  ID##_grow(ID##_t *restrict self, const u##TSizeBits##_t nmem) { \
    u##TSizeBits##_t u; \
    u = self->len + nmem; \
    return ID##_growth(self, (const u##TSizeBits##_t) \
      (u < self->len ? U##TSizeBits##_MAX : u)); \
  } \
  static inline err_t \
  ID##_shrink(ID##_t *restrict self, const u##TSizeBits##_t nmem) { \
    return ID##_decay( \
      self, \
      (const u##TSizeBits##_t) (nmem >= self->len ? 0 : self->len - nmem) \
    ); \
  } \
  static inline err_t \
  ID##_trim(ID##_t *restrict self) { \
    u##TSizeBits##_t nearest_pow2; \
    nearest_pow2 = pow2_next##TSizeBits(self->len); \
    if (self->cap > nearest_pow2) { \
      self->cap = nearest_pow2; \
      if ((self->buf = (TItem *) \
        REALLOC_FN(self->buf, sizeof(TItem) * (size_t) self->cap)) == nil) { \
        return (err_t) errno; \
      } \
    } \
    return SUCCESS; \
  } \
  static inline err_t \
  ID##_push(ID##_t *restrict self, TItem v) { \
    err_t err; \
    if ((err = ID##_grow(self, 1)) > 0) { \
      return err; \
    } \
    self->buf[self->len++] = v; \
    return SUCCESS; \
  } \
  static inline err_t \
  ID##_pop(ID##_t *restrict self, TItem *restrict out) { \
    if (self->len <= 0) { \
      return FAILURE; \
    } \
    *out = self->buf[--self->len]; \
    return SUCCESS; \
  } \
  static inline err_t \
  ID##_resize(ID##_t *restrict self, const u##TSizeBits##_t n) { \
    err_t err; \
    if ((err = ID##_growth(self, n)) > 0) { \
      return err; \
    } \
    self->len = n; \
    return SUCCESS; \
  }

#define VEC_DEFINE(ID, TItem, TSizeBits) \
  VEC_DEFINE_ALLOC(ID, TItem, TSizeBits, malloc, realloc, free)

#define V8_DEFINE(ID, TItem) \
  VEC_DEFINE(ID, TItem, 8)

#define V16_DEFINE(ID, TItem) \
  VEC_DEFINE(ID, TItem, 16)

#define V32_DEFINE(ID, TItem) \
  VEC_DEFINE(ID, TItem, 32)

#define V64_DEFINE(ID, TItem) \
  VEC_DEFINE(ID, TItem, 64)

V8_DEFINE(vi8, i8_t);
V8_DEFINE(vu8, u8_t);
V16_DEFINE(vi16, i16_t);
V16_DEFINE(vu16, u16_t);
V32_DEFINE(vi32, i32_t);
V32_DEFINE(vu32, u32_t);
V64_DEFINE(vi64, i64_t);
V64_DEFINE(vu64, u64_t);
V8_DEFINE(v8, void *);
V16_DEFINE(v16, void *);
V32_DEFINE(v32, void *);
V64_DEFINE(v64, void *);

#endif /* !__DS_VECTOR_H */
