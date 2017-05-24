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

#ifndef   WULK_ATTR_H__
# define  WULK_ATTR_H__

/**
 * @def WULK_NORETURN
 * Attribute to mark a function which never returns.
 */
#if defined(__GNUC__) && __GNUC__ >= 3 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 70)
# define WULK_NORETURN __attribute__((__noreturn__)) void
#elif defined(__STDC__) && (__STDC_VERSION__ >= 201112L)
# define WULK_NORETURN _Noreturn void
#elif defined(_MSC_VER)
# define WULK_NORETURN void __declspec(noreturn)
#else
# define WULK_NORETURN void
#endif

#endif /* WULK_ATTR_H__ */
