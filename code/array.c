#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int array_value_t;

typedef struct Array_s {
  int length;
  int capacity;
  array_value_t *elements;

  void (*append)(struct Array_s *self, array_value_t value);
  void (*prepend)(struct Array_s *self, array_value_t value);
  array_value_t (*remove)(struct Array_s *self, array_value_t value, bool *success);
  array_value_t (*get)(struct Array_s *self, int index, bool *success);
  int (*index_of)(struct Array_s *self, array_value_t value);
  array_value_t (*remove_at)(struct Array_s *self, int index, bool *success);
  array_value_t (*pop)(struct Array_s *self, bool *success);
  bool (*is_full)(struct Array_s *self);
  void (*insert_at)(struct Array_s *self, array_value_t value, int index, bool *success);
  void (*_expand)(struct Array_s *self);
} Array_t;

bool array_is_full(Array_t *self) { return self->length == self->capacity; }

void array_append(Array_t *self, array_value_t value) {
  if (self->is_full(self)) {
    self->_expand(self);
  }

  self->elements[self->length] = value;
  self->length++;

  return;
}

void array_prepend(Array_t *self, array_value_t value) {
  if (self->is_full(self)) {
    self->_expand(self);
  }

  for (int i = self->length; i > 0; i--) {
    self->elements[i] = self->elements[i - 1];
  }

  self->elements[0] = value;
  self->length++;
}

array_value_t array_get(Array_t *self, int index, bool *success) {
  if (index < 0 || index >= self->length) {
    *success = false;
    return -1;
  }
  *success = true;
  return self->elements[index];
}

array_value_t array_remove_at(struct Array_s *self, int index, bool *success) {
  if (index < 0 || index >= self->length) {
    *success = false;
    return -1;
  }

  int value = self->elements[index];
  for (int i = index; i < self->length; i++) {
    self->elements[i] = self->elements[i + 1];
  }
  self->length--;
  *success = true;
  return value;
}

array_value_t array_remove(struct Array_s *self, array_value_t value, bool *success) {
  int index;
  if ((index = self->index_of(self, value)) > -1)
    return self->remove_at(self, index, success);

  *success = false;
  return -1;
}

int array_index_of(struct Array_s *self, array_value_t value) {
  for (int i = 0; i < self->length; i++) {
    if (self->elements[i] == value)
      return i;
  }
  return -1;
}

void array_insert_at(struct Array_s *self, array_value_t value, int index,
                     bool *success) {
  if (index < 0 || index > self->length) {
    *success = false;
    return;
  }

  if (self->is_full(self)) {
    self->_expand(self);
  }

  for (int i = self->length; i > index - 1; i--) {
    self->elements[i] = self->elements[i - 1];
  }

  self->elements[index] = value;
  self->length++;
  *success = true;
}

array_value_t array_pop(struct Array_s *self, bool *success) {
  if (self->length < 1) {
    *success = false;
    return -1;
  }

  int value = self->elements[self->length - 1];
  self->length--;
  *success = true;
  return value;
}

void array_expand(struct Array_s *self) {
  array_value_t *new_p = realloc(self->elements, self->capacity * 2 * sizeof(int));

  if (new_p != NULL) {
    self->elements = new_p;
    return;
  }

  printf("Unable to expand array");
  exit(1);
}

Array_t make_array_t(int capacity) {
  Array_t out = {.length = 0,
                 .capacity = capacity,
                 .elements = calloc(capacity, sizeof(int))};

  out.is_full = array_is_full;
  out.index_of = array_index_of;
  out.append = array_append;
  out.prepend = array_prepend;
  out.get = array_get;
  out.remove_at = array_remove_at;
  out.remove = array_remove;
  out.insert_at = array_insert_at;
  out.pop = array_pop;
  out._expand = array_expand;

  return out;
};

int main() {
  bool success;
  Array_t items = make_array_t(2);

  items.append(&items, 5);
  items.append(&items, 7);
  items.append(&items, 9);

  ASSERT(items.get(&items, 2, &success) == 9, "Failed");

  ASSERT(items.remove_at(&items, 1, &success) == 7, "Failed");
  ASSERT(items.length == 2, "Failed");

  items.append(&items, 11);

  ASSERT(items.remove_at(&items, 1, &success) == 9, "Failed");
  ASSERT(items.remove_at(&items, 9, &success) == -1, "Failed");
  ASSERT(success == false, "Failed");
  ASSERT(items.remove_at(&items, 0, &success) == 5, "Failed");
  ASSERT(items.remove_at(&items, 0, &success) == 11, "Failed");
  ASSERT(items.length == 0, "Failed");

  items.prepend(&items, 5);
  items.prepend(&items, 7);
  items.prepend(&items, 9);

  ASSERT(items.get(&items, 2, &success) == 5, "Failed");
  ASSERT(items.get(&items, 0, &success) == 9, "Failed");
  ASSERT(items.remove(&items, 9, &success) == 9, "Failed");

  ASSERT(items.length == 2, "Failed");
  ASSERT(items.get(&items, 0, &success) == 7, "Failed");

  items.insert_at(&items, 3, 1, &success);

  ASSERT(items.length == 3, "Failed");

  ASSERT(items.get(&items, 0, &success) == 7, "Failed");
  ASSERT(items.get(&items, 1, &success) == 3, "Failed");
  ASSERT(items.get(&items, 2, &success) == 5, "Failed");

  ASSERT(items.index_of(&items, 3) == 1, "Failed");

  ASSERT(items.pop(&items, &success) == 5, "Failed");
  ASSERT(items.length == 2, "Failed");

  ASSERT(items.pop(&items, &success) == 3, "Failed");
  ASSERT(items.pop(&items, &success) == 7, "Failed");

  printf("✅ success\n");

  return 0;
}

// gcc array.c -o array.out && ./array.out && rm array.out
