#include <stdlib.h>
#include <stdio.h>
#include "pgm.h"

int main() {
    pgm *image;
    char *path = "C:/Users/SpeekeR/Documents/_FAV/2. Druhak/Zimak/PC/Semestralka/data/w2test.pgm";

    image = create_pgm(path);
    if (!image) return EXIT_FAILURE;
    free_pgm(&image);

    return EXIT_SUCCESS;
}