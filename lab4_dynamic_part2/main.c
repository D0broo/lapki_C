/*
 * Laboratory work 4, part 2: two-dimensional dynamic integer matrices.
 * Variant 10 (even): max/min are searched STRICTLY BELOW the main diagonal.
 *
 * Build: gcc -std=c11 -Wall -Wextra -Werror -pedantic -o lab4_2.exe main.c
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RANDOM_LOW  (-50)
#define RANDOM_HIGH (50)
#define DIM_MIN     (1)
#define DIM_MAX     (20)

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
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
    return 1;
}

/* Read an integer in [min; max]; return 0 on EOF. */
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

/* Read an integer, using `def` when the line is empty; return 0 on EOF. */
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

/* Read exactly `count` integers from one line; return 0 on EOF. */
static int read_int_values(const char *prompt, int *values, int count,
                           int min, int max)
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
            long parsed;

            while (*p == ' ' || *p == '\t') {
                ++p;
            }
            if (*p == '\0') {
                break;
            }
            errno = 0;
            parsed = strtol(p, &end, 10);
            if (end == p || errno != 0 || parsed < (long)min ||
                parsed > (long)max) {
                bad = 1;
                break;
            }
            values[read++] = (int)parsed;
            p = end;
        }
        while (*p == ' ' || *p == '\t') {
            ++p;
        }
        if (!bad && read == count && *p == '\0') {
            return 1;
        }
        printf("Enter exactly %d integer(s) in [%d; %d].\n", count, min, max);
    }
}

/* --------------------------------------------------------------- memory -- */

void clear_memory(int **matrix, int rows)
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

int **memory_allocation(int rows, int cols)
{
    int **matrix;
    int i;

    if (rows <= 0 || cols <= 0) {
        return NULL;
    }
    matrix = (int **)malloc((size_t)rows * sizeof *matrix);
    if (matrix == NULL) {
        return NULL;
    }
    for (i = 0; i < rows; ++i) {
        matrix[i] = (int *)malloc((size_t)cols * sizeof *matrix[i]);
        if (matrix[i] == NULL) {
            clear_memory(matrix, i);
            return NULL;
        }
    }
    return matrix;
}

/* ----------------------------------------------------------- printing ---- */

static void print_matrix(int **matrix, int rows, int cols, const char *name)
{
    int i;
    int j;

    printf("Matrix %s (%d x %d):\n", name, rows, cols);
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

/* ------------------------------------------------------- initialization -- */

static int fill_keyboard(int **matrix, int rows, int cols, const char *name)
{
    char prompt[128];
    int i;

    printf("Enter matrix %s row by row.\n", name);
    for (i = 0; i < rows; ++i) {
        (void)snprintf(prompt, sizeof prompt, "  Row %d/%d (%d int): ",
                       i + 1, rows, cols);
        if (!read_int_values(prompt, matrix[i], cols, -1000000, 1000000)) {
            return 0;
        }
    }
    return 1;
}

static void fill_random(int **matrix, int rows, int cols)
{
    int i;
    int j;

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            matrix[i][j] = RANDOM_LOW +
                           rand() % (RANDOM_HIGH - RANDOM_LOW + 1);
        }
    }
}

/* kind 0: A[i][j] = 10*i - j; kind 1: B[i][j] = i + j */
static void fill_formula(int **matrix, int rows, int cols, int kind)
{
    int i;
    int j;

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            matrix[i][j] = (kind == 0) ? 10 * i - j : i + j;
        }
    }
}

/* 1 = matrix ready, 0 = end of input, -1 = allocation failure */
static int init_matrix(int ***target, int rows, int cols, const char *name)
{
    int mode;
    int **matrix;

    printf("\nInitialization of matrix %s (%d x %d):\n", name, rows, cols);
    printf("  1 - keyboard input\n");
    printf("  2 - random values in [%d; %d]\n", RANDOM_LOW, RANDOM_HIGH);
    printf("  3 - formula\n");
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
        fill_random(matrix, rows, cols);
    } else {
        fill_formula(matrix, rows, cols, (name[0] == 'A') ? 0 : 1);
    }

    *target = matrix;
    return 1;
}

/* ------------------------------------------------------------ the tasks -- */

/*
 * Variant 10 is even: search ONLY below the main diagonal (i > j).
 * Initialised from A[1][0] and iterating i = 1..n_a-1, j = 0..i-1.
 */
