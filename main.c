#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pgm.h"
#include "ccl.h"
#include "mem.h"

/**
 * If .pgm is not filled in the user input filepath, fills it in
 * Uses string.h smartly (unlike the older version of this function
 * which used cycles and a lot of ifs)
 * @param filepath Filepath from the user
 * @param input Filepath for the programme
 */
void check_user_input(const char *filepath, char **programme_filepath) {
    /* Check if last 4 chars of user input filepath is ".pgm" */
    if(!strcmp(filepath + strlen(filepath) - 4, ".pgm")) {
        /* If yes, just copy it to program's filepath */
        *programme_filepath = (char *) mymalloc((strlen(filepath) + 1) * sizeof(char));
        if (!*programme_filepath) return;
        strncpy(*programme_filepath, filepath, strlen(filepath));
    }
    else {
        /* If not, copy it to the program's filepath and append ".pgm" to it */
        *programme_filepath = (char *) mymalloc((strlen(filepath) + 5) * sizeof(char));
        if (!*programme_filepath) return;
        strncpy(*programme_filepath, filepath, strlen(filepath) + 1);
        strncat(*programme_filepath, ".pgm", 4);
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
    /* Sanity check, sort of */
    if (argc != 3) {
        printf("ERROR: Expected execution of program as shown below:\nccl.exe <input-file[.pgm]> <output-file>\n");
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
        printf("ERROR: Input file is not binary!\nInput file must be black and white!\n");
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
    /* Ini */
    pgm *image;
    char *input = NULL;
    char *result = NULL;

    /* Input & result image, cmd arguments */
    arguments(argc, argv, &input, &result);

    /* PMG struct */
    image = create_pgm(input);
    if (!image) {
        printf("ERROR: Out of memory!\n");
        return EXIT_FAILURE;
    }

    /* Checking if the input is binary */
    check_binary_input(image);

    /* CCL Algorithm, image data is coloured after this */
    if (!run_ccl_algo(image->data, image->width, image->height)) {
        printf("ERROR: Error occurred while running the main algorithm\n");
        return EXIT_FAILURE;
    }

    /* Output image, result */
    if (!write_pgm_file(image, result)) {
        printf("ERROR: Error occurred while creating output file!\n");
        return EXIT_FAILURE;
    }

    /* Free */
    myfree((void **) &input);
    myfree((void **) &result);
    free_pgm(&image);
    /*printf("Allocated memory blocks at the end: %d\n", mem_blocks);*/

    return EXIT_SUCCESS;
}