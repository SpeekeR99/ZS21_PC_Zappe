#include <stdio.h>
#include <stdlib.h>
#include "ccl.h"
#include "disj_set.h"
#include "mem.h"

int run_ccl_algo(byte *data, uint width, uint height) {
    // Init
    uint i, x, y, next_label = 1, curr_index, left_index, top_left_index, top_index, top_right_index, continue_now, unique = 0, *uniques = NULL;
    byte *colors = NULL;
    disj_set_element *left_el = NULL, *top_left_el = NULL, *top_el = NULL, *top_right_el = NULL;
    disj_set_element **disj_sets = NULL;

    // Sanity check
    if (!data) return FAILURE;
    disj_sets = (disj_set_element **) calloc(width * height, sizeof(disj_set_element));
    if (!disj_sets) return FAILURE;

    // First pixel
    if (data[0] != 0x00) {
        disj_sets[0] = disj_set_make_set(next_label);
        next_label++;
    }

    // First row of pixels
    for (x = 1; x < width; x++) {
        if (data[x] != 0x00) {
            if (data[x-1] != 0x00) {
                disj_sets[x] = disj_set_make_set(disj_sets[x-1]->value);
                disj_set_union(disj_sets[x], disj_sets[x-1]);
            } else {
                disj_sets[x] = disj_set_make_set(next_label);
                next_label++;
            }
        }
    }

    // First column of pixels
    for (y = 1; y < height; y++) {
        curr_index = y * width;
        if (data[curr_index] != 0x00) {
            if (data[curr_index - width] != 0x00) {
                disj_sets[curr_index] = disj_set_make_set(disj_sets[curr_index - width]->value);
                disj_set_union(disj_sets[curr_index], disj_sets[curr_index - width]);
            } else {
                disj_sets[curr_index] = disj_set_make_set(next_label);
                next_label++;
            }
        }
    }

    // Last column of pixels
    for (y = 1; y < height; y++) {
        curr_index = y * width + width - 1;
        if (data[curr_index] != 0x00) {
            if (data[curr_index - width] != 0x00) {
                disj_sets[curr_index] = disj_set_make_set(disj_sets[curr_index - width]->value);
                disj_set_union(disj_sets[curr_index], disj_sets[curr_index - width]);
            } else {
                disj_sets[curr_index] = disj_set_make_set(next_label);
                next_label++;
            }
        }
    }

    // All the other pixels
    for (y = 1; y < height; y++) {
        for (x = 1; x < width - 1; x++) {
            curr_index = x + y * width;
            if (data[curr_index] != 0x00) {
                left_index = curr_index - 1;
                top_left_index = curr_index - 1 - width;
                top_index = curr_index - width;
                top_right_index = curr_index + 1 - width;
                if (data[left_index] != 0x00 || data[top_left_index] != 0x00 || data[top_index] != 0x00 || data[top_right_index] != 0x00) {
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
                    disj_sets[curr_index] = disj_set_make_set(next_label);
                    next_label++;
                }
            }
        }
    }

    // Find unique numbers, so colours can be equally distributed
    uniques = calloc(255, sizeof(uint)); // 255 because there is 256 values in byte, 0 is background
    if (!uniques) return FAILURE
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            continue_now = 0;
            curr_index = x + y * width;
            top_el = disj_set_find(disj_sets[curr_index]);
            if (top_el) {
                for (i = 0; i < 255; i++) {
                    if (top_el->value == uniques[i]) {
                        continue_now = 1;
                        break;
                    }
                }
                if (continue_now) {
                    continue;
                }
                uniques[unique] = top_el->value;
                unique++;
                if (unique > 255) {
                    perror("ERROR: There is more than 255 distinct components in the picture!\n");
                    return FAILURE;
                }
            }
        }
    }

    // Setup colors
    colors = malloc(unique * sizeof(byte));
    for (i = 0; i < unique; i++) {
        colors[i] = (i + 1) * 255 / unique;
    }

    // Second pass, assign colours
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            curr_index = x + y * width;
            top_el = disj_set_find(disj_sets[curr_index]);
            if (top_el) {
                for (i = 0; i < 255; i++) {
                    if (top_el->value == uniques[i]) {
                        data[curr_index] = colors[i];
                    }
                }
            }
        }
    }

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