void max_min(int **A, int n_a, int *max_a, int *min_a)
{
    int i;
    int j;
    int max_i = 1;
    int max_j = 0;
    int min_i = 1;
    int min_j = 0;

    *max_a = A[1][0];
    *min_a = A[1][0];
    for (i = 1; i < n_a; ++i) {
        for (j = 0; j < i; ++j) {
            if (A[i][j] > *max_a) {
                *max_a = A[i][j];
                max_i = i;
                max_j = j;
            }
            if (A[i][j] < *min_a) {
                *min_a = A[i][j];
                min_i = i;
                min_j = j;
            }
        }
    }
    printf("Below the main diagonal of A:\n");
    printf("  max = %d at A[%d][%d]\n", *max_a, max_i, max_j);
    printf("  min = %d at A[%d][%d]\n", *min_a, min_i, min_j);
}

/* Allocate and return an (m_b x n_b) transpose of B, or NULL on failure. */
int **transpose_b(int **B, int n_b, int m_b)
{
    int **transposed;
    int i;
    int j;

    transposed = memory_allocation(m_b, n_b);
    if (transposed == NULL) {
        fprintf(stderr, "Error: memory allocation failed for the transpose.\n");
        return NULL;
    }
    for (i = 0; i < n_b; ++i) {
        for (j = 0; j < m_b; ++j) {
            transposed[j][i] = B[i][j];
        }
    }
    return transposed;
}

/* Allocate and return C = A x B (n_a x m_b), or NULL if incompatible. */
int **mult_a_b(int **B, int n_b, int m_b, int **A, int n_a)
{
    int **C;
    int i;
    int j;
    int k;

    if (n_a != n_b) {
        printf("Error: A x B is impossible: n_a = %d but n_b = %d.\n",
               n_a, n_b);
        return NULL;
    }
    C = memory_allocation(n_a, m_b);
    if (C == NULL) {
        fprintf(stderr, "Error: memory allocation failed for C.\n");
        return NULL;
    }
    for (i = 0; i < n_a; ++i) {
        for (j = 0; j < m_b; ++j) {
            long long total = 0;

            for (k = 0; k < n_a; ++k) {
                total += (long long)A[i][k] * (long long)B[k][j];
            }
            C[i][j] = (int)total;
        }
    }
    return C;
}

static void sort_row(int *row, int size)
{
    int i;
    int j;

    for (i = 0; i < size - 1; ++i) {
        int best = i;

        for (j = i + 1; j < size; ++j) {
            if (row[j] < row[best]) {
                best = j;
            }
        }
        if (best != i) {
            int tmp = row[i];

            row[i] = row[best];
            row[best] = tmp;
        }
    }
}

static void sort_all(int **A, int n_a)
{
    int total = n_a * n_a;
    int p;

    for (p = 0; p < total - 1; ++p) {
        int best = p;
        int best_i = best / n_a;
        int best_j = best % n_a;
        int q;

        for (q = p + 1; q < total; ++q) {
            int q_i = q / n_a;
            int q_j = q % n_a;

            if (A[q_i][q_j] < A[best_i][best_j]) {
                best = q;
                best_i = q_i;
                best_j = q_j;
            }
        }
        if (best != p) {
            int tmp = A[p / n_a][p % n_a];

            A[p / n_a][p % n_a] = A[best_i][best_j];
            A[best_i][best_j] = tmp;
        }
    }
}

static void sort_menu(int **A, int n_a)
{
    int choice;
    int row;

    printf("\nMatrix A before sorting:\n");
    print_matrix(A, n_a, n_a, "A");
    printf("  1 - sort ALL elements of A in ascending order\n");
    printf("  2 - sort ONE selected row of A in ascending order\n");
    if (!read_int_line("Choice: ", 1, 2, &choice)) {
        return;
    }
    if (choice == 1) {
        sort_all(A, n_a);
    } else {
        if (!read_int_line("Row index (0..n_a-1): ", 0, n_a - 1, &row)) {
            return;
        }
        sort_row(A[row], n_a);
    }
    printf("\nMatrix A after sorting:\n");
    print_matrix(A, n_a, n_a, "A");
}

static void row_and_column_sums(int **A, int n_a, int **B, int n_b, int m_b)
{
    int i;
    int j;

    printf("\nRow sums of A (%d x %d):\n", n_a, n_a);
    for (i = 0; i < n_a; ++i) {
        long long total = 0;

        for (j = 0; j < n_a; ++j) {
            total += A[i][j];
        }
        printf("  row %2d: %lld\n", i, total);
    }
    printf("Column sums of B (%d x %d):\n", n_b, m_b);
    for (j = 0; j < m_b; ++j) {
        long long total = 0;

        for (i = 0; i < n_b; ++i) {
            total += B[i][j];
        }
        printf("  col %2d: %lld\n", j, total);
    }
}

