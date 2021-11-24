#ifndef __PGM_H__
#define __PGM_H__

typedef unsigned int uint;
typedef unsigned char byte;

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