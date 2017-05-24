/*
 * Copyright (c) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2.1.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
