/*
 * Медіанна фільтрація 3x3 масиву Y[N][M] на місці.
 * Спочатку робиться повна копія Y, тому кожна вихідна клітинка
 * обчислюється з попередніх (зашумлених) значень. На межах і в кутах
 * збираються лише дійсні сусіди в межах масиву; береться їхня медіана.
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
