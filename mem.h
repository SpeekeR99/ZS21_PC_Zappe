#ifndef __MEM_H__
#define __MEM_H__

typedef unsigned int uint;

extern uint mem_blocks;

void *mymalloc(size_t size);
void myfree(void **ptr);

#endif