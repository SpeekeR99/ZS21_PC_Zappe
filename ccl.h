#ifndef __CCL_H__
#define __CCL_H__

#define FAILURE 0
#define SUCCESS 1

#ifndef __UINT__
#define __UINT__
typedef unsigned int uint;
#endif
#ifndef MY_BYTE_
#define MY_BYTE_
typedef unsigned char byte;
#endif

int run_ccl_algo(byte *data, uint width, uint height);

#endif