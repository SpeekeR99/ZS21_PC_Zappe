#include <stdio.h>
#include <stdlib.h>
#include "ccl.h"
#include "disj_set.h"

/**
 * First row of pixels, needs to be done because of the shape of the mask
 * @param data Data from pgm struct, actual image pixels
 * @param width Width of the image
 * @param disj_sets Array of disjoint set data structures
 * @param next_label Labeling the pixels, for disjoint sets
 */
void first_row(const byte *data, uint width, disj_set_element **disj_sets, uint *next_label) {
    uint x;

    for (x = 1; x < width; x++) {
        if (data[x] != 0x00) {
            if (data[x - 1] != 0x00) {
                disj_sets[x] = disj_set_make_set(disj_sets[x - 1]->value);
                disj_set_union(disj_sets[x], disj_sets[x - 1]);
            } else {
                disj_sets[x] = disj_set_make_set(*next_label);
                (*next_label)++;
            }
        }
    }
}

/**
 * First column of pixels, needs to be done because of the shape of the mask
 * @param data Data from pgm struct, actual image pixels
 * @param width Width of the image
 * @param height Height of the image
 * @param disj_sets Array of disjoint set data structures
 * @param next_label Labeling the pixels, for disjoint sets
 */
void first_column(const byte *data, uint width, uint height, disj_set_element **disj_sets, uint *next_label) {
    uint y, curr_index;

    for (y = 1; y < height; y++) {
        curr_index = y * width;
        if (data[curr_index] != 0x00) {
            if (data[curr_index - width] != 0x00) {
                disj_sets[curr_index] = disj_set_make_set(disj_sets[curr_index - width]->value);
                disj_set_union(disj_sets[curr_index], disj_sets[curr_index - width]);
            } else {
                disj_sets[curr_index] = disj_set_make_set(*next_label);
                (*next_label)++;
            }
        }
    }
}

/**
 * Last column of pixels, needs to be done because of the shape of the mask
 * @param data Data from pgm struct, actual image pixels
 * @param width Width of the image
 * @param height Height of the image
 * @param disj_sets DArray of disjoint set data structures
 * @param next_label Labeling the pixels, for disjoint sets
 */
void last_column(const byte *data, uint width, uint height, disj_set_element **disj_sets, uint *next_label) {
    uint y, curr_index;

    for (y = 1; y < height; y++) {
        curr_index = y * width + width - 1;
        if (data[curr_index] != 0x00) {
            if (data[curr_index - width] != 0x00) {
                disj_sets[curr_index] = disj_set_make_set(disj_sets[curr_index - width]->value);
                disj_set_union(disj_sets[curr_index], disj_sets[curr_index - width]);
            } else {
                disj_sets[curr_index] = disj_set_make_set(*next_label);
                (*next_label)++;
            }
        }
    }
}

/**
 * Creates sets for the pre-required pixels (which are pre-required due to the shape of the mask)
 * First pixels is needed first, then first row and first column can be done, then the last column
 * @param data Data from pgm struct, actual image pixels
 * @param width Width of the image
 * @param height Height of the image
 * @param disj_sets Array of disjoint set data structures
 * @param next_label Labeling the pixels, for disjoint sets
 */
void prereq_pixels(const byte *data, uint width, uint height, disj_set_element **disj_sets, uint *next_label) {
    // First pixel
    if (data[0] != 0x00) {
        disj_sets[0] = disj_set_make_set(*next_label);
        (*next_label)++;
    }

    // First row of pixels
    first_row(data, width, disj_sets, next_label);

    // First column of pixels
    first_column(data, width, height, disj_sets, next_label);

    // Last column of pixels
    last_column(data, width, height, disj_sets, next_label);
}

/**
 * Does the first pass of the algorithm
 * Passes through the pre-required pixels (due to the shape of the mask)
 * then through every other pixel
 * Checks every pixel, if it's not background and some of the neighbours (mask) is also not background
 * takes his set value as his own and unions himself with said neighbour. If there is more neighbours
 * unions them all, so those disjoint sets become one. If there's only background neighbours creates new set
 * @param data Data from pgm struct, actual image pixels
 * @param width Width of the image
 * @param height Height of the image
 * @param disj_sets Array of disjoint set data structures
 * @param next_label Labeling the pixels, for disjoint sets
 */
