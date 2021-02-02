#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <float.h>

// Default table size
#define HT_DEFAULT_SIZE 5201

typedef struct __ht_entry {
  void *key;
  void *value;
  struct __ht_entry *next;
} __ht_entry;

typedef struct hash_table {
  // size of value element in bytes
  size_t value_size;
  // size of key element in bytes
  size_t key_size;
  // array of buckets
  __ht_entry **entries;
  // hashing function
  // s_hash used by default
  size_t (*hash_func)(const void *key);
  // optional function used to compare keys
  // s_comp (memcmp wrapper) used by default
  int (*kcomp)(const void *x, const void *y, size_t n);
  // size of the array
  size_t max;
  // number of elements stored
  size_t count;
} hash_table;

__ht_entry *__ht_entry_init(__ht_entry *entry, hash_table *ht, const void *key,
    const void *value, __ht_entry *next);

__ht_entry *__ht_entry_destroy(__ht_entry *entry);

// Initializes hash table ht with attributes
hash_table *ht_init(hash_table *ht, size_t key_size,
                    size_t value_size, size_t max);

// Changes value of entry with corresponding key
// Terminates execution if key not found
void ht_set_value(hash_table *ht, const void *key, const void *value);

// Inserts new key-value pair
void ht_insert(hash_table *ht, const void *key, const void *value);

// Removes entry with corresponding key
void ht_remove(hash_table *ht, const void *key);

// Copies value of some key
// Returns whether the table contains the key
bool ht_get_value(hash_table *ht, const void *key, void *value);

// Copies ht's contents to 'keys' and 'values' arrays
void ht_arrays(hash_table *ht, uint8_t *keys, uint8_t *values);

// Executes func on each element of the table
void ht_iterate(hash_table *ht, void func(void *key, void *value));

// Destroys hash_table instance
void ht_destroy(hash_table *ht);

// Standard harh function
size_t s_hash(const void *x);

// Standard key comparison function
int s_comp(const void *x, const void *y, size_t n);

// Double comparison function
int __dbl_kcomp(const void *x, const void *y, size_t n);
