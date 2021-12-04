# Connected-Component Labeling - Coloring of Connected Areas in an Image

## Project Description
This project implements a console application in ANSI C that performs coloring of connected areas in a binary digital image (containing only black and white pixels) using the Connected-Component Labeling (CCL) algorithm from the field of computer vision.

## Functionality
- **Loading binary image**: From an input file in Portable Gray Map (PGM) format.
- **Coloring connected areas**: Using the two-pass CCL algorithm.
- **Binary image validation**: Ensures the input image contains only black (0x00) and white (0xFF) pixels.
- **Color equivalence table**: Creates a color equivalence table to resolve color conflicts during coloring.
- **Saving the colored image**: To an output file in PGM format.

## Algorithm
### 1st Pass
1. Traverse the image row by row.
2. Assign a value to each non-zero pixel (representing an object) at coordinates `[i, j]` based on the values of its neighboring pixels (see the mask in the documentation).
   - If all neighboring pixels are part of the background (value `0x00`), assign a new, unassigned value (a new color) to pixel `[i, j]`.
   - If one neighboring pixel has a non-zero value, assign the non-zero neighboring pixel's value to the pixel being colored.
   - If multiple neighboring pixels have non-zero values, assign the value of any non-zero pixel from the examined neighborhood. If the pixel values in the mask are different (i.e., a color collision occurred), record the equivalence of the pixel values in the color equivalence table.

### 2nd Pass
1. Traverse the entire image again row by row.
2. Recolor the pixels of the areas based on the color equivalence table (gathered during the 1st pass) to ensure each area has a unique color not shared by any other area.

## PGM File Format

	P5 width height max_value pixel_data

- `P5`: Magic number for binary PGM file.
- `width`: Number of columns in the image.
- `height`: Number of rows in the image.
- `max_value`: Maximum pixel value (typically 255).
- `pixel_data`: Byte representing the pixel value (0x00 for black, 0xFF for white).

## Implementation
- **Language**: ANSI C
- **Libraries**: Standard C libraries
- **pgm.h**: Includes the structure for representing PGM images (width, height, pixel data) and functions for loading, saving, and validating PGM files.
- **disj_set.h**: Implements a disjoint set (Union-Find) data structure for tracking connected components.
- **ccl.h**: Contains the main algorithm for Connected-Component Labeling.
- **mem.h**: Includes utility functions for memory management.
- **main.c**: Contains the main function of the program and processes command-line arguments.

## How to Run

### Compilation
For Linux:

	gcc -o ccl main.c pgm.c disj_set.c ccl.c mem.c -Wall -Wextra

For Windows (using MinGW):

	gcc -o ccl.exe main.c pgm.c disj_set.c ccl.c mem.c -Wall -Wextra


### Running the Program

	ccl <input-file.pgm> <output-file.pgm>

- `<input-file.pgm>`: Input file containing the binary image in PGM format.
- `<output-file.pgm>`: Output file where the colored image will be saved.
