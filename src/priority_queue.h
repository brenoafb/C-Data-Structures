#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include "hash_table.h"

#define SWAP(x,y) do \
   { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
     memcpy(swap_temp,&y,sizeof(x)); \
     memcpy(&y,&x,       sizeof(x)); \
     memcpy(&x,swap_temp,sizeof(x)); \
    } while(0)

typedef struct pair {
  double priority;
  int elem;
} pair;

typedef struct priority_queue {
  pair **a;
  size_t max;
  size_t size;
  hash_table *ht;
} priority_queue;

// Basics
// swaps elements in indices i and j in pq
void swap(priority_queue *pq, size_t i, size_t j);

static inline size_t parent(size_t i) { return (i-1)/2; }
static inline size_t left(size_t i) { return (2*i) + 1; }
static inline size_t right(size_t i) { return (2*i) + 2; }

// Mantains min heap property for ith element branch
void min_heapify(priority_queue *pq, size_t i);

// Priority Queue operations
// initializes priority queue with attributes
priority_queue *pq_init(priority_queue *pq, size_t max);
// destroys priority queue instance
void pq_destroy(priority_queue *pq);
// inserts value with priority in pq
void pq_insert(priority_queue *pq, int value, double priority);
// returns the value of the element with least priority  stored in the queue
int pq_minimum(priority_queue *pq);
// removes element stored with least priority and returns its value
int pq_extract_min(priority_queue *pq);
// reduces priority of element stored in index i
void pq_decrease_priority(priority_queue *pq, size_t i, double priority);
// returns whethes queue contains elem
bool pq_contains(priority_queue *pq, int elem);
// returns the index of elem in pq or -1 if not found
int pq_index_of(priority_queue *pq, int elem);
// returns whether pq is empty
static inline bool pq_empty(priority_queue *pq) { return pq->size == 0; }

