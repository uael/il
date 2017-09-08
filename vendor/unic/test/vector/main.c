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

#include <cute.h>
#include <ds/vector.h>

#include "ds/vector.h"

CUTEST_DATA {
  vi8_t vi8;
  vu8_t vu8;
  vi16_t vi16;
  vu16_t vu16;
  vi32_t vi32;
  vu32_t vu32;
  vi64_t vi64;
  vu64_t vu64;
};

CUTEST_SETUP {
  vi8_ctor(&self->vi8);
  vu8_ctor(&self->vu8);
  vi16_ctor(&self->vi16);
  vu16_ctor(&self->vu16);
  vi32_ctor(&self->vi32);
  vu32_ctor(&self->vu32);
  vi64_ctor(&self->vi64);
  vu64_ctor(&self->vu64);
}

CUTEST_TEARDOWN {
  vi8_dtor(&self->vi8);
  vu8_dtor(&self->vu8);
  vi16_dtor(&self->vi16);
  vu16_dtor(&self->vu16);
  vi32_dtor(&self->vi32);
  vu32_dtor(&self->vu32);
  vi64_dtor(&self->vi64);
  vu64_dtor(&self->vu64);
}

CUTEST(v8, growth) {
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, 0));
  ASSERT_EQ(0, self->vi8.cap);
  ASSERT_EQ(0, self->vi8.len);
  ASSERT_EQ(nil, self->vi8.buf);
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, 1));
  ASSERT_EQ(DS_MIN_CAP, self->vi8.cap);
  ASSERT_EQ(0, self->vi8.len);
  ASSERT_NEQ(nil, self->vi8.buf);
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, 1));
  ASSERT_EQ(DS_MIN_CAP, self->vi8.cap);
  ASSERT_EQ(0, self->vi8.len);
  ASSERT_NEQ(nil, self->vi8.buf);
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, 2));
  ASSERT_EQ(DS_MIN_CAP, self->vi8.cap);
  ASSERT_EQ(0, self->vi8.len);
  ASSERT_NEQ(nil, self->vi8.buf);
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, DS_MIN_CAP + 1));
  ASSERT_EQ(pow2_next8(DS_MIN_CAP + 1), self->vi8.cap);
  ASSERT_EQ(0, self->vi8.len);
  ASSERT_NEQ(nil, self->vi8.buf);
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, U8_MAX));
  ASSERT_EQ(U8_MAX, self->vi8.cap);
  ASSERT_EQ(0, self->vi8.len);
  ASSERT_NEQ(nil, self->vi8.buf);
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, U8_MAX));
  ASSERT_EQ(U8_MAX, self->vi8.cap);
  ASSERT_EQ(0, self->vi8.len);
  ASSERT_NEQ(nil, self->vi8.buf);
  vi8_dtor(&self->vi8);
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, DS_MIN_CAP));
  ASSERT_EQ(DS_MIN_CAP, self->vi8.cap);
  ASSERT_EQ(0, self->vi8.len);
  ASSERT_NEQ(nil, self->vi8.buf);
  vi8_dtor(&self->vi8);
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, U8_MAX));
  ASSERT_EQ(U8_MAX, self->vi8.cap);
  ASSERT_EQ(0, self->vi8.len);
  ASSERT_NEQ(nil, self->vi8.buf);
  vi8_dtor(&self->vi8);
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, pow2_next8(DS_MIN_CAP + 1)));
  ASSERT_EQ(pow2_next8(DS_MIN_CAP + 1), self->vi8.cap);
  ASSERT_EQ(0, self->vi8.len);
  ASSERT_NEQ(nil, self->vi8.buf);
  return CUTE_SUCCESS;
}

