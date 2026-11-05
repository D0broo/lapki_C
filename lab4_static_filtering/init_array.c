/*
 * Initialization of the image matrix Y[N][M] and the shared input helpers.
 * Laboratory work 4: static array filtering, Variant 10.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "filter.h"

/* Public prototypes (filter_read_*, init_array) are declared in filter.h. */

/* ----------------------------------------------------------- prototypes -- */

/* Internal initialization helpers. */
static void init_random(int Y[N][M]);
static void init_keyboard(int Y[N][M]);

/* ------------------------------------------------------------------ code -- */

int filter_read_line(char *buffer, int size)
{
    size_t length;
    int ch;

    if (fgets(buffer, size, stdin) == NULL) {
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

int filter_read_int(const char *prompt, int min, int max, int *out)
{
    char line[128];

    for (;;) {
        char *end = NULL;
        long parsed;

        printf("%s", prompt);
        fflush(stdout);
        if (!filter_read_line(line, (int)sizeof line)) {
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

int filter_read_double(const char *prompt, double min, double max, double *out)
{
    char line[128];

    for (;;) {
        char *end = NULL;
        double parsed;

        printf("%s", prompt);
        fflush(stdout);
        if (!filter_read_line(line, (int)sizeof line)) {
            return 0;
        }
        if (line[0] == '\0') {
            printf("Empty input; enter a number in [%.2f; %.2f].\n", min, max);
            continue;
        }
        errno = 0;
        parsed = strtod(line, &end);
        while (end != NULL && (*end == ' ' || *end == '\t')) {
            ++end;
        }
        if (errno != 0 || end == line || end == NULL || *end != '\0' ||
            parsed < min || parsed > max) {
            printf("Invalid input; enter a number in [%.2f; %.2f].\n",
                   min, max);
            continue;
        }
        *out = parsed;
        return 1;
    }
}

int filter_read_values(const char *prompt, int *values, int count, int min,
                       int max)
{
    char line[4096];

    for (;;) {
        int read = 0;
        int bad = 0;
        char *p;

        printf("%s", prompt);
        fflush(stdout);
        if (!filter_read_line(line, (int)sizeof line)) {
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

static void init_random(int Y[N][M])
{
    int a;
    int b;
    int i;
    int j;

    if (!filter_read_int("a (lower bound, 0..255): ", 0, 255, &a)) {
        return;
    }
    if (!filter_read_int("b (upper bound, a..255): ", a, 255, &b)) {
        return;
    }
    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            Y[i][j] = a + rand() % (b - a + 1);
        }
    }
    printf("Y filled with random values in [%d; %d].\n", a, b);
}

static void init_keyboard(int Y[N][M])
{
    char prompt[64];
    int i;

    printf("Enter matrix Y row by row (values 0..255).\n");
    for (i = 0; i < N; ++i) {
        (void)snprintf(prompt, sizeof prompt, "  Row %2d/%d (%d int): ",
                       i + 1, N, M);
        if (!filter_read_values(prompt, Y[i], M, 0, 255)) {
            return;
        }
    }
}

void init_array(int Y[N][M])
{
    int mode;

    printf("\nInitialization of matrix Y (%d x %d):\n", N, M);
    printf("  1 - random values in [a; b], 0 <= a <= b <= 255\n");
    printf("  2 - keyboard input, validated to 0..255\n");
    if (!filter_read_int("Mode: ", 1, 2, &mode)) {
        return;
    }
    if (mode == 1) {
        init_random(Y);
    } else {
        init_keyboard(Y);
    }
}
