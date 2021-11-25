#include <stdio.h>
#include <stdlib.h>
#include "ccl.h"
#include "mem.h"

void first_pass(byte *data, byte *labels, uint width, uint height) {
    uint x, y, curr_index;
    byte next_label = 0, left, top_left, top, top_right;

    for (y = 1; y < height; y++) {
        for (x = 1; x < width - 1; x++) {
            curr_index = x + y * width;
            if (data[curr_index] != 0x00) {
                left = labels[curr_index - 1];
                top_left = labels[curr_index - 1 - width];
                top = labels[curr_index - width];
                top_right = labels[curr_index + 1 - width];
                if (left != 0x00 || top_left != 0x00 || top != 0x00 || top_right != 0x00) {
                    if (left != 0x00) labels[curr_index] = left;
                    else if (top_left != 0x00) labels[curr_index] = top_left;
                    else if (top != 0x00) labels[curr_index] = top;
                    else labels[curr_index] = top_right;
                }
                else {
                    labels[curr_index] = next_label;
                    next_label++;
                }
            }
        }
    }
}

byte get_min(byte left, byte top_left, byte top, byte top_right) {
    byte min;

    return min;
}

void second_pass(byte *data, byte *labels, uint width, uint height) {
    uint x, y, curr_index, min;
    byte left, top_left, top, top_right;

//    for (y = 0; y < height; y++) {
//        for (x = 0; x < width; x++) {
//            curr_index = x + y * width;
//            if (data[curr_index] != 0x00) {
//                left = labels[curr_index - 1];
//                top_left = labels[curr_index - 1 - width];
//                top = labels[curr_index - width];
//                top_right = labels[curr_index + 1 - width];
//                min = get_min(left, top_left, top, top_right);
//                if (min) labels[curr_index] = min;
//            }
//        }
//    }

    data = labels;


}

int run_ccl_algo(byte *data, uint width, uint height) {
    // Sanity check
    if (!data) return FAILURE;
    byte *labels = (byte *) calloc(width * height, sizeof(byte));
    if (!labels) return FAILURE;

    first_pass(data, labels, width, height);
    second_pass(data, labels, width, height);

    free(labels);
    return SUCCESS;
}