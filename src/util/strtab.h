#ifndef _JAYL_STRTAB_H_
#define _JAYL_STRTAB_H_

#include "adt/string.h"

#include <stddef.h>

/**
 * Register a string and store it internally, allocating a new copy if
 * needed. Manages memory ownership for all string constants used at
 * runtime.
 */
string_t str_register(const char *str, size_t len);

/* Concatenate two strings together. */
string_t str_cat(string_t a, string_t b);

#endif /* _JAYL_STRTAB_H_ */
