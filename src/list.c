#include "list.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Initalizes list with desired deinitFunction
List *initList(deinitFunction deinit, size_t elementSize) {
  List *ls = calloc(1, sizeof(List));
  assert(ls != NULL);

  ls->deinit = deinit;
  ls->size = elementSize;
  ls->count = 0;
  ls->first = NULL;
  ls->last = NULL;
  ls->current = NULL;
  ls->previous = NULL;

  return ls;
}

// Frees up the list and all its elements
void deinitList(List *ls) {
  assert(ls != NULL);

  ListNode *n = ls->first;
  while (n != NULL) {
    if (ls->deinit == NULL) {
      free(n->data);
    } else {
      ls->deinit(n->data);
    }
    ListNode *ptr = n;
    n = n->next;
    free(ptr);
  }

  free(ls);
}

// Modifiers
// Inserts element after the last node of the list
void append(List *ls, void *element) {
  // Preconditions
  assert(ls != NULL);
  assert(element != NULL);

  // initlaize ListNode instance
  ListNode *n = calloc(1, sizeof(ListNode));
  assert(n != NULL);
  n->data = calloc(1, ls->size);
  n->next = NULL;
  assert(n->data != NULL);

  // copy data from element pointer
  memcpy(n->data, element, ls->size);

  // Insert
  if (ls->first == NULL) {
    ls->first = n;
    ls->last = n;
  } else {
    ls->last->next = n;
    ls->last = n;
  }
  ls->count += 1;
}

void __append(List *ls, void *element) {
  // Preconditions
  assert(ls != NULL);
  assert(element != NULL);

  // initlaize ListNode instance
  ListNode *n = calloc(1, sizeof(ListNode));
  assert(n != NULL);
  n->data = element;
  n->next = NULL;
  assert(n->data != NULL);

  // Insert
  if (ls->first == NULL) {
    ls->first = n;
    ls->last = n;
  } else {
    ls->last->next = n;
    ls->last = n;
  }
  ls->count += 1;
}

// Insert element before the first node of the list
void prepend(List *ls, void *element) {
  // Preconditions
  assert(ls != NULL);
  assert(element != NULL);

  // initlaize ListNode instance
  ListNode *n = calloc(1, sizeof(ListNode));
  assert(n != NULL);
  n->data = calloc(1, ls->size);
  n->next = NULL;
  assert(n->data != NULL);

  // copy data from element pointer
  memcpy(n->data, element, ls->size);

  // Insert
  if (ls->first == NULL) {
    ls->first = n;
    ls->last = n;
  } else {
    n->next = ls->first;
    ls->first = n;
  }
  ls->count += 1;
}

void __prepend(List *ls, void *element) {
  // Preconditions
  assert(ls != NULL);
  assert(element != NULL);

  // initlaize ListNode instance
  ListNode *n = calloc(1, sizeof(ListNode));
  assert(n != NULL);
  n->data = element;
  n->next = NULL;
  assert(n->data != NULL);

  // Insert
  if (ls->first == NULL) {
    ls->first = n;
    ls->last = n;
  } else {
    n->next = ls->first;
    ls->first = n;
  }
  ls->count += 1;
}


// Removes the first element of the list.
// Data is copied to the 'data' pointer.
void removeFirst(List *ls, void *data) {
  assert(ls != NULL);
  if (isEmpty(ls)) return;
  ListNode *n = ls->first;
  ls->first = ls->first->next;
  if (data != NULL && n->data != NULL) {
    memcpy(data, n->data, ls->size);
    ls->deinit(n->data);
  }
  free(n);
  ls->count -= 1;
}

// Removes the last element of the list.
// Data is copied to the 'data' pointer.
void removeLast(List *ls, void *data) {
  assert(ls != NULL);
  if (isEmpty(ls)) return;
  if (ls->first == ls->last) {
    // List has a single element
    removeFirst(ls, data);
    ls->last = NULL;
    return;
  }

  ListNode *n = ls->last;

  ListNode *ptr = ls->previous;

  if (data != NULL && n->data != NULL) {
    memcpy(data, n->data, ls->size);
    ls->deinit(n->data);
  }
  free(n);
  ls->count -= 1;
}

// Accessors
uint64_t listCount(List *ls) {
  assert(ls != NULL);
  return ls->count;
}

bool isEmpty(List *ls) {
  assert(ls != NULL);
  return ls->count == 0;
}

void *getFirstRef(List *ls) {
  assert(ls != NULL);
  if (ls->first == NULL) {
    return NULL;
  } else {
    return ls->first->data;
  }
}

void *getLastRef(List *ls) {
  assert(ls != NULL);
  if (ls->last == NULL) {
    return NULL;
  } else {
    return ls->last->data;
  }
}

// Iterators
void *resetIteration(List *ls) {
  assert(ls != NULL);
  ls->current = ls->first;
  ls->previous = NULL;
  if (ls->current == NULL) {
    return NULL;
  }
  return ls->current->data;
}

void *getCurrentRef(List *ls) {
  assert(ls != NULL);
  if (ls->current == NULL) {
    return NULL;
  } else {
    return ls->current->data;
  }
}

void removeCurrent(List *ls, void *element) {
  assert(ls != NULL);
  assert(ls->current != NULL);

  if (isEmpty(ls)) return;

  ListNode *n = ls->current;

  if (ls->current == ls->first) {
    // remove head of the list
    ls->first = ls->first->next;
    ls->current = ls->first;
  } else if (ls->current == ls->last) {
    ls->last = ls->previous;
    if (ls->previous != NULL) {
      // maybe not needed
      ls->previous->next = NULL;
    }
    ls->current = NULL;
  } else {
    ls->previous->next = ls->current->next;
    ls->current = ls->current->next;

  }

  if (element != NULL && n->data != NULL) {
    memcpy(element, n->data, ls->size);
  }

  if (n->data != NULL) {
    ls->deinit(n->data);
  }

  free(n);
  ls->count -= 1;
}

void *__removeCurrent(List *ls) {
  assert(ls != NULL);
  assert(ls->current != NULL);

  if (isEmpty(ls)) return NULL;

  ListNode *n = ls->current;
  void *data = n->data;
  if (ls->current == ls->first) {
    // remove head of the list
    ls->first = ls->first->next;
    ls->current = ls->first;
  } else if (ls->current == ls->last) {
    ls->last = ls->previous;
    if (ls->previous != NULL) {
      // maybe not needed
      ls->previous->next = NULL;
    }
    ls->current = NULL;
  } else {
    // ListNode *ptr = ls->previous;
    // ptr->next = ls->current->next;
    // ls->current = ls->current->next;

    ls->previous->next = ls->current->next;
    ls->current = ls->current->next;
  }

  free(n);
  ls->count -= 1;

  return data;
}

void *getNextRef(List *ls) {
  assert(ls != NULL);
  if (ls->current == NULL) {
    return NULL;
  } else {
    ls->previous = ls->current;
    ls->current = ls->current->next;
    if (ls->current != NULL) {
      return ls->current->data;
    } else {
      return NULL;
    }
  }
}

void forEach(List *ls, void (*function)(void *)) {
  void *ptr = resetIteration(ls);
  while(ptr != NULL) {
    function(ptr);
    ptr = getNextRef(ls);
  }
}

