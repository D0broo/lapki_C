#ifndef FILTER_H
#define FILTER_H

/* Лабораторна робота 4: фільтрація статичного двовимірного масиву. Варіант 10. */

#define N 10 /* рядки зображення Y / X */
#define M 10 /* стовпці зображення Y / X */
#define K 3  /* рядки маски фільтра A (непарне, < N) */
#define L 3  /* стовпці маски фільтра A (непарне, < M) */

/* --- функції введення (реалізовані в init_array.c) --- */
int filter_read_line(char *buffer, int size);
int filter_read_int(const char *prompt, int min, int max, int *out);
int filter_read_double(const char *prompt, double min, double max,
                       double *out);
int filter_read_values(const char *prompt, int *values, int count, int min,
                       int max);

/* --- елементарні завдання, по одному на файл --- */
void init_array(int Y[N][M]);
void print_array(const int *data, int rows, int cols, const char *title);
void init_filter(int A[K][L]);
void weighted_filter(int Y[N][M], int A[K][L], int X[N][M]);
void impulse_noise(int Y[N][M], double percent, int mode);
void median_filter(int Y[N][M]);

#endif /* FILTER_H */
