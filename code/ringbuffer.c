#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int ringbuffer_value_t;

typedef struct RingBuffer_s {
  int length;
  int capacity;
  int head;
  int tail;

  ringbuffer_value_t *buffer;

  void (*push)(struct RingBuffer_s *self, ringbuffer_value_t value,
               bool *success);
  ringbuffer_value_t (*pop)(struct RingBuffer_s *self, bool *success);
  ringbuffer_value_t (*get)(struct RingBuffer_s *self, int index,
                            bool *success);
  ringbuffer_value_t (*deque)(struct RingBuffer_s *self, bool *success);
  bool (*is_full)(struct RingBuffer_s *self);

} RingBuffer_t;

bool ringbuffer_is_full(RingBuffer_t *self) {
  return self->length == self->capacity;
}

void exception(char *error) {
  printf("🔥 %s\n", error);
  exit(1);
}

void ringbuffer_push(RingBuffer_t *self, ringbuffer_value_t value,
                     bool *success) {
  if (self->is_full(self)) {
    exception("RingBuffer capacity extension is not implemented!");
  }

  self->buffer[self->tail] = value;
  self->tail = (self->tail + 1) % self->capacity;
  self->length++;
  *success = true;
}

ringbuffer_value_t ringbuffer_pop(RingBuffer_t *self, bool *success) {
  if (self->length < 1) {
    *success = false;
    return -1;
  }

  int index = (self->tail - 1) % self->capacity;
  ringbuffer_value_t value = self->buffer[index];
  self->tail = index;
  self->length--;
  *success = true;
  return value;
}

ringbuffer_value_t ringbuffer_get(RingBuffer_t *self, int index,
                                  bool *success) {
  if (index < 0 || index >= self->length) {
    *success = false;
    return -1;
  }

  int adj_index = (self->head + index) % self->capacity;
  *success = true;
  return self->buffer[adj_index];
}

ringbuffer_value_t ringbuffer_deque(RingBuffer_t *self, bool *success) {
  if (self->length < 1) {
    *success = false;
    return -1;
  }

  ringbuffer_value_t value = self->buffer[self->head];
  self->head = (self->head + 1) % self->capacity;
  self->length--;
  *success = true;
  return value;
}

RingBuffer_t make_ringbuffer_t(int capacity) {
  RingBuffer_t out = {.length = 0,
                      .capacity = capacity,
                      .head = 0,
                      .tail = 0,
                      .buffer = calloc(capacity, sizeof(int))};

  out.is_full = ringbuffer_is_full;
  out.push = ringbuffer_push;
  out.pop = ringbuffer_pop;
  out.get = ringbuffer_get;
  out.deque = ringbuffer_deque;

  return out;
};

int main() {

  bool success;
  char *assert_error = "🔥 Failed";

  RingBuffer_t buffer = make_ringbuffer_t(5);

  buffer.push(&buffer, 5, &success);

  ASSERT(buffer.pop(&buffer, &success) == 5, assert_error);
  ASSERT(buffer.pop(&buffer, &success) == -1, assert_error);
  ASSERT(!success, assert_error);

  buffer.push(&buffer, 42, &success);
  buffer.push(&buffer, 9, &success);

  ASSERT(buffer.pop(&buffer, &success) == 9, assert_error);
  ASSERT(buffer.pop(&buffer, &success) == 42, assert_error);
  ASSERT(buffer.pop(&buffer, &success) == -1, assert_error);
  ASSERT(!success, assert_error);

  buffer.push(&buffer, 42, &success);
  buffer.push(&buffer, 9, &success);
  buffer.push(&buffer, 12, &success);

  ASSERT(buffer.get(&buffer, 2, &success) == 12, assert_error);
  ASSERT(buffer.get(&buffer, 1, &success) == 9, assert_error);
  ASSERT(buffer.get(&buffer, 0, &success) == 42, assert_error);

  ASSERT(buffer.pop(&buffer, &success) == 12, assert_error);
  ASSERT(buffer.pop(&buffer, &success) == 9, assert_error);
  ASSERT(buffer.pop(&buffer, &success) == 42, assert_error);

  buffer.push(&buffer, 22, &success);
  buffer.push(&buffer, 33, &success);
  buffer.push(&buffer, 44, &success);

  ASSERT(buffer.deque(&buffer, &success) == 22, assert_error);
  ASSERT(buffer.length == 2, assert_error);

  ASSERT(buffer.deque(&buffer, &success) == 33, assert_error);

  buffer.push(&buffer, 55, &success);

  ASSERT(buffer.pop(&buffer, &success) == 55, assert_error);
  ASSERT(buffer.pop(&buffer, &success) == 44, assert_error);

  printf("✅ success\n");

  return 0;
}

// gcc ringbuffer.c -o ringbuffer.out && ./ringbuffer.out && rm ringbuffer.out
