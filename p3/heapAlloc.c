///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019-2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
///////////////////////////////////////////////////////////////////////////////
 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "heapAlloc.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           
    int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit 
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    * 
    * End Mark: 
    *  The end of the available memory is indicated using a size_status of 1.
    * 
    * Examples:
    * 
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27
    *      If the previous block is free, size_status should be 25
    * 
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26
    *      If the previous block is free, size_status should be 24
    *    Footer:
    *      size_status should be 24
    */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */
blockHeader *heapCurrent;  

 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block on success.
 * Returns NULL on failure.
 * This function should:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
 * - Use NEXT-FIT PLACEMENT POLICY to chose a free block
 * - Use SPLITTING to divide the chosen free block into two if it is too large.
 * - Update header(s) and footer as needed.
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* allocHeap(int size) {     
    //TODO: Your code goes in here.
    dumpMem();
    if (size < 0 || size + sizeof(blockHeader) > allocsize) {
        return NULL;
    }
    printf("The size of blockHeader is %d\n", sizeof(blockHeader));    

    static int allocOnce = 0;
    int blockSize = 0;
    
    if ((size + sizeof(blockHeader)) % 8 == 0) {
        blockSize = size + sizeof(blockHeader);
    } else {
        blockSize = ((size + sizeof(blockHeader)) / 8 + 1) * 8;
    }
    printf("heapStart is %08x\n", (unsigned int)(heapStart));   
    printf("The size status of heapStart is %d\n", heapStart->size_status); 
    if (allocOnce == 0) {
        heapCurrent = heapStart;
        allocOnce = 1;
    } 
    
    blockHeader *heapNow = heapCurrent;
    printf("heapNow is %08x\n", (unsigned int)(heapNow));
    printf("The size status of heapNow is %d\n", heapNow->size_status); 
 
    printf("heapCurrent is %08x\n", (unsigned int)(heapCurrent));
    printf("The size status of heapCurrent is %d\n", heapCurrent->size_status); 
    
    while (heapNow->size_status != 1) {
        int nextBlockSize = heapNow->size_status - (heapNow->size_status & 3);
        blockHeader *heapFooter;
        printf("nextBlockSize is %d\n", nextBlockSize);
        // the next block has been allocated 
        if (heapNow->size_status & 1) {
            heapNow = (blockHeader*)((void*)heapNow + heapNow->size_status / 4);
            printf("First Case\n");
            continue;
        // the next block is not large enough
        } else if (nextBlockSize < blockSize) {
            heapNow = (blockHeader*)((void*)heapNow + heapNow->size_status / 4);
            printf("Second Case\n");
            continue;
        // no need splitting 
        } else if (nextBlockSize == blockSize) { 
            heapNow->size_status ++;
            heapCurrent = (blockHeader*)((void*)heapNow + blockSize / 4);
            heapCurrent->size_status += 2;
            printf("Third Case\n");
            dumpMem();
            return (void*)(heapNow + 1);
        } else {
            printf("1");
            heapNow->size_status ++;
            printf("2");
            heapCurrent = (blockHeader*)((void*)heapNow + blockSize / 4);
            printf("3");
            heapCurrent->size_status = nextBlockSize - blockSize;
            printf("4");
            heapFooter = (blockHeader*)((void*)heapCurrent + ((heapCurrent->size_status / 4) + 1));
            printf("5");
            heapFooter->size_status = heapCurrent->size_status;
            printf("6");
            heapCurrent->size_status += 2;
            printf("7");
            printf("Fourth Case\n");
            dumpMem();
            return (void*)(heapNow + 1);
        }
        
    }

    // wrap around
    heapNow = heapStart;
    printf("\nWrap around!\n\n"); 
    while (heapNow != heapCurrent) {
        int nextBlockSize = heapNow->size_status - (heapNow->size_status & 3);
        blockHeader *heapFooter;
        // the next block has been allocated 
        if (heapNow->size_status & 1) {
            heapNow = (blockHeader*)((void*)heapNow + heapNow->size_status / 4);
            printf("The First First Case\n");
            continue;
        // the next block is not large enough
        } else if (nextBlockSize < blockSize) {
            heapNow = (blockHeader*)((void*)heapNow + heapNow->size_status / 4);
            printf("The Second Second Case\n");
            continue;
        // no need splitting 
        } else if (nextBlockSize == blockSize) { 
            heapNow->size_status ++;
            heapCurrent = (blockHeader*)((void*)heapNow + blockSize / 4);
            heapCurrent->size_status += 2;
            printf("The Third Third Case\n");
            return (void*)(heapNow + 1);
        } else {
            heapNow->size_status ++;
            heapCurrent = (blockHeader*)((void*)heapNow + blockSize / 4);
            heapCurrent->size_status = nextBlockSize - blockSize;
            heapFooter = (blockHeader*)((void*)heapCurrent + (heapCurrent->size_status / 4 + 1));
            heapFooter->size_status = heapCurrent->size_status;
            heapCurrent->size_status += 2;
            printf("The Fourth Fourth Case\n");
            return (void*)(heapNow + 1);
        }
          
    }
    return NULL;
} 
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Return
s -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
 * - Update header(s) and footer as needed.
 */                    
int freeHeap(void *ptr) {    
    if (ptr == NULL) {
        return -1;
    }
    if ((int)ptr % 8 != 0) {
        return -1;
    }
    blockHeader* heapEnd = (char*)heapStart + allocSize
} 
 
/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int initHeap(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple initHeap calls
 
    int pagesize;  // page size
    int padsize;   // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dumpMem() {     
 
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "used");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "used");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total used size = %d\n", used_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", used_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;  
} 
