#include "utils.h"
#include <stdbool.h>
#include <stdio.h>

typedef int value_t;

typedef struct Node_s {
  value_t value;
  struct Node_s *next;
  struct Node_s *prev;
} Node_t;

Node_t *node_new(value_t value) {
  Node_t *node = (Node_t *)malloc(sizeof(Node_t));

  if (!node) {
    printf("Memory allocation failed!\n");
    exit(1);
  }

  node->value = value;
  node->next = node->prev = NULL;

  return node;
}

typedef struct DoublyLinkedList_s {
  int length;
  Node_t *head;
  Node_t *tail;

  void (*prepend)(struct DoublyLinkedList_s *self, value_t value);
  void (*append)(struct DoublyLinkedList_s *self, value_t value);

  void (*insert_at)(struct DoublyLinkedList_s *self, value_t value, int index,
                    bool *success);
  value_t (*remove)(struct DoublyLinkedList_s *self, value_t value,
                    bool *success);
  value_t (*get)(struct DoublyLinkedList_s *self, int index, bool *success);
  value_t (*remove_at)(struct DoublyLinkedList_s *self, int index,
                       bool *success);
  Node_t *(*get_at)(struct DoublyLinkedList_s *self, int index, bool *success);
  value_t (*remove_node)(struct DoublyLinkedList_s *self, Node_t *node,
                         bool *success);

  int (*index_of)(struct DoublyLinkedList_s *self, value_t value);

} DoublyLinkedList_t;

void dll_prepend(DoublyLinkedList_t *self, value_t value) {
  self->length++;

  Node_t *node = node_new(value);

  if (self->head == NULL) {
    self->head = self->tail = node;
    return;
  }

  node->next = self->head;
  self->head->prev = node;
  self->head = node;
}

void dll_append(DoublyLinkedList_t *self, value_t value) {
  self->length++;

  Node_t *node = node_new(value);

  if (self->tail == NULL) {
    self->head = self->tail = node;
    return;
  }

  self->tail->next = node;
  node->prev = self->tail;
  self->tail = node;
}

void dll_insert_at(DoublyLinkedList_t *self, value_t value, int index,
                   bool *success) {
  if (index > self->length || index < 0) {
    *success = false;
    return;
  }

  if (index == self->length) {
    return self->append(self, value);
  }

  if (index == 0) {
    return self->prepend(self, value);
  }

  self->length++;

  Node_t *node = node_new(value);

  Node_t *current_node = self->get_at(self, index, success);

  node->next = current_node;
  node->prev = current_node->prev;
  current_node->prev = node;

  if (node->prev != NULL)
    node->prev->next = node;
}

Node_t *dll_get_at(DoublyLinkedList_t *self, int index, bool *success) {
  Node_t *current = self->head;

  for (int i = 0; i < index; i++) {
    if (current == NULL) {
      *success = false;
      return NULL;
    }
    current = current->next;
  }

  return current;
}

value_t dll_get(DoublyLinkedList_t *self, int index, bool *success) {
  Node_t *node = self->get_at(self, index, success);

  if (*success)
    return node->value;
  return -1;
}

int dll_index_of(DoublyLinkedList_t *self, value_t value) {
  Node_t *current = self->head;

  if (current == NULL)
    return -1;

  for (int i = 0; current != NULL; i++) {
    if (current->value == value) {
      return i;
    }

    current = current->next;
  }

  return -1;
}

value_t dll_remove(DoublyLinkedList_t *self, value_t value, bool *success) {
  Node_t *current = self->head;

  if (current == NULL) {
    *success = false;
    return -1;
  }

  for (int i = 0; current != NULL; i++) {
    if (current->value == value) {
      break;
    }
    current = current->next;
  }

  if (current == NULL) {
    *success = false;
    return -1;
  }

  return self->remove_node(self, current, success);
}

value_t dll_remove_at(struct DoublyLinkedList_s *self, int index,
                      bool *success) {
  Node_t *node = self->get_at(self, index, success);
  if (!*success)
    return -1;

  return self->remove_node(self, node, success);
}

value_t dll_remove_node(struct DoublyLinkedList_s *self, Node_t *node,
                        bool *success) {
  self->length--;
  value_t out;

  if (self->length == 0) {
    out = self->head->value;
    free(self->head);
    self->head = self->tail = NULL;
    return out;
  }

  out = node->value;

  if (node->prev)
    node->prev->next = node->next;

  if (node->next)
    node->next->prev = node->prev;

  if (node == self->head)
    self->head = node->next;

  if (node == self->tail)
    self->tail = node->prev;

  node->prev = node->next = NULL;
  free(node);

  return out;
}

DoublyLinkedList_t doubly_linked_list_new() {
  DoublyLinkedList_t list = {.length = 0, .head = NULL, .tail = NULL};
  list.prepend = dll_prepend;
  list.append = dll_append;
  list.insert_at = dll_insert_at;
  list.get_at = dll_get_at;
  list.get = dll_get;
  list.remove_at = dll_remove_at;
  list.remove_node = dll_remove_node;
  list.index_of = dll_index_of;
  list.remove = dll_remove;
  return list;
}

void print_lld(DoublyLinkedList_t *list) {
  Node_t *current = list->head;
  for (int i = 0; current != NULL; i++) {
    printf("[%d]=%d; ", i, current->value);
    current = current->next;
  }
  printf("\n");
}

int main() {

  bool *success;
  *success = true;

  DoublyLinkedList_t list = doubly_linked_list_new();

  list.append(&list, 5);
  list.append(&list, 7);
  list.append(&list, 9);

  ASSERT(list.get(&list, 2, success) == 9, "");
  ASSERT(list.remove_at(&list, 1, success) == 7, "");
  ASSERT(list.length == 2, "");

  list.append(&list, 11);

  ASSERT(list.remove_at(&list, 1, success) == 9, "");

  ASSERT(list.remove_at(&list, 9, success) == -1, "");
  ASSERT(!*success, "");

  *success = true;

  ASSERT(list.remove_at(&list, 0, success) == 5, "");
  ASSERT(list.remove_at(&list, 0, success) == 11, "");

  ASSERT(list.length == 0, "");

  list.prepend(&list, 5);
  list.prepend(&list, 7);
  list.prepend(&list, 9);

  ASSERT(list.get(&list, 2, success) == 5, "");
  ASSERT(list.get(&list, 0, success) == 9, "");

  ASSERT(list.remove(&list, 9, success) == 9, "");

  ASSERT(list.length == 2, "");

  ASSERT(list.get(&list, 0, success) == 7, "");

  list.insert_at(&list, 3, 1, success);

  ASSERT(list.length == 3, "");

  ASSERT(list.get(&list, 0, success) == 7, "");
  ASSERT(list.get(&list, 1, success) == 3, "");
  ASSERT(list.get(&list, 2, success) == 5, "");

  ASSERT(list.index_of(&list, 5) == 2, "");

  print_lld(&list);

  printf("✅ success\n");
  return 0;
}

// gcc linked_list.c -o linked_list.out && ./linked_list.out && rm linked_list.out
