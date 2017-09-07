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

#include "ds/set.h"

CUTEST_DATA {
  i8_set_t i8_set;
  u8_set_t u8_set;
  i16_set_t i16_set;
  u16_set_t u16_set;
  i32_set_t i32_set;
  u32_set_t u32_set;
  i64_set_t i64_set;
  u64_set_t u64_set;
  str_set_t str_set;
};

CUTEST_SETUP {
  i8_set_ctor(&self->i8_set);
  u8_set_ctor(&self->u8_set);
  i16_set_ctor(&self->i16_set);
  u16_set_ctor(&self->u16_set);
  i32_set_ctor(&self->i32_set);
  u32_set_ctor(&self->u32_set);
  i64_set_ctor(&self->i64_set);
  u64_set_ctor(&self->u64_set);
  str_set_ctor(&self->str_set);
}

CUTEST_TEARDOWN {
  i8_set_ctor(&self->i8_set);
  u8_set_ctor(&self->u8_set);
  i16_set_dtor(&self->i16_set);
  u16_set_dtor(&self->u16_set);
  i32_set_dtor(&self->i32_set);
  u32_set_dtor(&self->u32_set);
  i64_set_dtor(&self->i64_set);
  u64_set_dtor(&self->u64_set);
  str_set_dtor(&self->str_set);
}

