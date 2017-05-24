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

#ifndef   WULK_XMALLOC_H__
# define  WULK_XMALLOC_H__

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Includes for alloca() */
#ifndef alloca
#	if defined(__GNUC__)
#		define alloca(x)       __builtin_alloca(x)
#	elif defined(_WIN32)
#		include <malloc.h>
#   ifdef _alloca
#     define alloca _alloca
#   endif
#	else
#		error do not know how to get alloca
#	endif
#endif

/**
 * Allocate @p size bytes on the heap.
 * This is a wrapper for malloc which calls panic() in case of errors, so no
 * error handling is required for code using it.
 */
void *xmalloc(size_t size);

/**
 * Chane size of a previously allocated memory block to @p size bytes.
 * This is a wrapper for realloc which calls panic() in case of errors, so no
 * error handling is required for code using it.
 */
void *xrealloc(void *ptr, size_t size);

void *xcalloc(size_t num, size_t size);

void xfree(void *ptr);

#endif /* WULK_XMALLOC_H__ */
