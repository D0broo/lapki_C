/*
 * Laboratory work 5: two-dimensional dynamic real matrices (double **).
 * Variant 10 (even): the minimum of A is searched STRICTLY BELOW the main
 * diagonal (i > j); the maximum is searched over the whole matrix.
 *
 * Build: gcc -std=c11 -Wall -Wextra -Werror -pedantic -o lab5.exe main.c
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DIM_MIN (1)
#define DIM_MAX (20)

/* ---------------------------------------------------------------- input -- */

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
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
    return 1;
}

static int read_int_line(const char *prompt, int min, int max, int *out)
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
            printf("Empty input; enter an integer in [%d; %d].\n", min, max);
            continue;
        }
        errno = 0;
        parsed = strtol(line, &end, 10);
        while (end != NULL && (*end == ' ' || *end == '\t')) {
            ++end;
        }
        if (errno != 0 || end == line || end == NULL || *end != '\0' ||
            parsed < (long)min || parsed > (long)max) {
            printf("Invalid input; enter an integer in [%d; %d].\n", min, max);
            continue;
        }
        *out = (int)parsed;
        return 1;
    }
}

static int read_int_default(const char *prompt, int def, int min, int max,
                            int *out)
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
            *out = def;
            return 1;
        }
        errno = 0;
        parsed = strtol(line, &end, 10);
        while (end != NULL && (*end == ' ' || *end == '\t')) {
            ++end;
        }
        if (errno != 0 || end == line || end == NULL || *end != '\0' ||
            parsed < (long)min || parsed > (long)max) {
            printf("Invalid input; enter an integer in [%d; %d].\n", min, max);
            continue;
        }
        *out = (int)parsed;
        return 1;
    }
}

static int read_double_values(const char *prompt, double *values, int count)
{
    char line[4096];

    for (;;) {
        int read = 0;
        int bad = 0;
        char *p;

        printf("%s", prompt);
        fflush(stdout);
        if (!read_line(line, sizeof line)) {
            return 0;
        }
        p = line;
        while (read < count) {
            char *end = NULL;
            double parsed;

            while (*p == ' ' || *p == '\t') {
                ++p;
            }
            if (*p == '\0') {
                break;
            }
            errno = 0;
            parsed = strtod(p, &end);
            if (end == p || errno != 0 || parsed < -1.0e9 ||
                parsed > 1.0e9) {
                bad = 1;
                break;
            }
            values[read++] = parsed;
            p = end;
        }
        while (*p == ' ' || *p == '\t') {
            ++p;
        }
        if (!bad && read == count && *p == '\0') {
            return 1;
        }
        printf("Enter exactly %d real number(s).\n", count);
    }
}

/* --------------------------------------------------------------- memory -- */

void clear_memory(double **matrix, int rows)
{
    int i;

    if (matrix == NULL) {
        return;
    }
    for (i = 0; i < rows; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

double **memory_allocation(int rows, int cols)
{
    double **matrix;
    int i;

    if (rows <= 0 || cols <= 0) {
        return NULL;
    }
    matrix = (double **)malloc((size_t)rows * sizeof *matrix);
    if (matrix == NULL) {
        return NULL;
    }
    for (i = 0; i < rows; ++i) {
        matrix[i] = (double *)malloc((size_t)cols * sizeof *matrix[i]);
        if (matrix[i] == NULL) {
            clear_memory(matrix, i);
            return NULL;
        }
    }
    return matrix;
}

/* ----------------------------------------------------------- printing ---- */

static void print_matrix(double **matrix, int rows, int cols, const char *name)
{
    int i;
    int j;

    printf("Matrix %s (%d x %d):\n", name, rows, cols);
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            printf("%10.3f", matrix[i][j]);
        }
        printf("\n");
    }
}

/* ------------------------------------------------------- initialization -- */

static int fill_keyboard(double **matrix, int rows, int cols,
                         const char *name)
{
    char prompt[128];
    int i;

    printf("Enter matrix %s row by row.\n", name);
    for (i = 0; i < rows; ++i) {
        (void)snprintf(prompt, sizeof prompt, "  Row %d/%d (%d real): ",
                       i + 1, rows, cols);
        if (!read_double_values(prompt, matrix[i], cols)) {
            return 0;
        }
    }
    return 1;
}

static void fill_formula(double **matrix, int rows, int cols, int kind)
{
    int i;
    int j;

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            matrix[i][j] = (kind == 0) ? (double)(i + j)
                                       : (double)(i + j) + 0.5;
        }
    }
}

static void fill_random(double **matrix, int rows, int cols)
{
    int i;
    int j;

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            matrix[i][j] = ((double)rand() / (double)RAND_MAX) * 100.0 - 50.0;
        }
    }
}

