// second allocation is too big to fit
#include <assert.h>
#include <stdlib.h>
#include "heapAlloc.h"
#include <stdio.h>
int main() {
   assert(initHeap(4096) == 0);
    printf("initHeap\n");
   assert(allocHeap(2048) != NULL);
    printf("allocHeap2048\n");
   assert(allocHeap(2047) == NULL);
    printf("allocHeap2047\n");
   exit(0);
}
