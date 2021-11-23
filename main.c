#include <stdlib.h>
#include "pgm.h"

int main() {
    pgm *image;
    char *input = "../data/w2test.pgm";
    char *result = "../data/result.pgm";

    image = create_pgm(input);
    if (!image) return EXIT_FAILURE;

    write_pgm_file(image, result);

    free_pgm(&image);

    return EXIT_SUCCESS;
}