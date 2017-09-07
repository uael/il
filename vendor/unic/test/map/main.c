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

#include "data.h"

CUTEST_DATA {
  test_i8_map_t i8_map;
  test_u8_map_t u8_map;
  test_i16_map_t i16_map;
  test_u16_map_t u16_map;
  test_i32_map_t i32_map;
  test_u32_map_t u32_map;
  test_i64_map_t i64_map;
  test_u64_map_t u64_map;
  test_str_map_t str_map;
};

CUTEST_SETUP {
  test_i8_map_ctor(&self->i8_map);
  test_u8_map_ctor(&self->u8_map);
  test_i16_map_ctor(&self->i16_map);
  test_u16_map_ctor(&self->u16_map);
  test_i32_map_ctor(&self->i32_map);
  test_u32_map_ctor(&self->u32_map);
  test_i64_map_ctor(&self->i64_map);
  test_u64_map_ctor(&self->u64_map);
  test_str_map_ctor(&self->str_map);
}

CUTEST_TEARDOWN {
  test_i8_map_ctor(&self->i8_map);
  test_u8_map_ctor(&self->u8_map);
  test_i16_map_dtor(&self->i16_map);
  test_u16_map_dtor(&self->u16_map);
  test_i32_map_dtor(&self->i32_map);
  test_u32_map_dtor(&self->u32_map);
  test_i64_map_dtor(&self->i64_map);
  test_u64_map_dtor(&self->u64_map);
  test_str_map_dtor(&self->str_map);
}

