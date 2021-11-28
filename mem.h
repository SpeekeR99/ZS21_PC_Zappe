#ifndef __MEM_H__
#define __MEM_H__

#include <stddef.h>

typedef unsigned int uint;

extern uint mem_blocks;

void *mymalloc(size_t size);
void *mycalloc(size_t num, size_t size);
void myfree(void **ptr);

#endif