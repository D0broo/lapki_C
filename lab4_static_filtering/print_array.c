/*
 * Друк двовимірного масиву з довільною кількістю рядків і стовпців.
 * Масив передається плоским покажчиком (рядковий порядок), тому одна
 * функція друкує і Y[N][M], і маску A[K][L].
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