/* 1 = ready, 0 = end of input, -1 = allocation failure */
static int init_matrix(double ***target, int rows, int cols, const char *name)
{
    int mode;
    double **matrix;

    printf("\nInitialization of matrix %s (%d x %d):\n", name, rows, cols);
    printf("  1 - keyboard input\n");
    printf("  2 - formula (%s[i][j] = %s)\n", name,
           (name[0] == 'A') ? "i + j" : "i + j + 0.5");
    printf("  3 - random real values in [-50; 50]\n");
    if (!read_int_line("Mode: ", 1, 3, &mode)) {
        return 0;
    }

    matrix = memory_allocation(rows, cols);
    if (matrix == NULL) {
        fprintf(stderr, "Error: memory allocation failed for matrix %s.\n",
                name);
        return -1;
    }

    if (mode == 1) {
        if (!fill_keyboard(matrix, rows, cols, name)) {
            clear_memory(matrix, rows);
            return 0;
        }
    } else if (mode == 2) {
        fill_formula(matrix, rows, cols, (name[0] == 'A') ? 0 : 1);
    } else {
        fill_random(matrix, rows, cols);
    }

    *target = matrix;
    return 1;
}

/* ------------------------------------------------------------ the tasks -- */

/* Maximum over the whole matrix A and minimum strictly below the diagonal. */
static void max_and_min_below(double **A, int na)
{
    double max_value = A[0][0];
    int max_i = 0;
    int max_j = 0;
    int i;
    int j;

    for (i = 0; i < na; ++i) {
        for (j = 0; j < na; ++j) {
            if (A[i][j] > max_value) {
                max_value = A[i][j];
                max_i = i;
                max_j = j;
            }
        }
    }
    printf("Maximum over the whole matrix A: %.6f at A[%d][%d]\n",
           max_value, max_i, max_j);

    if (na < 2) {
        printf("Minimum below the main diagonal is undefined: na = %d.\n",
               na);
        return;
    }
    {
        double min_value = A[1][0];
        int min_i = 1;
        int min_j = 0;

        for (i = 1; i < na; ++i) {
            for (j = 0; j < i; ++j) {
                if (A[i][j] < min_value) {
                    min_value = A[i][j];
                    min_i = i;
                    min_j = j;
                }
            }
        }
        printf("Minimum strictly below the main diagonal: %.6f at A[%d][%d]\n",
               min_value, min_i, min_j);
    }
}

static double **transpose_b(double **B, int nb, int mb)
{
    double **transposed;
    int i;
    int j;

    transposed = memory_allocation(mb, nb);
    if (transposed == NULL) {
        fprintf(stderr, "Error: memory allocation failed for the transpose.\n");
        return NULL;
    }
    for (i = 0; i < nb; ++i) {
        for (j = 0; j < mb; ++j) {
            transposed[j][i] = B[i][j];
        }
    }
    return transposed;
}

static double **mult_a_b(double **A, int na, double **B, int nb, int mb)
{
    double **C;
    int i;
    int j;
    int k;

    if (na != nb) {
        printf("Error: A x B is impossible: na = %d but nb = %d.\n", na, nb);
        return NULL;
    }
    C = memory_allocation(na, mb);
    if (C == NULL) {
        fprintf(stderr, "Error: memory allocation failed for C.\n");
        return NULL;
    }
    for (i = 0; i < na; ++i) {
        for (j = 0; j < mb; ++j) {
            double total = 0.0;

            for (k = 0; k < na; ++k) {
                total += A[i][k] * B[k][j];
            }
            C[i][j] = total;
        }
    }
    return C;
}

/* Mandatory signature: sorts one row (a 1D array of `size` doubles). */
void sort_row(double *row, int size)
{
    int i;

    for (i = 1; i < size; ++i) {
        double key = row[i];
        int j = i - 1;

        while (j >= 0 && row[j] > key) {
            row[j + 1] = row[j];
            --j;
        }
        row[j + 1] = key;
    }
}

static void row_and_column_sums(double **A, int na, double **B, int nb, int mb)
{
    int i;
    int j;

    printf("\nRow sums of A (%d x %d):\n", na, na);
    for (i = 0; i < na; ++i) {
        double total = 0.0;

        for (j = 0; j < na; ++j) {
            total += A[i][j];
        }
        printf("  row %2d: %.6f\n", i, total);
    }
    printf("Column sums of B (%d x %d):\n", nb, mb);
    for (j = 0; j < mb; ++j) {
        double total = 0.0;

        for (i = 0; i < nb; ++i) {
            total += B[i][j];
        }
        printf("  col %2d: %.6f\n", j, total);
    }
}

static void wait_for_enter(void)
{
    char line[128];

    printf("\nPress Enter to continue...");
    fflush(stdout);
    (void)read_line(line, sizeof line);
}

/* ---------------------------------------------------------------- setup -- */

