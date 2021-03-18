// first pointer returned is 8-byte aligned
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "heapAlloc.h"
#include <stdio.h>
int main() {
   assert(initHeap(4096) == 0);
    printf("initHeap\n");
   int* ptr = (int*) allocHeap(sizeof(int));
   assert(ptr != NULL);
    printf("ptr != NULL\n");
   assert(((int)ptr) % 8 == 0);
    printf("ptr 8\n");
   exit(0);
}
