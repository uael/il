#include "strtab.h"
#include <jayl/hash.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define STRTAB_SIZE 1024

static hash_map_t strtab;

/*
 * Every unique string encountered, being identifiers or literals, is
 * kept for the lifetime of the program. To save allocations, store the
 * raw string buffer in the same allocation as the struct.
 *
 *  _________ String ________    ________ const char [] ________
 * |                          | |                               |
 * [ <len> | <ptr to data>    ] [ 'H', 'e', 'l', 'l', 'o', '\0' ]
 */
static void *str_hash_add(void *ref) {
  string_t *s;
  char *buffer;
  unsigned short l;

  s = (string_t *) ref;
  l = s->p.len;
  buffer = malloc(sizeof(string_t) + l + 1);
  buffer[sizeof(string_t) + l] = '\0';
  memcpy(buffer + sizeof(string_t), s->p.str, l);
  s = (string_t *) buffer;
  s->p.str = buffer + sizeof(*s);
  s->p.len = l;
  return s;
}

static string_t str_hash_key(void *ref) {
  string_t *str = (string_t *) ref;
  return *str;
}

static void strtab_free(void) {
  hash_dtor(&strtab);
}

string_t str_register(const char *str, size_t len) {
  static int initialized;
  string_t data, *ref;
  assert(len >= 0);

  if (len < SHORT_STRING_LEN) {
    memcpy(data.a.str, str, len);
    data.a.str[len] = '\0';
    data.a.len = (unsigned short) len;
    ref = &data;
  } else {
    if (!initialized) {
      hash_ctor(
        &strtab,
        STRTAB_SIZE,
        str_hash_key,
        str_hash_add,
        free);

      atexit(strtab_free);
      initialized = 1;
    }
    data.p.str = str;
    data.p.len = (unsigned short) len;
    ref = hash_insert(&strtab, &data);
  }

  return *ref;
}

string_t str_cat(string_t a, string_t b) {
  static char buf[SHORT_STRING_LEN + 1];
  size_t len;
  char *str;
  string_t s;

  len = a.len + b.len;
  if (len < SHORT_STRING_LEN) {
    str = buf;
  } else {
    str = calloc(len + 1, sizeof(*str));
  }

  memcpy(str, str_raw(a), a.len);
  memcpy(str + a.len, str_raw(b), b.len);
  s = str_register(str, len);
  if (str != buf) {
    free(str);
  }

  return s;
}
