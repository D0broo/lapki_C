/*
 * Printing of a 2D array with an arbitrary number of rows and columns.
 * The array is passed as a flat pointer (row-major), so the same function
 * prints both Y[N][M] and the mask A[K][L].
 */

#include <stdio.h>

#include "filter.h"

void print_array(const int *data, int rows, int cols, const char *title)
{
    int i;
    int j;

    printf("%s (%d x %d):\n", title, rows, cols);
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            printf("%5d", data[i * cols + j]);
        }
        printf("\n");
    }
}
