#ifndef __PGM_H__
#define __PGM_H__

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

typedef struct {
    byte magic_number[3];
    uint width;
    uint height;
    uint max_value;
    byte *data;
} pgm;

pgm *create_pgm(char *filepath);
int is_pgm_binary(pgm *p);
int write_pgm_file(pgm *p, char *filepath);
void free_pgm(pgm **p);

#endif