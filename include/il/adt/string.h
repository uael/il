/*
 * libil - Intermediate Language cross-platform c library
 * Copyright (C) 2016-2017 Lucas Abel <www.github.com/uael>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>
 */

#ifndef   IL_STRING_H__
# define  IL_STRING_H__

#include <string.h>
#include <stdarg.h>

#include "vector.h"
#include "xmalloc.h"

typedef adt_vector_of(const char *) string_r;

/**
 * Allocates memory and copies string @p str into it.
 * This is a wrapper for strdup which calls panic() in case of errors, so no
 * error handling is required for code using it.
 */
char *xstrdup(const char *str);

/**
 * Allocates memory and copies string @p str into it.
 * This is a wrapper for strndup which calls panic() in case of errors, so no
 * error handling is required for code using it.
 */
char *xstrndup(const char *str, size_t n);

/**
 * Sets `char **' pointer to be a buffer
 * large enough to hold the formatted string
 * accepting a `va_list' args of variadic
 * arguments.
 */

int vasprintf(char **, const char *, va_list);

/**
 * Sets `char **' pointer to be a buffer
 * large enough to hold the formatted
 * string accepting `n' arguments of
 * variadic arguments.
 */

int asprintf(char **, const char *, ...);

#endif /* IL_STRING_H__ */
