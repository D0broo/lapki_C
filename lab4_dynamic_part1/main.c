/*
 * Laboratory work 4, part 1: one-dimensional dynamic arrays.
 * Variant 10.
 *
 * Build: gcc -std=c11 -Wall -Wextra -Werror -pedantic -o lab4_1.exe main.c
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------- prototypes -- */

/* Required public interface (definitions are below). */
void fill_int_array(unsigned int *A, unsigned int Size);
void fill_float_array(float *B, unsigned int Size);
void print_int_array(const unsigned int *uiptr, unsigned int size_of_array);
void print_float_array(const float *fptr, unsigned int size_of_array);
unsigned int sum(const unsigned int *uiptr, unsigned int size_of_array);
double product(const float *fptr, unsigned int size_of_array);
double mean_value(const unsigned int *A, unsigned int Size);
unsigned int find_min(const unsigned int *A, unsigned int Size);
float find_max(const float *B, unsigned int Size);

/* Internal input helpers. */
static int read_line(char *buffer, size_t size);
static int read_uint(const char *prompt, unsigned int min, unsigned int max,
                     unsigned int *value);

/* ---------------------------------------------------------------- input -- */

/* Read one line from stdin; return 0 on EOF. A trailing newline is removed. */
static int read_line(char *buffer, size_t size)
{
    size_t length;
    int ch;

    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }
    length = strlen(buffer);
    if (length > 0U && buffer[length - 1U] == '\n') {
        buffer[length - 1U] = '\0';
    } else {
        /* The line was longer than the buffer: discard the remainder. */
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
    return 1;
}

/* Read an unsigned integer in [min; max]; return 0 on EOF. */
static int read_uint(const char *prompt, unsigned int min, unsigned int max,
                     unsigned int *value)
{
    char line[128];

    for (;;) {
        char *end = NULL;
        long parsed;

        printf("%s", prompt);
        fflush(stdout);
        if (!read_line(line, sizeof line)) {
            return 0;
        }
        if (line[0] == '\0') {
            printf("Empty input; enter a number in [%u; %u].\n", min, max);
            continue;
        }
        errno = 0;
        parsed = strtol(line, &end, 10);
        while (end != NULL && (*end == ' ' || *end == '\t')) {
            ++end;
        }
        if (errno != 0 || end == line || end == NULL || *end != '\0' ||
            parsed < (long)min || parsed > (long)max) {
            printf("Invalid input; enter a number in [%u; %u].\n", min, max);
            continue;
        }
        *value = (unsigned int)parsed;
        return 1;
    }
}

/* A[j] = Size - (j + 1) */
void fill_int_array(unsigned int *A, unsigned int Size)
{
    unsigned int j;

    for (j = 0U; j < Size; ++j) {
        A[j] = Size - (j + 1U);
    }
}

/* B[j] = 5 * j + 5 */
void fill_float_array(float *B, unsigned int Size)
{
    unsigned int j;

    for (j = 0U; j < Size; ++j) {
        B[j] = 5.0f * (float)j + 5.0f;
    }
}

void print_int_array(const unsigned int *uiptr, unsigned int size_of_array)
{
    unsigned int j;

    printf("Array A (%u element(s)):\n", size_of_array);
    for (j = 0U; j < size_of_array; ++j) {
        printf("  A[%u] = %u\n", j, uiptr[j]);
    }
}

void print_float_array(const float *fptr, unsigned int size_of_array)
{
    unsigned int j;

    printf("Array B (%u element(s)):\n", size_of_array);
    for (j = 0U; j < size_of_array; ++j) {
        printf("  B[%u] = %.2f\n", j, (double)fptr[j]);
    }
}

unsigned int sum(const unsigned int *uiptr, unsigned int size_of_array)
{
    unsigned int j;
    unsigned int total = 0U;

    for (j = 0U; j < size_of_array; ++j) {
        total += uiptr[j];
    }
    return total;
}

double product(const float *fptr, unsigned int size_of_array)
{
    unsigned int j;
    double result = 1.0;

    for (j = 0U; j < size_of_array; ++j) {
        result *= (double)fptr[j];
    }
    return result;
}

double mean_value(const unsigned int *A, unsigned int Size)
{
    return (double)sum(A, Size) / (double)Size;
}

unsigned int find_min(const unsigned int *A, unsigned int Size)
{
    unsigned int j;
    unsigned int minimum = A[0];

    for (j = 1U; j < Size; ++j) {
        if (A[j] < minimum) {
            minimum = A[j];
        }
    }
    return minimum;
}

float find_max(const float *B, unsigned int Size)
{
    unsigned int j;
    float maximum = B[0];

    for (j = 1U; j < Size; ++j) {
        if (B[j] > maximum) {
            maximum = B[j];
        }
    }
    return maximum;
}

int main(void)
{
    unsigned int Size = 0U;
    unsigned int *A = NULL;
    float *B = NULL;
    unsigned int total_int;
    double total_float;
    double average_value;
    unsigned int min_value;
    float max_value;

    printf("Laboratory work 4, part 1 (Variant 10).\n");
    if (!read_uint("Enter array size (1..1000000, e.g. 10): ", 1U, 1000000U,
                   &Size)) {
        printf("End of input.\n");
        return 0;
    }

    A = (unsigned int *)calloc((size_t)Size, sizeof *A);
    B = (float *)malloc((size_t)Size * sizeof *B);
    if (A == NULL || B == NULL) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        free(A);
        free(B);
        return 1;
    }

    fill_int_array(A, Size);
    fill_float_array(B, Size);

    print_int_array(A, Size);
    print_float_array(B, Size);

    total_int = sum(A, Size);
    total_float = product(B, Size);
    average_value = mean_value(A, Size);
    min_value = find_min(A, Size);
    max_value = find_max(B, Size);

    printf("\nResults:\n");
    printf("  sum(A)        = %u\n", total_int);
    printf("  product(B)    = %.6e\n", total_float);
    printf("  mean_value(A) = %.6f\n", average_value);
    printf("  find_min(A)   = %u\n", min_value);
    printf("  find_max(B)   = %.2f\n", (double)max_value);

    free(A);
    free(B);
    return 0;
}
