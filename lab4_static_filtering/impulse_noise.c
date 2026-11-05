/*
 * Імпульсний шум на Y[N][M].
 * Пошкоджується count = (int)((N * M * percent) / 100.0 + 0.5) клітинок;
 * кожна клітинка — не більше одного разу (контроль через логічну маску).
 * Режим 1 -> 0, режим 2 -> 255, режим 3 -> випадкове 0..255.
 */

#include <stdio.h>
#include <stdlib.h>

#include "filter.h"

void impulse_noise(int Y[N][M], double percent, int mode)
{
    int corrupted[N][M];
    int target;
    int done = 0;
    int i;
    int j;

    if (percent < 0.0) {
        percent = 0.0;
    }
    if (percent > 100.0) {
        percent = 100.0;
    }
    target = (int)(((double)(N * M) * percent) / 100.0 + 0.5);
    if (target > N * M) {
        target = N * M;
    }

    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            corrupted[i][j] = 0;
        }
    }

    while (done < target) {
        int row = rand() % N;
        int col = rand() % M;

        if (corrupted[row][col] != 0) {
            continue;
        }
        corrupted[row][col] = 1;
        if (mode == 1) {
            Y[row][col] = 0;
        } else if (mode == 2) {
            Y[row][col] = 255;
        } else {
            Y[row][col] = rand() % 256;
        }
        ++done;
    }

    printf("Impulse noise: %d of %d cells corrupted (%.2f%%), mode %d.\n",
           target, N * M, percent, mode);
}
