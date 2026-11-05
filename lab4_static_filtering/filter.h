#ifndef FILTER_H
#define FILTER_H

/* Laboratory work 4: filtering of a static 2D array. Variant 10. */

#define N 10 /* rows of the image matrix Y / X */
#define M 10 /* columns of the image matrix Y / X */
#define K 3  /* rows of the filter mask A (odd, < N) */
#define L 3  /* columns of the filter mask A (odd, < M) */

/* --- input helpers (implemented in init_array.c) --- */
int filter_read_line(char *buffer, int size);
int filter_read_int(const char *prompt, int min, int max, int *out);
int filter_read_double(const char *prompt, double min, double max,
                       double *out);
int filter_read_values(const char *prompt, int *values, int count, int min,
                       int max);

/* --- elementary tasks, one per source file --- */
void init_array(int Y[N][M]);
void print_array(const int *data, int rows, int cols, const char *title);
void init_filter(int A[K][L]);
void weighted_filter(int Y[N][M], int A[K][L], int X[N][M]);
void impulse_noise(int Y[N][M], double percent, int mode);
void median_filter(int Y[N][M]);

#endif /* FILTER_H */
