#ifndef __CCL_H__
#define __CCL_H__

#define FAILURE 0
#define SUCCESS 1

typedef unsigned int uint;
typedef unsigned char byte;

int run_ccl_algo(byte *data, uint width, uint height);

#endif