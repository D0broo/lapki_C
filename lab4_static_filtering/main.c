/*
 * Лабораторна робота 4: фільтрація статичного двовимірного масиву
 * (багатофайловий проєкт). Варіант 10: N = M = 10, маска K = L = 3.
 *
 * Порядок роботи: ініціалізувати Y, ініціалізувати маску A, застосувати
 * зважений фільтр, імпульсний шум і медіанний фільтр 3x3, друкуючи масиви
 * на кожному кроці.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "filter.h"

int main(void)
{
    int Y[N][M];
    int A[K][L];
    int X[N][M];
    int y_ready = 0;
    int filter_ready = 0;
    int x_ready = 0;
    int choice;

    srand((unsigned)time(NULL));

    printf("Laboratory work 4: static array filtering (Variant 10).\n");
    printf("Y and X are %d x %d, the mask A is %d x %d.\n", N, M, K, L);

    for (;;) {
        printf("\nMenu:\n");
        printf("  1 - initialize image matrix Y\n");
        printf("  2 - initialize filter mask A\n");
        printf("  3 - weighted filtering: X = filter(Y, A)\n");
        printf("  4 - add impulse noise to Y\n");
        printf("  5 - 3x3 median filtering of Y (in place)\n");
        printf("  6 - print Y, A and X\n");
        printf("  0 - exit\n");
        if (!filter_read_int("Choice: ", 0, 6, &choice)) {
            break;
        }
        if (choice == 0) {
            break;
        }

        switch (choice) {
        case 1:
            init_array(Y);
            y_ready = 1;
            printf("\n");
            print_array(&Y[0][0], N, M, "Y (initial)");
            break;
        case 2:
            init_filter(A);
            filter_ready = 1;
            printf("\n");
            print_array(&A[0][0], K, L, "A (filter mask)");
            break;
        case 3:
            if (!y_ready || !filter_ready) {
                printf("Initialize Y (option 1) and A (option 2) first.\n");
                break;
            }
            weighted_filter(Y, A, X);
            x_ready = 1;
            printf("\n");
            print_array(&X[0][0], N, M, "X (weighted filter result)");
            break;
        case 4: {
            double percent;
            int mode;

            if (!y_ready) {
                printf("Initialize Y (option 1) first.\n");
                break;
            }
            if (!filter_read_double("Percent of corrupted cells (0..100): ",
                                    0.0, 100.0, &percent)) {
                goto done;
            }
            printf("Noise mode: 1 -> 0, 2 -> 255, 3 -> random 0..255\n");
            if (!filter_read_int("Mode: ", 1, 3, &mode)) {
                goto done;
            }
            impulse_noise(Y, percent, mode);
            printf("\n");
            print_array(&Y[0][0], N, M, "Y (after impulse noise)");
            break;
        }
        case 5:
            if (!y_ready) {
                printf("Initialize Y (option 1) first.\n");
                break;
            }
            median_filter(Y);
            printf("3x3 median filtering finished (Y updated in place).\n\n");
            print_array(&Y[0][0], N, M, "Y (after median filter)");
            break;
        case 6:
            if (!y_ready) {
                printf("Y is not initialized yet.\n");
            } else {
                printf("\n");
                print_array(&Y[0][0], N, M, "Y");
            }
            if (!filter_ready) {
                printf("A is not initialized yet.\n");
            } else {
                printf("\n");
                print_array(&A[0][0], K, L, "A");
            }
            if (!x_ready) {
                printf("X is not computed yet.\n");
            } else {
                printf("\n");
                print_array(&X[0][0], N, M, "X");
            }
            break;
        default:
            break;
        }
    }

done:
    printf("Goodbye.\n");
    return 0;
}
