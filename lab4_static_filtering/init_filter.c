/*
 * Ініціалізація маски фільтра A[K][L].
 * Лабораторна робота 4: фільтрація статичного масиву, Варіант 10.
 */

#include <stdio.h>
#include <stdlib.h>

#include "filter.h"

/* Публічний прототип (init_filter) оголошений у filter.h. */

/* ----------------------------------------------------------- прототипи -- */

/* Внутрішні допоміжні функції ініціалізації. */
static void filter_random(int A[K][L]);
static void filter_ones(int A[K][L]);
static void filter_keyboard(int A[K][L]);

/* ------------------------------------------------------------------ код -- */

static void filter_random(int A[K][L])
{
    int c;
    int d;
    int i;
    int j;

    if (!filter_read_int("c (lower weight, 1..5): ", 1, 5, &c)) {
        return;
    }
    if (!filter_read_int("d (upper weight, c..5): ", c, 5, &d)) {
        return;
    }
    for (i = 0; i < K; ++i) {
        for (j = 0; j < L; ++j) {
            A[i][j] = c + rand() % (d - c + 1);
        }
    }
    printf("Mask A filled with random weights in [%d; %d].\n", c, d);
}

static void filter_ones(int A[K][L])
{
    int i;
    int j;

    for (i = 0; i < K; ++i) {
        for (j = 0; j < L; ++j) {
            A[i][j] = 1;
        }
    }
    printf("Mask A filled with ones.\n");
}

static void filter_keyboard(int A[K][L])
{
    char prompt[64];
    int i;

    printf("Enter mask A row by row (weights 1..3).\n");
    for (i = 0; i < K; ++i) {
        (void)snprintf(prompt, sizeof prompt, "  Row %d/%d (%d int): ",
                       i + 1, K, L);
        if (!filter_read_values(prompt, A[i], L, 1, 3)) {
            return;
        }
    }
}

void init_filter(int A[K][L])
{
    int mode;

    printf("\nInitialization of mask A (%d x %d):\n", K, L);
    printf("  1 - random weights in [c; d], 1 <= c <= d <= 5\n");
    printf("  2 - all weights equal to 1\n");
    printf("  3 - keyboard input, validated strictly to 1..3\n");
    if (!filter_read_int("Mode: ", 1, 3, &mode)) {
        return;
    }
    if (mode == 1) {
        filter_random(A);
    } else if (mode == 2) {
        filter_ones(A);
    } else {
        filter_keyboard(A);
    }
}
