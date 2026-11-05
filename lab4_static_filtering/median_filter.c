/*
 * 3x3 median filtering of Y[N][M] in place.
 * A full copy of Y is taken first, so every output cell is computed from
 * the pre-filtered (noisy) values. At borders and corners only valid
 * in-bound neighbours are collected; their median is used.
 */

#include "filter.h"

void median_filter(int Y[N][M])
{
    int source[N][M];
    int i;
    int j;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            source[i][j] = Y[i][j];
        }
    }

    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            int values[9];
            int count = 0;
            int di;
            int dj;

            for (di = -1; di <= 1; ++di) {
                int row = i + di;

                if (row < 0 || row >= N) {
                    continue;
                }
                for (dj = -1; dj <= 1; ++dj) {
                    int col = j + dj;
                    int p;
                    int value;

                    if (col < 0 || col >= M) {
                        continue;
                    }
                    value = source[row][col];
                    p = count;
                    while (p > 0 && values[p - 1] > value) {
                        values[p] = values[p - 1];
                        --p;
                    }
                    values[p] = value;
                    ++count;
                }
            }
            Y[i][j] = values[count / 2];
        }
    }
}
