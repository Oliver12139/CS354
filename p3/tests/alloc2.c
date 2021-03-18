// a few allocations in multiples of 4 bytes
#include <assert.h>
#include <stdlib.h>
#include "heapAlloc.h"
#include <stdio.h>
int main() {
   assert(initHeap(4096) == 0);
   printf("InitHeap\n");
   assert(allocHeap(4) != NULL);
    printf("allocHeap4\n");
   assert(allocHeap(8) != NULL);
    printf("allocHeap8\n");
   assert(allocHeap(16) != NULL);
    printf("allocHeap16\n");
   assert(allocHeap(24) != NULL);
    printf("allocHeap24\n");
   exit(0);
}
