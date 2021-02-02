#include "priority_queue.h"

void swap(priority_queue *pq, size_t i, size_t j) {
  // if values are in hash table, update the indices
  if (ht_get_value(pq->ht, &pq->a[i]->elem, 0)) {
    ht_set_value(pq->ht, &pq->a[i]->elem, &j);
  }

  if (ht_get_value(pq->ht, &pq->a[j]->elem, 0)) {
    ht_set_value(pq->ht, &pq->a[j]->elem, &i);
  }

  // copy data
  pair tmp; // static allocation is fine (we know the size in advance)

  memcpy(&tmp, pq->a[i], sizeof(pair));
  memcpy(pq->a[i], pq->a[j], sizeof(pair));
  memcpy(pq->a[j], &tmp, sizeof(pair));
}

// O(lg n)
// On the worst case, we need to shift a[i] all
// the way to the lowest level, taking lg n calls
void min_heapify(priority_queue *pq, size_t i) {
  size_t l = left(i);
  size_t r = right(i);
  size_t smallest = i;

  if (l < pq->size && pq->a[l]->priority < pq->a[i]->priority) {
    smallest = l;
  }

  if (r < pq->size && pq->a[r]->priority < pq->a[smallest]->priority) {
    smallest = r;
  }

  if (smallest != i) {
    swap(pq, i, smallest);
    min_heapify(pq, smallest);
  }
}

priority_queue *pq_init(priority_queue *pq, size_t max) {
  assert(pq);

  pq->a = calloc(max, sizeof(pair *));
  assert(pq->a);

  pq->ht = ht_init(calloc(1, sizeof(hash_table)), sizeof(int), sizeof(int), 2 * max);
  assert(pq->ht);

  for (size_t i = 0; i < max; ++i) {
   pq->a[i] = calloc(1, sizeof(pair));
   assert(pq->a[i]);
   pq->a[i]->elem = -1;
   pq->a[i]->priority = INFINITY;
  }

  pq->size = 0;
  pq->max = max;

  return pq;
}

void pq_destroy(priority_queue *pq) {
  assert(pq);
  assert(pq->a);
  for (size_t i = 0; i < pq->max; i++) {
    free(pq->a[i]);
  }
  ht_destroy(pq->ht);
  free(pq->ht);
  free(pq->a);
}

// O(lg n)
// On worst case, inserted key will be shifted all the way up
void pq_insert(priority_queue *pq, int elem, double priority) {
  assert(pq);
  assert(pq->a);
  assert(pq->size < pq->max);

  pq->size += 1;

  size_t i =  pq->size - 1;

  // add new entry to hash table
  int tmp = (int) i;
  ht_insert(pq->ht, &elem, &tmp);

  pq->a[i]->elem = elem;
  pq->a[i]->priority = INFINITY;
  pq_decrease_priority(pq, pq->size-1, priority);
}

// O(1)
int pq_minimum(priority_queue *pq) {
  assert(pq);
  assert(pq->a);
  return pq->a[0]->elem;
}

// O(lg n)
// Constant operations, plus call to O(lg n) procedure
int pq_extract_min(priority_queue *pq) {
  assert(pq);
  assert(pq->a);
  assert(pq->size > 0);

  int min = pq->a[0]->elem;
  swap(pq, 0, pq->size - 1);
  pq->size -= 1;

  ht_remove(pq->ht, &min);

  min_heapify(pq, 0);

  return min;
}

// O(lg n)
// On worst case, leave will go all the way to the top,
// taking h = lg n operations
void pq_decrease_priority(priority_queue *pq, size_t i, double priority) {
  assert(pq);
  assert(pq->a);
  assert(i < pq->max);
  assert(priority <= pq->a[i]->priority);

  pq->a[i]->priority = priority;
  while (i > 0 && pq->a[parent(i)]->priority > pq->a[i]->priority) {
    swap(pq, i, parent(i));
    i = parent(i);
  }
}

int pq_index_of(priority_queue *pq, int elem) {
  assert(pq);
  assert(pq->a);

  int index;
  if (!ht_get_value(pq->ht, &elem, &index)) { // possibly broken
    return -1;
  }

  return index;
}

