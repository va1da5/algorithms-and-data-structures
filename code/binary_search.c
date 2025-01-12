#include "utils.h"
#include <stdio.h>

int binary_search(int array[], int array_length, int needle) {
  int low = 0;
  int high = array_length;

  while (low < high) {
    int middle = low + (high - low) / 2;

    int value = array[middle];

    if (value == needle)
      return middle;

    if (needle < value) {
      high = middle;
      continue;
    }

    low = middle + 1;
  }

  return -1;
}

int main() {
  ASSERT(binary_search((int[7]){1, 2, 3, 5, 6, 8, 9}, 7, 8) == 5,
         "Expected index to be 5");

  ASSERT(binary_search((int[7]){1, 2, 3, 5, 6, 8, 9}, 7, 5) == 3,
         "Expected index to be 3");

  ASSERT(binary_search((int[7]){1, 2, 3, 5, 6, 8, 9}, 7, 9) == 6,
         "Expected index to be 6");

  ASSERT(binary_search((int[7]){1, 2, 3, 5, 6, 8, 9}, 7, 11) == -1,
         "Expected index to be -1 as item is not present");

  printf("✅ success\n");
  return 0;
}

// gcc binary_search.c -o binary_search.out && ./binary_search.out && rm binary_search.out