/* 1 = ready, 0 = end of input, -1 = allocation failure */
static int setup_matrices(double ***pA, int *pna, double ***pB, int *pnb,
                          int *pmb)
{
    int na;
    int nb;
    int mb;
    int rc;

    printf("\n--- Matrix dimensions ---\n");
    if (!read_int_default("na (square A, 2..20, default 10): ",
                          10, 2, DIM_MAX, &na)) {
        return 0;
    }
    if (!read_int_default("nb (rows of B, 1..20, default 10): ",
                          10, DIM_MIN, DIM_MAX, &nb)) {
        return 0;
    }
    if (!read_int_default("mb (columns of B, 1..20, default 5): ",
                          5, DIM_MIN, DIM_MAX, &mb)) {
        return 0;
    }

    rc = init_matrix(pA, na, na, "A");
    if (rc <= 0) {
        return rc;
    }
    rc = init_matrix(pB, nb, mb, "B");
    if (rc <= 0) {
        clear_memory(*pA, na);
        *pA = NULL;
        return rc;
    }
    *pna = na;
    *pnb = nb;
    *pmb = mb;
    return 1;
}

/* ----------------------------------------------------------------- main -- */

int main(void)
{
    double **A = NULL;
    double **B = NULL;
    int na = 0;
    int nb = 0;
    int mb = 0;
    int rc;
    int choice;

    srand((unsigned)time(NULL));

    printf("Laboratory work 5 (Variant 10, even).\n");
    printf("A is na x na; the minimum is searched strictly below the diagonal.\n");

    rc = setup_matrices(&A, &na, &B, &nb, &mb);
    if (rc == 0) {
        printf("End of input.\n");
        clear_memory(A, na);
        clear_memory(B, nb);
        return 0;
    }
    if (rc < 0) {
        clear_memory(A, na);
        clear_memory(B, nb);
        return 1;
    }

    printf("\n");
    print_matrix(A, na, na, "A");
    printf("\n");
    print_matrix(B, nb, mb, "B");
    wait_for_enter();

    for (;;) {
        printf("\nMenu:\n");
        printf("  1 - max of the whole A and min strictly below the diagonal\n");
        printf("  2 - transpose B in memory (mb x nb)\n");
        printf("  3 - multiply C = A x B\n");
        printf("  4 - sort a selected row of A\n");
        printf("  5 - row sums of A and column sums of B\n");
        printf("  6 - print matrices A and B\n");
        printf("  7 - re-initialize matrices\n");
        printf("  0 - exit\n");
        if (!read_int_line("Choice: ", 0, 7, &choice)) {
            break;
        }
        if (choice == 0) {
            break;
        }

        switch (choice) {
        case 1:
            max_and_min_below(A, na);
            wait_for_enter();
            break;
        case 2: {
            double **transposed = transpose_b(B, nb, mb);

            if (transposed != NULL) {
                int tmp = nb;

                clear_memory(B, nb);
                B = transposed;
                nb = mb;
                mb = tmp;
                printf("\nB was transposed in memory.\n");
                print_matrix(B, nb, mb, "B (transposed)");
            }
            wait_for_enter();
            break;
        }
        case 3: {
            double **C = mult_a_b(A, na, B, nb, mb);

            if (C != NULL) {
                printf("\nC = A x B:\n");
                print_matrix(C, na, mb, "C");
                clear_memory(C, na);
            }
            wait_for_enter();
            break;
        }
        case 4: {
            int row;

            printf("\nMatrix A before sorting:\n");
            print_matrix(A, na, na, "A");
            if (!read_int_line("Row index (0..na-1): ", 0, na - 1, &row)) {
                goto done;
            }
            sort_row(A[row], na);
            printf("\nMatrix A after sorting row %d:\n", row);
            print_matrix(A, na, na, "A");
            wait_for_enter();
            break;
        }
        case 5:
            row_and_column_sums(A, na, B, nb, mb);
            wait_for_enter();
            break;
        case 6:
            printf("\n");
            print_matrix(A, na, na, "A");
            printf("\n");
            print_matrix(B, nb, mb, "B");
            wait_for_enter();
            break;
        case 7: {
            clear_memory(A, na);
            A = NULL;
            clear_memory(B, nb);
            B = NULL;
            rc = setup_matrices(&A, &na, &B, &nb, &mb);
            if (rc == 0) {
                printf("End of input.\n");
                goto done;
            }
            if (rc < 0) {
                clear_memory(A, na);
                clear_memory(B, nb);
                return 1;
            }
            printf("\n");
            print_matrix(A, na, na, "A");
            printf("\n");
            print_matrix(B, nb, mb, "B");
            wait_for_enter();
            break;
        }
        default:
            break;
        }
    }

done:
    clear_memory(A, na);
    clear_memory(B, nb);
    printf("Memory released. Goodbye.\n");
    return 0;
}
