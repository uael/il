#ifndef _JAYL_HASH_SET_H_
#define _JAYL_HASH_SET_H_

#include <p99.h>

#define __OFFSET_BASIS 2166136261U
#define __FNV_PRIME 16777619U

static inline __attribute__((pure))
unsigned hash_string(const char *str) {
  const unsigned char *p;
  unsigned hash = __OFFSET_BASIS;

  for (p = (const unsigned char *) str; *p != 0; ++p) {
    hash *= __FNV_PRIME;
    hash ^= *p;
  }

  return hash;
}

static inline __attribute__((pure))
unsigned hash_string_size(const char *str, size_t size) {
  size_t i;
  unsigned hash = __OFFSET_BASIS;

  for (i = 0; i < size; ++i) {
    hash *= __FNV_PRIME;
    hash ^= str[i];
  }

  return hash;
}

#define hset_of(T, name, get_key_macro, init_data_macro) \
  typedef struct { \
    T data; \
    unsigned hash; \
  } P99_PASTE2(name, _entry_t) \
  ; \
  typedef struct { \
    P99_PASTE2(name, _entry_t) *entries; \
    size_t num_buckets, enlarge_threshold, shrink_threshold, num_elements, num_deleted; \
    int consider_shrink; \
    size_t hash; \
    size_t hash; \
    unsigned hash; \
    unsigned hash; \
  } P99_PASTE2(name, _t) \
  ; \
  typedef struct { \
    P99_PASTE2(name, _entry_t) *current_bucket, *end; \
  } P99_PASTE2(name, _it_t)

hset_of(void *, hset, b, l);

#endif /* _JAYL_HASH_SET_H_ */
