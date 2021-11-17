#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "pgm.h"

/**
 * Creates pgm structure based on input file
 * Expecting first 2 symbols to be "magic-numbers" of pgm (P2, P5 etc)
 * Expecting next symbols to be width and height
 * Expecting next symbol to be max_value of pixel
 * Then expecting the pixels of the image itself
 * @param filepath Path to PGM image file
 * @return Pointer to corresponding structure if everything goes well, NULL otherwise
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

/**
 * Frees the array within the pgm structure, then frees the structure itself
 * @param p double pointer to pgm struct
 */
void free_pgm(pgm **p) {
    // Sanity check
    if (!p) return;

    // Freeing data first, then pgm struct
    free((*p)->data);
    free(*p);
    *p = NULL;
}