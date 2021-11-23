#include <stddef.h>
#include <stdlib.h>
#include "mem.h"

/** allocated memory blocks, for memory leaks checking */
uint mem_blocks = 0;

/**
 * Function uses standard malloc and if the allocation goes well
 * increments the memory blocks counter
 * @param size size of wanted memory block
 * @return pointer to wanted memory block or NULL
 */
void *mymalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) mem_blocks++;
    return ptr;
}

/**
 * Function uses standard free and if the double pointer was not NULL to begin with
 * decrements the memory blocks counter
 * @param ptr pointer to a pointer that's to be freed
 */
void myfree(void **ptr) {
    if (*ptr) mem_blocks--;
    free(*ptr);
    *ptr = NULL;
}