CUTEST(v16, growth) {
  ASSERT_EQ(SUCCESS, vi16_growth(&self->vi16, 0));
  ASSERT_EQ(0, self->vi16.cap);
  ASSERT_EQ(0, self->vi16.len);
  ASSERT_EQ(nil, self->vi16.buf);
  ASSERT_EQ(SUCCESS, vi16_growth(&self->vi16, 1));
  ASSERT_EQ(DS_MIN_CAP, self->vi16.cap);
  ASSERT_EQ(0, self->vi16.len);
  ASSERT_NEQ(nil, self->vi16.buf);
  ASSERT_EQ(SUCCESS, vi16_growth(&self->vi16, 1));
  ASSERT_EQ(DS_MIN_CAP, self->vi16.cap);
  ASSERT_EQ(0, self->vi16.len);
  ASSERT_NEQ(nil, self->vi16.buf);
  ASSERT_EQ(SUCCESS, vi16_growth(&self->vi16, 2));
  ASSERT_EQ(DS_MIN_CAP, self->vi16.cap);
  ASSERT_EQ(0, self->vi16.len);
  ASSERT_NEQ(nil, self->vi16.buf);
  ASSERT_EQ(SUCCESS, vi16_growth(&self->vi16, DS_MIN_CAP + 1));
  ASSERT_EQ(pow2_next16(DS_MIN_CAP + 1), self->vi16.cap);
  ASSERT_EQ(0, self->vi16.len);
  ASSERT_NEQ(nil, self->vi16.buf);
  ASSERT_EQ(SUCCESS, vi16_growth(&self->vi16, U16_MAX));
  ASSERT_EQ(U16_MAX, self->vi16.cap);
  ASSERT_EQ(0, self->vi16.len);
  ASSERT_NEQ(nil, self->vi16.buf);
  ASSERT_EQ(SUCCESS, vi16_growth(&self->vi16, U16_MAX));
  ASSERT_EQ(U16_MAX, self->vi16.cap);
  ASSERT_EQ(0, self->vi16.len);
  ASSERT_NEQ(nil, self->vi16.buf);
  vi16_dtor(&self->vi16);
  ASSERT_EQ(SUCCESS, vi16_growth(&self->vi16, DS_MIN_CAP));
  ASSERT_EQ(DS_MIN_CAP, self->vi16.cap);
  ASSERT_EQ(0, self->vi16.len);
  ASSERT_NEQ(nil, self->vi16.buf);
  vi16_dtor(&self->vi16);
  ASSERT_EQ(SUCCESS, vi16_growth(&self->vi16, U16_MAX));
  ASSERT_EQ(U16_MAX, self->vi16.cap);
  ASSERT_EQ(0, self->vi16.len);
  ASSERT_NEQ(nil, self->vi16.buf);
  vi16_dtor(&self->vi16);
  ASSERT_EQ(SUCCESS, vi16_growth(&self->vi16, pow2_next16(DS_MIN_CAP + 1)));
  ASSERT_EQ(pow2_next16(DS_MIN_CAP + 1), self->vi16.cap);
  ASSERT_EQ(0, self->vi16.len);
  ASSERT_NEQ(nil, self->vi16.buf);
  return CUTE_SUCCESS;
}

CUTEST(v32, growth) {
  ASSERT_EQ(SUCCESS, vi32_growth(&self->vi32, 0));
  ASSERT_EQ(0, self->vi32.cap);
  ASSERT_EQ(0, self->vi32.len);
  ASSERT_EQ(nil, self->vi32.buf);
  ASSERT_EQ(SUCCESS, vi32_growth(&self->vi32, 1));
  ASSERT_EQ(DS_MIN_CAP, self->vi32.cap);
  ASSERT_EQ(0, self->vi32.len);
  ASSERT_NEQ(nil, self->vi32.buf);
  ASSERT_EQ(SUCCESS, vi32_growth(&self->vi32, 1));
  ASSERT_EQ(DS_MIN_CAP, self->vi32.cap);
  ASSERT_EQ(0, self->vi32.len);
  ASSERT_NEQ(nil, self->vi32.buf);
  ASSERT_EQ(SUCCESS, vi32_growth(&self->vi32, 2));
  ASSERT_EQ(DS_MIN_CAP, self->vi32.cap);
  ASSERT_EQ(0, self->vi32.len);
  ASSERT_NEQ(nil, self->vi32.buf);
  ASSERT_EQ(SUCCESS, vi32_growth(&self->vi32, DS_MIN_CAP + 1));
  ASSERT_EQ(pow2_next32(DS_MIN_CAP + 1), self->vi32.cap);
  ASSERT_EQ(0, self->vi32.len);
  ASSERT_NEQ(nil, self->vi32.buf);
  ASSERT_EQ(SUCCESS, vi32_growth(&self->vi32, U32_MAX));
  ASSERT_EQ(U32_MAX, self->vi32.cap);
  ASSERT_EQ(0, self->vi32.len);
  ASSERT_NEQ(nil, self->vi32.buf);
  ASSERT_EQ(SUCCESS, vi32_growth(&self->vi32, U32_MAX));
  ASSERT_EQ(U32_MAX, self->vi32.cap);
  ASSERT_EQ(0, self->vi32.len);
  ASSERT_NEQ(nil, self->vi32.buf);
  vi32_dtor(&self->vi32);
  ASSERT_EQ(SUCCESS, vi32_growth(&self->vi32, DS_MIN_CAP));
  ASSERT_EQ(DS_MIN_CAP, self->vi32.cap);
  ASSERT_EQ(0, self->vi32.len);
  ASSERT_NEQ(nil, self->vi32.buf);
  vi32_dtor(&self->vi32);
  ASSERT_EQ(SUCCESS, vi32_growth(&self->vi32, U32_MAX));
  ASSERT_EQ(U32_MAX, self->vi32.cap);
  ASSERT_EQ(0, self->vi32.len);
  ASSERT_NEQ(nil, self->vi32.buf);
  vi32_dtor(&self->vi32);
  ASSERT_EQ(SUCCESS, vi32_growth(&self->vi32, pow2_next32(DS_MIN_CAP + 1)));
  ASSERT_EQ(pow2_next32(DS_MIN_CAP + 1), self->vi32.cap);
  ASSERT_EQ(0, self->vi32.len);
  ASSERT_NEQ(nil, self->vi32.buf);
  return CUTE_SUCCESS;
}

