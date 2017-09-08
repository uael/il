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

CUTEST(vector, general) {
  u8_t i;

  ASSERT_EQ(SUCCESS, vi8_push(&self->vi8, -5));
  ASSERT_EQ(SUCCESS, vi8_push(&self->vi8, -4));
  ASSERT_EQ(SUCCESS, vi8_push(&self->vi8, -3));
  ASSERT_EQ(SUCCESS, vi8_push(&self->vi8, -2));
  ASSERT_EQ(SUCCESS, vi8_push(&self->vi8, -1));
  ASSERT_EQ(SUCCESS, vi8_push(&self->vi8, 0));
  ASSERT_EQ(SUCCESS, vi8_trim(&self->vi8));
  ASSERT_EQ(SUCCESS, vi8_shrink(&self->vi8, 3));
  ASSERT_EQ(SUCCESS, vi8_grow(&self->vi8, 15));
  ASSERT_EQ(SUCCESS, vi8_resize(&self->vi8, 1));
  ASSERT_EQ(SUCCESS, vi8_trim(&self->vi8));
  ASSERT_EQ(SUCCESS, vi8_growth(&self->vi8, U8_MAX));
  ASSERT_EQ(SUCCESS, vi8_grow(&self->vi8, U8_MAX));
  return CUTE_SUCCESS;
}

i32_t main(void) {
  CUTEST_DATA test;

  CUTEST_PASS(vector, general);
  return EXIT_SUCCESS;
}
