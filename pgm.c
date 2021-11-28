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
    // Initialization
    FILE *fp = NULL;
    pgm *temp = NULL;
    unsigned char ch;
    int i = 0;

    // Opening file from param
    fp = fopen(filepath, "r");
    if (!fp) {
        perror("ERROR: Filepath is invalid, input file might not exist!\n");
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
    fgetc(fp); // Skipping the whitespace after max_value

    // Storing actual PGM image data (pixel values)
    i = 0;
    temp->data = (byte *) malloc(temp->width * temp->width * sizeof(byte));
    if (!temp->data) {
        free(temp);
        return NULL;
    }
    while(1) {
        ch = fgetc(fp);
        if(feof(fp)) break;
        temp->data[i] = ch;
        i++;
    }

    // Checking if the file is properly closed
    if (fclose(fp) == EOF) {
        perror("ERROR Closing input file!\n");
        return 0;
    }

    return temp;
}

/**
 * Checks if the given pgm image is binary (consisting only of 0x00 and 0xFF)
 * @param p pgm image saved as struct pgm
 * @return 1 if the image is binary, 0 if it's not
 */
int is_pgm_binary(pgm *p) {
    // Initialization
    int i = 0;

    // Sanity check
    if (!p) return FAILURE;

    // Checking data byte by byte
    for (i = 0; i < p->width * p->height; i++) {
        if (p->data[i] != 0x00 && p->data[i] != 0xFF) return FAILURE;
    }

    return SUCCESS;
}

/**
 * Creates a .pgm file based on pgm struct
 * @param p pointer to pgm struct that is to be written into a file
 * @param filepath filepath to the new file
 * @return 1 if everything went well, 0 if there was an error
 */
int write_pgm_file(pgm *p, char *filepath) {
    // Initialization
    FILE *fp = NULL;
    int i = 0;

    // Sanity check
    if (!p) return FAILURE;

    // Opening file for writing
    fp = fopen(filepath, "w");
    if (!fp) {
        perror("ERROR: Something went wrong with creating the file!\nProbably out of memory!\n");
        return FAILURE;
    }

    // Writing actual data into the file
    fprintf(fp, "%s\n", p->magic_number);
    fprintf(fp, "%d %d\n", p->width, p->height);
    fprintf(fp, "%d\n", p->max_value);
    for (i = 0; i < p->width * p->height; i++) {
        fputc(p->data[i], fp);
    }

    // Checking if the file is properly closed
    if (fclose(fp) == EOF) {
        perror("ERROR Closing output file!\n");
        return FAILURE;
    }

    return SUCCESS;
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