/* ---------------------------------------------------------------- setup -- */

/* 1 = ready, 0 = end of input, -1 = allocation failure */
static int setup_matrices(int ***pA, int *pn_a, int ***pB, int *pn_b,
                          int *pm_b)
{
    int n_a;
    int n_b;
    int m_b;
    int rc;

    printf("\n--- Matrix dimensions ---\n");
    if (!read_int_default("n_a (square A, 2..20, default 10): ",
                          10, 2, DIM_MAX, &n_a)) {
        return 0;
    }
    if (!read_int_default("n_b (rows of B, 1..20, default 10): ",
                          10, DIM_MIN, DIM_MAX, &n_b)) {
        return 0;
    }
    if (!read_int_default("m_b (columns of B, 1..20, default 5): ",
                          5, DIM_MIN, DIM_MAX, &m_b)) {
        return 0;
    }

    rc = init_matrix(pA, n_a, n_a, "A");
    if (rc <= 0) {
        return rc;
    }
    rc = init_matrix(pB, n_b, m_b, "B");
    if (rc <= 0) {
        clear_memory(*pA, n_a);
        *pA = NULL;
        return rc;
    }
    *pn_a = n_a;
    *pn_b = n_b;
    *pm_b = m_b;
    return 1;
}

/* ----------------------------------------------------------------- main -- */

int main(void)
{
    int **A = NULL;
    int **B = NULL;
    int n_a = 0;
    int n_b = 0;
    int m_b = 0;
    int rc;
    int choice;

    srand((unsigned)time(NULL));

    printf("Laboratory work 4, part 2 (Variant 10, even).\n");
    printf("Max/min of A are searched strictly BELOW the main diagonal.\n");

    rc = setup_matrices(&A, &n_a, &B, &n_b, &m_b);
    if (rc == 0) {
        printf("End of input.\n");
        clear_memory(A, n_a);
        clear_memory(B, n_b);
        return 0;
    }
    if (rc < 0) {
        clear_memory(A, n_a);
        clear_memory(B, n_b);
        return 1;
    }

    printf("\n");
    print_matrix(A, n_a, n_a, "A");
    printf("\n");
    print_matrix(B, n_b, m_b, "B");

    for (;;) {
        printf("\nMenu:\n");
        printf("  1 - max and min of A strictly below the main diagonal\n");
        printf("  2 - transpose B in memory (m_b x n_b)\n");
        printf("  3 - multiply C = A x B\n");
        printf("  4 - sort A (all elements or one row)\n");
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
        case 1: {
            int max_a;
            int min_a;

            max_min(A, n_a, &max_a, &min_a);
            break;
        }
        case 2: {
            int **transposed = transpose_b(B, n_b, m_b);

            if (transposed != NULL) {
                int tmp = n_b;

                clear_memory(B, n_b);
                B = transposed;
                n_b = m_b;
                m_b = tmp;
                printf("\nB was transposed in memory.\n");
                print_matrix(B, n_b, m_b, "B (transposed)");
            }
            break;
        }
        case 3: {
            int **C = mult_a_b(B, n_b, m_b, A, n_a);

            if (C != NULL) {
                printf("\nC = A x B:\n");
                print_matrix(C, n_a, m_b, "C");
                clear_memory(C, n_a);
            }
            break;
        }
        case 4:
            sort_menu(A, n_a);
            break;
        case 5:
            row_and_column_sums(A, n_a, B, n_b, m_b);
            break;
        case 6:
            printf("\n");
            print_matrix(A, n_a, n_a, "A");
            printf("\n");
            print_matrix(B, n_b, m_b, "B");
            break;
        case 7: {
            clear_memory(A, n_a);
            A = NULL;
            clear_memory(B, n_b);
            B = NULL;
            rc = setup_matrices(&A, &n_a, &B, &n_b, &m_b);
            if (rc == 0) {
                printf("End of input.\n");
                goto done;
            }
            if (rc < 0) {
                clear_memory(A, n_a);
                clear_memory(B, n_b);
                return 1;
            }
            printf("\n");
            print_matrix(A, n_a, n_a, "A");
            printf("\n");
            print_matrix(B, n_b, m_b, "B");
            break;
        }
        default:
            break;
        }
    }

done:
    clear_memory(A, n_a);
    clear_memory(B, n_b);
    printf("Memory released. Goodbye.\n");
    return 0;
}
