#ifndef __MEM_H__
#define __MEM_H__

#include <stddef.h>

#ifndef __UINT__
#define __UINT__
typedef unsigned int uint;
#endif

extern uint mem_blocks;

void *mymalloc(size_t size);
void *mycalloc(size_t num, size_t size);
void myfree(void **ptr);

#endif