CUTEST(set, i8) {
  u32_t out;
  i8_t i;

  for (i = 0; i < I8_MAX; ++i) {
    switch (i8_set_put(&self->i8_set, i, &out)) {
      case SET_PUT_SUCCESS:
        break;
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        FAIL("populated");
    }
    switch (i8_set_put(&self->i8_set, i, &out)) {
      case SET_PUT_SUCCESS:
        FAIL("must be populated");
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->i8_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i8_set.cap);
  for (i = 0; i < I8_MAX; ++i) {
    ASSERT(i8_set_get(&self->i8_set, i, &out));
    ASSERT(i8_set_del(&self->i8_set, out));
  }
  ASSERT_EQ(0, self->i8_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i8_set.cap);
  i8_set_clear(&self->i8_set);
  return CUTE_SUCCESS;
}

CUTEST(set, u8) {
  u32_t out;
  u8_t i;

  for (i = 0; i < U8_MAX; ++i) {
    switch (u8_set_put(&self->u8_set, i, &out)) {
      case SET_PUT_SUCCESS:
        break;
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        FAIL("populated");
    }
    switch (u8_set_put(&self->u8_set, i, &out)) {
      case SET_PUT_SUCCESS:
        FAIL("must be populated");
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->u8_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u8_set.cap);
  for (i = 0; i < U8_MAX; ++i) {
    ASSERT(u8_set_get(&self->u8_set, i, &out));
    ASSERT(u8_set_del(&self->u8_set, out));
  }
  ASSERT_EQ(0, self->u8_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u8_set.cap);
  u8_set_clear(&self->u8_set);
  return CUTE_SUCCESS;
}

CUTEST(set, i16) {
  u32_t out;
  i16_t i;

  for (i = 0; i < I16_MAX; ++i) {
    switch (i16_set_put(&self->i16_set, i, &out)) {
      case SET_PUT_SUCCESS:
        break;
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        FAIL("populated");
    }
    switch (i16_set_put(&self->i16_set, i, &out)) {
      case SET_PUT_SUCCESS:
        FAIL("must be populated");
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->i16_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i16_set.cap);
  for (i = 0; i < I16_MAX; ++i) {
    ASSERT(i16_set_get(&self->i16_set, i, &out));
    ASSERT(i16_set_del(&self->i16_set, out));
  }
  ASSERT_EQ(0, self->i16_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i16_set.cap);
  i16_set_clear(&self->i16_set);
  return CUTE_SUCCESS;
}

CUTEST(set, u16) {
  u32_t out;
  u16_t i;

  for (i = 0; i < U16_MAX; ++i) {
    switch (u16_set_put(&self->u16_set, i, &out)) {
      case SET_PUT_SUCCESS:
        break;
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        FAIL("populated");
    }
    switch (u16_set_put(&self->u16_set, i, &out)) {
      case SET_PUT_SUCCESS:
        FAIL("must be populated");
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->u16_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u16_set.cap);
  for (i = 0; i < U16_MAX; ++i) {
    ASSERT(u16_set_get(&self->u16_set, i, &out));
    ASSERT(u16_set_del(&self->u16_set, out));
  }
  ASSERT_EQ(0, self->u16_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u16_set.cap);
  u16_set_clear(&self->u16_set);
  return CUTE_SUCCESS;
}

CUTEST(set, i32) {
  u32_t out;
  i32_t i;

  for (i = 0; i < U16_MAX; ++i) {
    switch (i32_set_put(&self->i32_set, i, &out)) {
      case SET_PUT_SUCCESS:
        break;
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        FAIL("populated");
    }
    switch (i32_set_put(&self->i32_set, i, &out)) {
      case SET_PUT_SUCCESS:
        FAIL("must be populated");
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->i32_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i32_set.cap);
  for (i = 0; i < U16_MAX; ++i) {
    ASSERT(i32_set_get(&self->i32_set, i, &out));
    ASSERT(i32_set_del(&self->i32_set, out));
  }
  ASSERT_EQ(0, self->i32_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i32_set.cap);
  i32_set_clear(&self->i32_set);
  return CUTE_SUCCESS;
}

CUTEST(set, u32) {
  u32_t out;
  u32_t i;

  for (i = 0; i < U16_MAX; ++i) {
    switch (u32_set_put(&self->u32_set, i, &out)) {
      case SET_PUT_SUCCESS:
        break;
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        FAIL("populated");
    }
    switch (u32_set_put(&self->u32_set, i, &out)) {
      case SET_PUT_SUCCESS:
        FAIL("must be populated");
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->u32_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u32_set.cap);
  for (i = 0; i < U16_MAX; ++i) {
    ASSERT(u32_set_get(&self->u32_set, i, &out));
    ASSERT(u32_set_del(&self->u32_set, out));
  }
  ASSERT_EQ(0, self->u32_set.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u32_set.cap);
  u32_set_clear(&self->u32_set);
  return CUTE_SUCCESS;
}

CUTEST(set, i64) {
  u32_t out;
  i64_t i;

  for (i = 0; i < U16_MAX; ++i) {
    switch (i64_set_put(&self->i64_set, i, &out)) {
      case SET_PUT_SUCCESS:
        break;
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        FAIL("populated");
    }
    switch (i64_set_put(&self->i64_set, i, &out)) {
      case SET_PUT_SUCCESS:
        FAIL("must be populated");
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->i64_set.len);
  ASSERT_EQ(pow2_next32((u32_t) i + 2), self->i64_set.cap);
  for (i = 0; i < U16_MAX; ++i) {
    ASSERT(i64_set_get(&self->i64_set, i, &out));
    ASSERT(i64_set_del(&self->i64_set, out));
  }
  ASSERT_EQ(0, self->i64_set.len);
  ASSERT_EQ(pow2_next32((u32_t) i + 2), self->i64_set.cap);
  i64_set_clear(&self->i64_set);
  return CUTE_SUCCESS;
}

CUTEST(set, u64) {
  u32_t out;
  u64_t i;

  for (i = 0; i < U16_MAX; ++i) {
    switch (u64_set_put(&self->u64_set, i, &out)) {
      case SET_PUT_SUCCESS:
        break;
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        FAIL("populated");
    }
    switch (u64_set_put(&self->u64_set, i, &out)) {
      case SET_PUT_SUCCESS:
        FAIL("must be populated");
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->u64_set.len);
  ASSERT_EQ(pow2_next32((u32_t) i + 2), self->u64_set.cap);
  for (i = 0; i < U16_MAX; ++i) {
    ASSERT(u64_set_get(&self->u64_set, i, &out));
    ASSERT(u64_set_del(&self->u64_set, out));
  }
  ASSERT_EQ(0, self->u64_set.len);
  ASSERT_EQ(pow2_next32((u32_t) i + 2), self->u64_set.cap);
  u64_set_clear(&self->u64_set);
  return CUTE_SUCCESS;
}

CUTEST(set, str) {
  u32_t out;
  const i8_t **str;
  const i8_t *strs[51] = {
    "2lsDCacn6PYP7XbIWBDj",
    "rUPoh4tmafEbIxevHCSk",
    "VKaP4ZFpkOsedOYzvbQr",
    "KD30gGmnSwTNCOCUsHPG",
    "iLgdLRLMjAWUebIGUIRB",
    "bzQRVJaW7Pbdaa0uXN7F",
    "bDGPXbKV1eORSnrUcKpY",
    "nnvxJ5caqTswn67TUR2n",
    "L8rrOQ176El3CdJPy5Xq",
    "OtKDpfKFuN2oRG239KvK",
    "chvuzpvz9tyV8wwPPu9X",
    "Uy3cvIHR16KJWqPL2uNr",
    "Lbj4KPMwMJJgIsfyCJAt",
    "UcOc2zpGTZKthmoxAr1z",
    "ADTQo6o3axlrGRlK08aX",
    "iBVeBysuO0g65PSa2Z9v",
    "IIi7c7TL5NMZYJqXAYc3",
    "dtjIH6Us5snbvkotQRVE",
    "Ch2SevYxj7MMtfD0C7cY",
    "CIwtnvrKqbwGWrjeIJI0",
    "aykAPp72oqjOb5Qu6e6E",
    "7s85g9xBseiKRAm5xfUe",
    "HligRMrPQBCsv12ejGsJ",
    "uEFkV2Mz6FtscTaQpYEl",
    "HjcpWpxj6nMPnN6ut9Me",
    "1j9QI5omSEXcTlj7XCot",
    "C4TAFjU1ZySISDOtOmlG",
    "R7yaBBOOO8eeiLLGykJz",
    "6ycVA2aTYzDIPo7KYAkG",
    "mHD0ZGm9iSYF4jbeyWKy",
    "4CgUKokOaW87x6tymFjv",
    "Zpjyu8izAmpfCnM2bQar",
    "5GtFnjgSr7jcVcp5GTbt",
    "qPfra6DHtSldxer95dax",
    "YLBqp8nhM5WlIf5jzjzL",
    "2YHlBUCemE0lCCdroaDL",
    "ZsK7NTxtmewsJn7gRdov",
    "LqPefw767IQGWVriw1zt",
    "zfPP10z2ldi2WaM66A94",
    "d6J9xts1Rh16UCnTLiKC",
    "cTRi56evRxZDQOXmPefY",
    "lRCyN67Fmr0FShxeFdL5",
    "V6gmc0S2IK8KYVgwXR8t",
    "w70GkoKHjSF67ETpRoWR",
    "GjL81xpZVwo0s4RtuVGU",
    "tO8aGuNGVhxUnRjNc17I",
    "QUqdpKDmR7pawzqWBd3i",
    "DQC6ynjuqO97RIKVIEVc",
    "ikJah1IgglY7Fz2F9t8A",
    "NCUZIpgWsWU4w9u80KUq",
    nil
  };

  str = strs;
  while (*str) {
    switch (str_set_put(&self->str_set, *str, &out)) {
      case SET_PUT_SUCCESS:
        break;
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        FAIL("populated");
    }
    switch (str_set_put(&self->str_set, *str, &out)) {
      case SET_PUT_SUCCESS:
        FAIL("must be populated");
      case SET_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case SET_PUT_DELETED:
        FAIL("deleted");
      case SET_PUT_POPULATED:
        break;
    }
    ++str;
  }
  ASSERT_EQ(50, self->str_set.len);
  str = strs;
  while (*str) {
    ASSERT(str_set_get(&self->str_set, *str, &out));
    ASSERT(str_set_del(&self->str_set, out));
    ++str;
  }
  ASSERT_EQ(0, self->u64_set.len);
  str_set_clear(&self->str_set);
  return CUTE_SUCCESS;
}

i32_t main(void) {
  CUTEST_DATA test;

  CUTEST_PASS(set, i8);
  CUTEST_PASS(set, u8);
  CUTEST_PASS(set, i16);
  CUTEST_PASS(set, u16);
  CUTEST_PASS(set, i32);
  CUTEST_PASS(set, u32);
  CUTEST_PASS(set, i64);
  CUTEST_PASS(set, u64);
  CUTEST_PASS(set, str);
  return EXIT_SUCCESS;
}
