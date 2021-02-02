#include "hash_table.h"

__ht_entry *__ht_entry_init(__ht_entry *entry, hash_table *ht, const void *key,
    const void *value, __ht_entry *next) {
  assert(ht);
  assert(entry);
  assert(key);
  assert(value);

  entry->key = calloc(ht->key_size, 1); // watch out
  assert(entry->key);
  memcpy(entry->key, key, ht->key_size);

  entry->value = calloc(ht->value_size, 1); // watch out
  assert(entry->value);
  memcpy(entry->value, value, ht->value_size);

  entry->next = next;
  return entry;
}

__ht_entry *__ht_entry_destroy(__ht_entry *entry) {
  assert(entry);
  free(entry->key);
  entry->key = 0;
  free(entry->value);
  entry->value = 0;
  return entry->next;
}

hash_table *ht_init(hash_table *ht, size_t key_size,
                    size_t value_size, size_t max) {
  assert(ht);

  ht->max = max;
  ht->count = 0;
  ht->value_size = value_size;
  ht->key_size = key_size;
  ht->hash_func = s_hash; // standard hash
  ht->kcomp = s_comp;  // standard comp
  ht->entries = calloc(max, sizeof(__ht_entry *));
  assert(ht->entries);
  return ht;
}

void ht_destroy(hash_table *ht) {
  assert(ht);

  for (size_t i = 0; i < ht->max; i++) {
    __ht_entry *ptr = ht->entries[i];
    while (ptr) {
      __ht_entry *tmp = ptr->next;
      __ht_entry_destroy(ptr);
      free(ptr);
      ptr = tmp;
    }
  }

  ht->count = 0;
  ht->max = 0;
  ht->key_size = 0;
  ht->value_size = 0;
  ht->hash_func = 0;
  ht->kcomp = 0;

  free(ht->entries);
}


void ht_set_value(hash_table *ht, const void *key, const void *value) {
  assert(ht);
  assert(key);
  assert(value);
  size_t i = ht->hash_func(key) % ht->max;

  __ht_entry *ptr = ht->entries[i];

  // find entry with key
  while (ptr && ht->kcomp(ptr->key, key, ht->key_size) != 0) {
    ptr = ptr->next;
  }

  assert(ptr); // lets make sure key has been found
  memcpy(ptr->value, value, ht->value_size);
}

void ht_insert(hash_table *ht, const void *key, const void *value) {
  assert(ht);
  assert(key);
  assert(value);

  size_t i = ht->hash_func(key) % ht->max;

  __ht_entry *ptr = ht->entries[i];

  // insert in the beginning of the list
  __ht_entry *e = __ht_entry_init(calloc(1, sizeof(__ht_entry)), ht, key, value, ptr);
  ht->entries[i] = e;
  ht->count += 1;
}

void ht_remove(hash_table *ht, const void *key) {
  assert(ht);

  size_t i = ht->hash_func(key) % ht->max;
  __ht_entry *ptr = ht->entries[i];
  __ht_entry *prev = 0;
  // find key in table
  while (ptr && ht->kcomp(ptr->key, key, ht->key_size) != 0) {
    prev = ptr;
    ptr = ptr->next;
  }
  assert(ptr); // lets make sure value has been found
  // ptr holds key to be removed;
  if (prev) {
    prev->next = ptr->next;
  } else {
    // since prev is NULL, we're removing the first element
    ht->entries[i] = ptr->next;
  }
  __ht_entry_destroy(ptr);
  free(ptr);

  ht->count -= 1;
}

bool ht_get_value(hash_table *ht, const void *key, void *value) {
  assert(ht);
  assert(key);

  size_t i = ht->hash_func(key) % ht->max;
  __ht_entry *ptr = ht->entries[i];
  // find key in table
  while(ptr && ht->kcomp(ptr->key, key, ht->key_size) != 0) {
    ptr = ptr->next;
  }

  if (!ptr) {
    // did not find key
    return false;
  }

  if(value) {
    // copy ptr->value to value pointer if supplied
    memcpy(value, ptr->value, ht->value_size);
  }

  return true;
}

void ht_arrays(hash_table *ht, uint8_t *keys, uint8_t *values) {
  assert(ht);
  assert(keys);
  assert(values);

  size_t j = 0;
  for (size_t i = 0; i < ht->max; i++) {
    __ht_entry *ptr = ht->entries[i];
    while (ptr) {
      memcpy(keys + j * ht->key_size, ptr->key, ht->key_size);
      memcpy(values + j * ht->value_size, ptr->value, ht->value_size);
      j++;
      ptr = ptr->next;
    }
  }

}

void ht_iterate(hash_table *ht, void func(void *key, void *value)) {
  assert(ht);

  for (size_t i = 0; i < ht->max; i++) {
    __ht_entry *ptr = ht->entries[i];
    while (ptr) {
      func(ptr->key, ptr->value);
      ptr = ptr->next;
    }
  }
}

size_t s_hash(const void *x) {
  return *((const unsigned*) x); // only in C tm
}

int s_comp(const void *x, const void *y, size_t n) {
  assert(x);
  assert(y);

  return memcmp(x, y, n);
}

int __dbl_kcomp(const void *x, const void *y, size_t n) {
  double p = * (const double *) x;
  double q = * (const double *) y;

  if (fabs(p - q) <= DBL_EPSILON) return 0;
  else if (x < y) return -1;
  else return 1;
}

