#include <stdlib.h>
#include <stdio.h>
#include "pgm.h"
#include "mem.h"

int main() {
    pgm *image;
    char *input = "../data/w2test.pgm";
    char *result = "../data/result.pgm";

    image = create_pgm(input);
    if (!image) return EXIT_FAILURE;

    write_pgm_file(image, result);

    printf("Before free: %d\n", mem_blocks);

    free_pgm(&image);

    printf("After free: %d\n", mem_blocks);

    return EXIT_SUCCESS;
}