CUTEST(map, i8) {
  u32_t out;
  i8_t i;

  for (i = 0; i < I8_MAX; ++i) {
    switch (test_i8_map_put(&self->i8_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        break;
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        FAIL("populated");
    }
    switch (test_i8_map_put(&self->i8_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        FAIL("must be populated");
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->i8_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i8_map.cap);
  for (i = 0; i < I8_MAX; ++i) {
    ASSERT(test_i8_map_get(&self->i8_map, i, &out));
    ASSERT(test_i8_map_del(&self->i8_map, out));
  }
  ASSERT_EQ(0, self->i8_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i8_map.cap);
  test_i8_map_clear(&self->i8_map);
  return CUTE_SUCCESS;
}

CUTEST(map, u8) {
  u32_t out;
  u8_t i;

  for (i = 0; i < U8_MAX; ++i) {
    switch (test_u8_map_put(&self->u8_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        break;
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        FAIL("populated");
    }
    switch (test_u8_map_put(&self->u8_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        FAIL("must be populated");
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->u8_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u8_map.cap);
  for (i = 0; i < U8_MAX; ++i) {
    ASSERT(test_u8_map_get(&self->u8_map, i, &out));
    ASSERT(test_u8_map_del(&self->u8_map, out));
  }
  ASSERT_EQ(0, self->u8_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u8_map.cap);
  test_u8_map_clear(&self->u8_map);
  return CUTE_SUCCESS;
}

CUTEST(map, i16) {
  u32_t out;
  i16_t i;

  for (i = 0; i < I16_MAX; ++i) {
    switch (test_i16_map_put(&self->i16_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        break;
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        FAIL("populated");
    }
    switch (test_i16_map_put(&self->i16_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        FAIL("must be populated");
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->i16_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i16_map.cap);
  for (i = 0; i < I16_MAX; ++i) {
    ASSERT(test_i16_map_get(&self->i16_map, i, &out));
    ASSERT(test_i16_map_del(&self->i16_map, out));
  }
  ASSERT_EQ(0, self->i16_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i16_map.cap);
  test_i16_map_clear(&self->i16_map);
  return CUTE_SUCCESS;
}

CUTEST(map, u16) {
  u32_t out;
  u16_t i;

  for (i = 0; i < U16_MAX; ++i) {
    switch (test_u16_map_put(&self->u16_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        break;
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        FAIL("populated");
    }
    switch (test_u16_map_put(&self->u16_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        FAIL("must be populated");
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->u16_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u16_map.cap);
  for (i = 0; i < U16_MAX; ++i) {
    ASSERT(test_u16_map_get(&self->u16_map, i, &out));
    ASSERT(test_u16_map_del(&self->u16_map, out));
  }
  ASSERT_EQ(0, self->u16_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u16_map.cap);
  test_u16_map_clear(&self->u16_map);
  return CUTE_SUCCESS;
}

CUTEST(map, i32) {
  u32_t out;
  i32_t i;

  for (i = 0; i < U16_MAX; ++i) {
    switch (test_i32_map_put(&self->i32_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        break;
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        FAIL("populated");
    }
    switch (test_i32_map_put(&self->i32_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        FAIL("must be populated");
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->i32_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i32_map.cap);
  for (i = 0; i < U16_MAX; ++i) {
    ASSERT(test_i32_map_get(&self->i32_map, i, &out));
    ASSERT(test_i32_map_del(&self->i32_map, out));
  }
  ASSERT_EQ(0, self->i32_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->i32_map.cap);
  test_i32_map_clear(&self->i32_map);
  return CUTE_SUCCESS;
}

CUTEST(map, u32) {
  u32_t out;
  u32_t i;

  for (i = 0; i < U16_MAX; ++i) {
    switch (test_u32_map_put(&self->u32_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        break;
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        FAIL("populated");
    }
    switch (test_u32_map_put(&self->u32_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        FAIL("must be populated");
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->u32_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u32_map.cap);
  for (i = 0; i < U16_MAX; ++i) {
    ASSERT(test_u32_map_get(&self->u32_map, i, &out));
    ASSERT(test_u32_map_del(&self->u32_map, out));
  }
  ASSERT_EQ(0, self->u32_map.len);
  ASSERT_EQ(pow2_next32(i + 2), self->u32_map.cap);
  test_u32_map_clear(&self->u32_map);
  return CUTE_SUCCESS;
}

CUTEST(map, i64) {
  u32_t out;
  i64_t i;

  for (i = 0; i < U16_MAX; ++i) {
    switch (test_i64_map_put(&self->i64_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        break;
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        FAIL("populated");
    }
    switch (test_i64_map_put(&self->i64_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        FAIL("must be populated");
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->i64_map.len);
  ASSERT_EQ(pow2_next32((u32_t) i + 2), self->i64_map.cap);
  for (i = 0; i < U16_MAX; ++i) {
    ASSERT(test_i64_map_get(&self->i64_map, i, &out));
    ASSERT(test_i64_map_del(&self->i64_map, out));
  }
  ASSERT_EQ(0, self->i64_map.len);
  ASSERT_EQ(pow2_next32((u32_t) i + 2), self->i64_map.cap);
  test_i64_map_clear(&self->i64_map);
  return CUTE_SUCCESS;
}

CUTEST(map, u64) {
  u32_t out;
  u64_t i;

  for (i = 0; i < U16_MAX; ++i) {
    switch (test_u64_map_put(&self->u64_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        break;
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        FAIL("populated");
    }
    switch (test_u64_map_put(&self->u64_map, i, &out)) {
      case MAP_PUT_SUCCESS:
        FAIL("must be populated");
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        break;
    }
  }
  ASSERT_EQ(i, self->u64_map.len);
  ASSERT_EQ(pow2_next32((u32_t) i + 2), self->u64_map.cap);
  for (i = 0; i < U16_MAX; ++i) {
    ASSERT(test_u64_map_get(&self->u64_map, i, &out));
    ASSERT(test_u64_map_del(&self->u64_map, out));
  }
  ASSERT_EQ(0, self->u64_map.len);
  ASSERT_EQ(pow2_next32((u32_t) i + 2), self->u64_map.cap);
  test_u64_map_clear(&self->u64_map);
  return CUTE_SUCCESS;
}

CUTEST(map, str) {
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
    switch (test_str_map_put(&self->str_map, *str, &out)) {
      case MAP_PUT_SUCCESS:
        break;
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        FAIL("populated");
    }
    switch (test_str_map_put(&self->str_map, *str, &out)) {
      case MAP_PUT_SUCCESS:
        FAIL("must be populated");
      case MAP_PUT_ALLOC_FAILURE:
        FAIL("allocation failure");
      case MAP_PUT_DELETED:
        FAIL("deleted");
      case MAP_PUT_POPULATED:
        break;
    }
    ++str;
  }
  ASSERT_EQ(50, self->str_map.len);
  str = strs;
  while (*str) {
    ASSERT(test_str_map_get(&self->str_map, *str, &out));
    ASSERT(test_str_map_del(&self->str_map, out));
    ++str;
  }
  ASSERT_EQ(0, self->u64_map.len);
  test_str_map_clear(&self->str_map);
  return CUTE_SUCCESS;
}

i32_t main(void) {
  CUTEST_DATA test;

  CUTEST_PASS(map, i8);
  CUTEST_PASS(map, u8);
  CUTEST_PASS(map, i16);
  CUTEST_PASS(map, u16);
  CUTEST_PASS(map, i32);
  CUTEST_PASS(map, u32);
  CUTEST_PASS(map, i64);
  CUTEST_PASS(map, u64);
  CUTEST_PASS(map, str);
  return EXIT_SUCCESS;
}
