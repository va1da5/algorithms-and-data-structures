#include "utils.h"
#include <stdio.h>
#include <string.h>

void bubble_sort(int array[], int array_size) {
  for (int i = 0; i < array_size; i++) {
    for (int j = 0; j < array_size - 1 - i; j++) {
      if (array[j] > array[j + 1]) {
        int tmp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = tmp;
      }
    }
  }
}

int main() {

  int array[7] = {9, 3, 7, 4, 69, 420, 42};
  int sorted[7] = {3, 4, 7, 9, 42, 69, 420};
  int array_size = sizeof(array) / sizeof(array[0]);

  bubble_sort(array, array_size);

  ASSERT(memcmp(array, sorted, array_size) == 0, "Sorted incorrectly");

  printf("✅ success\n");
  return 0;
}

// gcc bubble_sort.c -o bubble_sort.out && ./bubble_sort.out && rm bubble_sort.out