CUTEST(v64, growth) {
  ASSERT_EQ(SUCCESS, vi64_growth(&self->vi64, 0));
  ASSERT_EQ(0, self->vi64.cap);
  ASSERT_EQ(0, self->vi64.len);
  ASSERT_EQ(nil, self->vi64.buf);
  ASSERT_EQ(SUCCESS, vi64_growth(&self->vi64, 1));
  ASSERT_EQ(DS_MIN_CAP, self->vi64.cap);
  ASSERT_EQ(0, self->vi64.len);
  ASSERT_NEQ(nil, self->vi64.buf);
  ASSERT_EQ(SUCCESS, vi64_growth(&self->vi64, 1));
  ASSERT_EQ(DS_MIN_CAP, self->vi64.cap);
  ASSERT_EQ(0, self->vi64.len);
  ASSERT_NEQ(nil, self->vi64.buf);
  ASSERT_EQ(SUCCESS, vi64_growth(&self->vi64, 2));
  ASSERT_EQ(DS_MIN_CAP, self->vi64.cap);
  ASSERT_EQ(0, self->vi64.len);
  ASSERT_NEQ(nil, self->vi64.buf);
  ASSERT_EQ(SUCCESS, vi64_growth(&self->vi64, DS_MIN_CAP + 1));
  ASSERT_EQ(pow2_next64(DS_MIN_CAP + 1), self->vi64.cap);
  ASSERT_EQ(0, self->vi64.len);
  ASSERT_NEQ(nil, self->vi64.buf);
  ASSERT_EQ(ERR_NOMEM, vi64_growth(&self->vi64, U64_MAX));
  ASSERT_EQ(0, self->vi64.cap);
  ASSERT_EQ(0, self->vi64.len);
  ASSERT_EQ(nil, self->vi64.buf);
  ASSERT_EQ(ERR_NOMEM, vi64_growth(&self->vi64, U64_MAX));
  ASSERT_EQ(0, self->vi64.cap);
  ASSERT_EQ(0, self->vi64.len);
  ASSERT_EQ(nil, self->vi64.buf);
  vi64_dtor(&self->vi64);
  ASSERT_EQ(SUCCESS, vi64_growth(&self->vi64, DS_MIN_CAP));
  ASSERT_EQ(DS_MIN_CAP, self->vi64.cap);
  ASSERT_EQ(0, self->vi64.len);
  ASSERT_NEQ(nil, self->vi64.buf);
  vi64_dtor(&self->vi64);
  ASSERT_EQ(ERR_NOMEM, vi64_growth(&self->vi64, U64_MAX));
  ASSERT_EQ(0, self->vi64.cap);
  ASSERT_EQ(0, self->vi64.len);
  ASSERT_EQ(nil, self->vi64.buf);
  vi64_dtor(&self->vi64);
  ASSERT_EQ(SUCCESS, vi64_growth(&self->vi64, pow2_next64(DS_MIN_CAP + 1)));
  ASSERT_EQ(pow2_next64(DS_MIN_CAP + 1), self->vi64.cap);
  ASSERT_EQ(0, self->vi64.len);
  ASSERT_NEQ(nil, self->vi64.buf);
  return CUTE_SUCCESS;
}

i32_t main(void) {
  CUTEST_DATA test;

  CUTEST_PASS(v8, growth);
  CUTEST_PASS(v16, growth);
  CUTEST_PASS(v32, growth);
  CUTEST_PASS(v64, growth);
  return EXIT_SUCCESS;
}
