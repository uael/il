#ifndef _JAYL_HASH_H_
#define _JAYL_HASH_H_

#include "string.h"

typedef struct {
  /* Number of slots in the top level array. */
  unsigned capacity;

  /*
   * Retrieve string representing the key we are hashing on. Keys are
   * unique identifiers of the elements, meaning they can be compared
   * for equality.
   */
  string_t (*key)(void *);

  /*
   * Element initializer, called when data is added to table. New data
   * is only inserted on hash_insert if it does not exist already,
   * where equality is determined by comparing keys.
   */
  void *(*add)(void *);

  /*
   * Element finalizer, called when data is removed, or table is
   * destroyed.
   */
  void (*del)(void *);

  /*
   * First level array of entries, of length capacity. Resolve
   * collisions by chaining elements.
   *
   * [A] -> [B]
   * [ ]
   * [C]
   * [ ]
   *
   */
  struct hash_entry *table;
} hash_map_t;

/* Initialize hash structure. Must be freed by hash_dtor. */
hash_map_t *hash_ctor(
  hash_map_t *tab,
  unsigned cap,
  string_t (*key)(void *),
  void *(*add)(void *),
  void (*del)(void *));

/* Reset table, clearing all values. Does not deallocate memory. */
void hash_clear(hash_map_t *tab);

/* Free resources owned by table. */
void hash_dtor(hash_map_t *tab);

/* Insert element, or return existing with the same key. */
void *hash_insert(hash_map_t *tab, void *val);

/* Retrieve element matching key, or NULL if not found. */
void *hash_lookup(hash_map_t *tab, string_t key);

/* Remove element matching key. */
void hash_remove(hash_map_t *tab, string_t key);

#endif /* _JAYL_HASH_H_ */
