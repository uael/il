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

/*!@file map/data.h
 * @author uael
 */
#ifndef __TEST_MAP_DATA_H
# define __TEST_MAP_DATA_H

#include "ds/map.h"

MAP_DECLARE(extern, test_i8, i8_t, u8_t);
MAP_DECLARE(extern, test_u8, u8_t, u8_t);
MAP_DECLARE(extern, test_i16, i16_t, u16_t);
MAP_DECLARE(extern, test_u16, u16_t, u16_t);
MAP_DECLARE(extern, test_i32, i32_t, u32_t);
MAP_DECLARE(extern, test_u32, u32_t, u32_t);
MAP_DECLARE(extern, test_i64, i64_t, u32_t);
MAP_DECLARE(extern, test_u64, u64_t, u32_t);
MAP_DECLARE(extern, test_str, const i8_t *, u32_t);

#endif /* !__TEST_MAP_DATA_H */