void first_pass(const byte *data, uint width, uint height, disj_set_element **disj_sets, uint *next_label) {
    // Init
    uint x, y, curr_index, left_index, top_left_index, top_index, top_right_index;
    disj_set_element *left_el = NULL, *top_left_el = NULL, *top_el = NULL, *top_right_el = NULL;

    // Pre-required first few pixels, for the mask shape
    prereq_pixels(data, width, height, disj_sets, next_label);

    // All the other pixels
    for (y = 1; y < height; y++) {
        for (x = 1; x < width - 1; x++) {
            curr_index = x + y * width;
            if (data[curr_index] != 0x00) {
                left_index = curr_index - 1;
                top_left_index = curr_index - 1 - width;
                top_index = curr_index - width;
                top_right_index = curr_index + 1 - width;
                if (data[left_index] != 0x00 || data[top_left_index] != 0x00 || data[top_index] != 0x00 ||
                    data[top_right_index] != 0x00) {
                    left_el = disj_sets[left_index];
                    top_left_el = disj_sets[top_left_index];
                    top_el = disj_sets[top_index];
                    top_right_el = disj_sets[top_right_index];
                    if (left_el) {
                        disj_sets[curr_index] = disj_set_make_set(left_el->value);
                        disj_set_union(disj_sets[curr_index], left_el);
                    } else if (top_left_el) {
                        disj_sets[curr_index] = disj_set_make_set(top_left_el->value);
                        disj_set_union(disj_sets[curr_index], top_left_el);
                    } else if (top_el) {
                        disj_sets[curr_index] = disj_set_make_set(top_el->value);
                        disj_set_union(disj_sets[curr_index], top_el);
                    } else {
                        disj_sets[curr_index] = disj_set_make_set(top_right_el->value);
                        disj_set_union(disj_sets[curr_index], top_right_el);
                    }
                    disj_set_union(left_el, top_left_el);
                    disj_set_union(top_left_el, top_el);
                    disj_set_union(top_el, top_right_el);
                } else {
                    disj_sets[curr_index] = disj_set_make_set(*next_label);
                    (*next_label)++;
                }
            }
        }
    }
}

/**
 * Counts how many unique distinct components there are in the picture
 * based on how many disjoint sets are there in the disj_sets array
 * @param uniques Array of unique set values
 * @param width Width of the image
 * @param height Height of the image
 * @param disj_sets Array of disjoint set data structures
 * @return Number of distinct unique components in the picture
 */
uint find_unique_components(uint *uniques, uint width, uint height, disj_set_element **disj_sets) {
    // Init
    uint x, y, i, curr_index, continue_now, unique = 0;
    disj_set_element *curr_el;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            continue_now = 0;
            curr_index = x + y * width;
            curr_el = disj_set_find(disj_sets[curr_index]);
            if (curr_el) {
                for (i = 0; i < 255; i++) {
                    if (curr_el->value == uniques[i]) {
                        continue_now = 1;
                        break;
                    }
                }
                if (continue_now) {
                    continue;
                }
                uniques[unique] = curr_el->value;
                unique++;
                if (unique > 255) {
                    perror("ERROR: There is more than 255 distinct components in the picture!\n");
                    return FAILURE;
                }
            }
        }
    }

    return unique;
}

/**
 * Second pass of the algo goes through every pixels and checks it's corresponding set
 * from the array of disjoint sets. Calling disj_set_find() function above every
 * pixel (more like same index in the array disj_sets and data) we can get value of parent
 * of each dataset. Colours array is the same size as array uniques. So we can index them the same,
 * meaning for each unique component value (in uniques, gotten from disj_set_find() ) we have
 * unique colour from colors array
 * Fills those colour value bytes back into the original picture (pgm struct)
 * @param data Original data from the input image (pgm struct)
 * @param width Width of the image
 * @param height Height of the image
 * @param disj_sets Array of disjoint set data structures
 * @param uniques Unique values of distinct components (disjoint sets)
 * @param colors Unique colors, same indexing as uniques
 */
void second_pass(byte *data, uint width, uint height, disj_set_element **disj_sets, const uint *uniques,
                 const byte *colors) {
    uint x, y, i, curr_index;
    disj_set_element *curr_el;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            curr_index = x + y * width;
            curr_el = disj_set_find(disj_sets[curr_index]);
            if (curr_el) {
                for (i = 0; i < 255; i++) {
                    if (curr_el->value == uniques[i]) {
                        data[curr_index] = colors[i];
                    }
                }
            }
        }
    }
}

/**
 * Main function of CCL, runs the whole algo
 * Using Disjoint set data structure analyzes the input binary image
 * finds distinct components using the 8-connectivity mask and
 * re-colors each component based on how many numbers of components are there in the image
 * @param data Input data, from the pgm struct
 * @param width Width of the image
 * @param height Height of the image
 * @return 1 if everything goes well, 0 otherwise
 */
int run_ccl_algo(byte *data, uint width, uint height) {
    // Init
    uint i, x, y, next_label = 1, curr_index, unique, *uniques = NULL;
    byte *colors = NULL;

    disj_set_element **disj_sets = NULL;

    // Sanity check
    if (!data) return FAILURE;

    // Ini of disjoint sets
    disj_sets = (disj_set_element **) calloc(width * height, sizeof(disj_set_element));
    if (!disj_sets) return FAILURE;

    // First pass
    first_pass(data, width, height, disj_sets, &next_label);

    // Find unique components, so colours can be equally distributed
    uniques = (uint *) calloc(255, sizeof(uint)); // 255 because there is 256 values in byte, 0 is background
    if (!uniques) return FAILURE;
    unique = find_unique_components(uniques, width, height, disj_sets);

    // Setup colors
    colors = (byte *) malloc(unique * sizeof(byte));
    if (!colors) return FAILURE;
    for (i = 0; i < unique; i++) {
        colors[i] = (i + 1) * 255 / unique;
    }

    // Second pass, assign colours
    second_pass(data, width, height, disj_sets, uniques, colors);

    // Free
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            curr_index = x + y * width;
            disj_set_element_free(&(disj_sets[curr_index]));
        }
    }
    free(disj_sets);
    free(uniques);
    free(colors);

    return SUCCESS;
}