#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "pgm.h"

/**
 *
 * @param filepath
 * @return
 */
pgm *create_pgm(char *filepath) {
    // Inicialization
    FILE *fp = NULL;
    pgm *temp = NULL;
    unsigned char ch;
    int i = 0;

    // Opening file from param
    fp = fopen(filepath, "r");
    if (!fp) {
        printf("ERROR: Filepath is invalid, input file might not exist!");
        return NULL;
    }

    // Memory alloc
    temp = (pgm *) malloc(sizeof(pgm));
    if (!temp) return NULL;

    // "P5" from the beginning of PGM is stored
    temp->magic_number[0] = fgetc(fp);
    temp->magic_number[1] = fgetc(fp);
    temp->magic_number[2] = 0x00;
    fgetc(fp); // Skipping the whitespace char

    // Storing width
    i = 0;
    while(!isspace(ch = fgetc(fp))) {
        i = i * 10 + ch - '0';
    }
    temp->width = i;

    // Storing height
    i = 0;
    while(!isspace(ch = fgetc(fp))) {
        i = i * 10 + ch - '0';
    }
    temp->height = i;

    // Storing max_value
    i = 0;
    while(!isspace(ch = fgetc(fp))) {
        i = i * 10 + ch - '0';
    }
    temp->max_value = i;

    // Storing actual PGM image data (pixel values)
    i = 0;
    temp->data = (byte *) malloc(temp->width * temp->width * sizeof(byte));
    if (!temp->data) return NULL;
    while(1) {
        ch = fgetc(fp);
        if(feof(fp)) break;
        temp->data[i] = ch;
        i++;
    }

    fclose(fp);
    return temp;
}

void free_pgm(pgm **p) {
    // Sanity check
    if (!p) return;

    // Freeing data first, then pgm struct
    free((*p)->data);
    free(*p);
    *p = NULL;
}