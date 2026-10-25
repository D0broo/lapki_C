#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Ширина стовпців для Варіанту 10 (Таблиця 1 / Таблиця 4) */
#define W_N 7
#define W_X 21
#define W_F 21

/* Кількість рядків таблиці на один екран */
#define ROWS_PER_PAGE 10

/* Прототипи (скелети) функцій */
double f(double x);
void print_chars(char c, int count);
void print_row_separator();
void print_table_header();
void clear_stdin();

int main() {
    unsigned int variant = 0;
    unsigned int N = 0;
    double X1 = 0.0, X2 = 0.0, delta = 0.0;

    /* 1. Вибір та валідація варіанту введення початкових даних */
	long temp_variant = 0;
    do {
        printf("Select input variant (1 - X1, X2, N; 2 - X1, X2, delta): ");
        if (scanf("%ld", &temp_variant) != 1) {
            clear_stdin();
            temp_variant = 0;
        }
        if (temp_variant != 1 && temp_variant != 2) {
            printf("Invalid variant! Please enter 1 or 2.\n");
        }
    } while (temp_variant != 1 && temp_variant != 2);
    variant = (unsigned int)temp_variant;

    /* 2. Введення та валідація X1, X2 і N / delta */
    if (variant == 1) {
        do {
            printf("Enter X1, X2 (X1 < X2): ");
            if (scanf("%lf %lf", &X1, &X2) != 2) {
                clear_stdin();
                X1 = 0.0;
                X2 = 0.0;
            }
            if (X1 >= X2) {
                printf("Error: X1 must be strictly less than X2.\n");
            }
        } while (X1 >= X2);

        long temp_N = 0;
        do {
            printf("Enter number of points N (N >= 2): ");
            if (scanf("%ld", &temp_N) != 1) {
                clear_stdin();
                temp_N = 0;
            }
            if (temp_N < 2) {
                printf("Error: N must be at least 2.\n");
            }
        } while (temp_N < 2);
        N = (unsigned int)temp_N;

        delta = (X2 - X1) / (double)(N - 1);
    }
    else {
        do {
            printf("Enter X1, X2 (X1 < X2): ");
            if (scanf("%lf %lf", &X1, &X2) != 2) {
                clear_stdin();
                X1 = 0.0;
                X2 = 0.0;
            }
            if (X1 >= X2) {
                printf("Error: X1 must be strictly less than X2.\n");
            }
        } while (X1 >= X2);

        do {
            printf("Enter step delta (0 < delta <= X2 - X1): ");
            if (scanf("%lf", &delta) != 1) {
                clear_stdin();
                delta = 0.0;
            }
            if (delta <= 0.0 || delta > (X2 - X1)) {
                printf("Error: delta must be positive and not exceed (X2 - X1).\n");
            }
        } while (delta <= 0.0 || delta > (X2 - X1));

        N = (unsigned int)((X2 - X1) / delta + 1e-9) + 1;
    }

    clear_stdin(); /* Очищення залишку рядка перед посторінковим виводом */

    /* 3. Вивід введених початкових даних перед таблицею */
    putchar('\n');
    if (variant == 1) {
        printf("X1=%.2f, X2=%.2f, N=%u (delta=%.4f)\n", X1, X2, N, delta);
    }
    else {
        printf("X1=%.2f, X2=%.2f, delta=%.2f (N=%u)\n", X1, X2, delta, N);
    }

    /* 4. Побудова та посторінковий вивід таблиці */
    print_table_header();
    print_row_separator();

    for (unsigned int i = 1; i <= N; i++) {
        double x = (i == N && variant == 1) ? X2 : (X1 + (i - 1) * delta);
        double fx = f(x);

        printf("|%*u|%*.2f|%*.4f|\n", W_N, i, W_X, x, W_F, fx);
        print_row_separator();

        /* Зупинка виведення при заповненні екрану (кожні ROWS_PER_PAGE рядків) */
        if (i % ROWS_PER_PAGE == 0 && i < N) {
            if (i == ROWS_PER_PAGE) {
                /* Для першого екрану: виводимо повідомлення і затираємо його після натискання клавіші */
                printf("Press Any Key to Continue ...");
                fflush(stdout);
                getchar();
                printf("\r                             \r");
                fflush(stdout);
            } else {
                /* Для другого і наступних екранів: зупинка без виведення повідомлення */
                getchar();
            }
        }
    }

    /* 5. Визначення діапазонів аргументів [alpha, beta], де функція змінює знак */
    printf("\nSign change intervals [alpha, beta]:\n");
    int intervals_found = 0;
    double prev_x = X1;
    double prev_fx = f(prev_x);

    for (unsigned int i = 2; i <= N; i++) {
        double curr_x = (i == N && variant == 1) ? X2 : (X1 + (i - 1) * delta);
        double curr_fx = f(curr_x);

        if (prev_fx * curr_fx < 0.0) {
            printf("  Interval #%d: [%.4f; %.4f] (f(%.4f) = %.4f, f(%.4f) = %.4f)\n",
                ++intervals_found, prev_x, curr_x, prev_x, prev_fx, curr_x, curr_fx);
        }
        else if (fabs(curr_fx) < 1e-12) {
            printf("  Exact root found at x = %.4f (f(x) = 0.0000)\n", curr_x);
            intervals_found++;
        }

        prev_x = curr_x;
        prev_fx = curr_fx;
    }

    if (intervals_found == 0) {
        printf("  No sign changes found on the interval [%.2f; %.2f] with step %.4f.\n", X1, X2, delta);
    }

    return 0;
}

/* ================= РЕАЛІЗАЦІЯ (ОПИС) ФУНКЦІЙ ================= */

/* Вихідна функція f(x) для Варіанту 10:
   За замовчуванням: f(x) = 0.25 * (x - 25)^2 + (x + 25)^3 / 100 - 1
   (При потребі скоригуйте знаки відповідно до оригіналу методички) */
double f(double x) {
    return 0.25 * pow(x - 25.0, 2.0) + pow(x + 25.0, 3.0) / 100.0 - 1.0;
}

/* Допоміжна функція для друку повторюваного символу */
void print_chars(char c, int count) {
    for (int i = 0; i < count; i++) {
        putchar(c);
    }
}

/* Друк горизонтального розділювача таблиці: +-------+---------------------+---------------------+ */
void print_row_separator() {
    putchar('+');
    print_chars('-', W_N);
    putchar('+');
    print_chars('-', W_X);
    putchar('+');
    print_chars('-', W_F);
    printf("+\n");
}

/* Друк шапки таблиці із символами '*' */
void print_table_header() {
    int total_width = 1 + W_N + 1 + W_X + 1 + W_F + 1; /* 53 символи */

    print_chars('*', total_width);
    putchar('\n');

    printf("*%*s%-*s*%*s%-*s*%*s%-*s*\n",
        (W_N - 1) / 2, "", W_N - (W_N - 1) / 2, "N",
        (W_X - 1) / 2, "", W_X - (W_X - 1) / 2, "X",
        (W_F - 4) / 2, "", W_F - (W_F - 4) / 2, "F(X)");

    print_chars('*', total_width);
    putchar('\n');
}

/* Очищення буфера вводу при некоректних даних */
void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}