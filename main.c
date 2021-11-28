#include <stdlib.h>
#include <stdio.h>
#include "pgm.h"
#include "ccl.h"
#include "mem.h"

/**
 * If .pgm is not filled in the user input filepath, fills it in
 * @param filepath Filepath from the user
 * @param input Filepath for the programme
 */
void check_user_input(const char *filepath, char **programme_filepath) {
    int length = 0, i;
    byte symbol;

    do { // Counting the input length
        symbol = filepath[length++];
    } while (symbol);

    // If the input already has .pgm, just copy it to input
    if (filepath[length - 5] == '.' && filepath[length - 4] == 'p' &&
        filepath[length - 3] == 'g' && filepath[length - 2] == 'm') {
        (*programme_filepath) = (char *) mymalloc(length * sizeof(char));
        if (!*programme_filepath) return;
        for (i = 0; i < length; i++) {
            (*programme_filepath)[i] = filepath[i];
        }
    } // If the input doesn't have .pgm make it have .pgm
    else {
        (*programme_filepath) = (char *) mymalloc((length + 4) * sizeof(char));
        if (!*programme_filepath) return;
        for (i = 0; i < length - 1; i++) {
            (*programme_filepath)[i] = filepath[i];
        }
        (*programme_filepath)[i++] = '.';
        (*programme_filepath)[i++] = 'p';
        (*programme_filepath)[i++] = 'g';
        (*programme_filepath)[i++] = 'm';
        (*programme_filepath)[i] = 0x00;
    }
}

/**
 * Checks the arguments from command line
 * Expected ccl.exe <input-file[.pgm]> <output-file>
 * If there's not 2 arguments, exit's the programme with exit code 3
 * @param argc Argument counter (cmd arguments)
 * @param argv Argument values (cmd arguments)
 * @param input Input filepath
 * @param result Result filepath
 */
void arguments(int argc, char *argv[], char **input, char **result) {
    // Sanity check, sort of
    if (argc != 3) {
        perror("ERROR: Expected execution of program as shown below:\nccl.exe <input-file[.pgm]> <output-file>\n");
        exit(3);
    }

    check_user_input(argv[1], input);
    check_user_input(argv[2], result);
}

/**
 * Function checks if the input file is in binary form
 * If the input file includes other bytes than just 0x00 and 0xFF
 * writes out an error and exit's the programme with exit code 2.
 * @param p input file processed into pgm structure
 */
void check_binary_input(pgm *p) {
    if (!is_pgm_binary(p)) {
        perror("ERROR: Input file is not binary!\nInput file must be black and white!\n");
        exit(2);
    }
}

/**
 * Main entry point of the app
 * @param argc Argument counter (cmd arguments)
 * @param argv Argument values (cmd arguments)
 * @return 0 if everything went well OR non-zero value if something went wrong
 */
int main(int argc, char *argv[]) {
    // Ini
    pgm *image;
    char *input = NULL;
    char *result = NULL;

    // Input & result image, cmd arguments
    arguments(argc, argv, &input, &result);

    // PMG struct
    image = create_pgm(input);
    if (!image) {
        perror("ERROR: Out of memory!");
        return EXIT_FAILURE;
    }

    // Checking if the input is binary
    check_binary_input(image);

    // CCL Algorithm, image data is coloured after this
    if (!run_ccl_algo(image->data, image->width, image->height)) {
        perror("ERROR: Error occurred while running the main algorithm");
        return EXIT_FAILURE;
    }

    // Output image, result
    if (!write_pgm_file(image, result)) {
        perror("ERROR: Error occurred while creating output file!");
        return EXIT_FAILURE;
    }

    // Free
    myfree((void **) &input);
    myfree((void **) &result);
    free_pgm(&image);
    printf("Allocated memory blocks at the end: %d\n", mem_blocks);

    return EXIT_SUCCESS;
}