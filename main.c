#include <stdlib.h>
#include <stdio.h>
#include "pgm.h"
#include "ccl.h"

/**
 * Function checks if the input file is in binary form
 * If the input file includes other bytes than just 0x00 and 0xFF
 * writes out an error and exit's the programme with exit code 2.
 * @param p input file processed into pgm structure
 */
void check_binary_input(pgm* p) {
    if(!is_pgm_binary(p)) {
        perror("ERROR: Input file is not binary!\nInput file must be black and white!\n");
        exit(2);
    }
}

int main() {
    pgm *image;
    char *input = "../data/w2test.pgm";
    char *result = "../data/result.pgm";

    image = create_pgm(input);
    if (!image) return EXIT_FAILURE;

    check_binary_input(image);

    run_ccl_algo(image->data, image->width, image->height);

    write_pgm_file(image, result);

    free_pgm(&image);
    return EXIT_SUCCESS;
}