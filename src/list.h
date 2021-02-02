#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef void (*deinitFunction)(void *);

typedef struct ListNode {
  void *data;
  struct ListNode *next;
} ListNode;

typedef struct List {
  // Number of elements on the list
  uint64_t count;
  // Size of a data element
  size_t size;
  // Deinit function for data element
  deinitFunction deinit;
  // Pointer to the first node
  ListNode *first;
  // Pointer to the last node
  ListNode *last;

  // Pointer to 'current' node, for iteration
  ListNode *current;
  // Pointer to 'previous' node
  ListNode *previous;

} List;

// Initalizes list
// deinitFunction: Function to be called whenever some element is removed or the list is deinitialized.
// elementSize: Size of the stored data element.
List *initList(deinitFunction deinit, size_t elementSize);
// Frees up the list and all its elements
void deinitList(List *ls);

// Modifiers
// Inserts element after the last node of the list
void append(List *ls, void *element);
void __append(List *ls, void *element);

// Insert element before the first node of the list
void prepend(List *ls, void *element);
void __prepend(List *ls, void *element);

// Removes the first element of the list.
// Data is copied to the 'data' pointer.
void removeFirst(List *ls, void *data);
// Removes the last element of the list.
// Data is copied to the 'data' pointer.
void removeLast(List *ls, void *data);

// Accessors
uint64_t listCount(List *ls);
bool isEmpty(List *ls);
void *getFirstRef(List *ls);
void *getLastRef(List *ls);

// Iterators
void *resetIteration(List *ls);
void *getCurrentRef(List *ls);
void removeCurrent(List *ls, void *element);
void *__removeCurrent(List *ls);
void *getNextRef(List *ls);

void forEach(List *ls, void (*function)(void *));

