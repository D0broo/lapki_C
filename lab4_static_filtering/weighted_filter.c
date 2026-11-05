/*
 * Зважена фільтрація Y[N][M] маскою A[K][L] у X[N][M].
 * Y не змінюється. На краях і в кутах враховуються лише дійсні сусіди
 * в межах масиву та відповідні активні ваги маски; результат
 * обмежується діапазоном 0..255.
 */

#include <stdio.h>

#include "filter.h"

void weighted_filter(int Y[N][M], int A[K][L], int X[N][M])
{
    const int half_k = K / 2;
    const int half_l = L / 2;
    int i;
    int j;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            long long weighted_sum = 0;
            long long weight_sum = 0;
            int di;
            int dj;

            for (di = -half_k; di <= half_k; ++di) {
                int row = i + di;

                if (row < 0 || row >= N) {
                    continue;
                }
                for (dj = -half_l; dj <= half_l; ++dj) {
                    int col = j + dj;
                    int weight;

                    if (col < 0 || col >= M) {
                        continue;
                    }
                    weight = A[di + half_k][dj + half_l];
                    weighted_sum += (long long)Y[row][col] * weight;
                    weight_sum += weight;
                }
            }
            if (weight_sum > 0) {
                double value = (double)weighted_sum / (double)weight_sum;

                if (value < 0.0) {
                    value = 0.0;
                }
                if (value > 255.0) {
                    value = 255.0;
                }
                X[i][j] = (int)(value + 0.5);
            } else {
                X[i][j] = Y[i][j];
            }
        }
    }
    printf("Weighted filtering finished: Y is unchanged, the result is in X.\n");
}
