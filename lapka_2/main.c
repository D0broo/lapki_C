#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ==================== ЛАБОРАТОРНА РОБОТА 2 (ВАРІАНТ 10) ====================
   Чисельне інтегрування f(x) = x^2 * sin(x) на відрізку [a; b]
   методами лівих/правих прямокутників, трапецій та Сімпсона. */

/* Тип вказівника на функцію чисельного інтегрування */
typedef double (*integral_fn)(double, double, unsigned long);

/* Ідентифікатори методів (індекси в масиві METHODS) */
#define M_LEFT  0
#define M_RIGHT 1
#define M_TRAP  2
#define M_SIMP  3
#define N_METHODS 4

/* Захисна верхня межа кількості кроків N при пошуку точності eps */
#define N_LIMIT 100000000UL

/* Точки та діапазон епсилона за методичкою */
#define EPS_MIN 0.00001
#define EPS_MAX 0.001

/* Прототипи функцій */
double integrand_expression(double x);
double num_comput_integral_l_re(double a, double b, unsigned long n);
double num_comput_integral_r_re(double a, double b, unsigned long n);
double num_comput_integral_trap(double a, double b, unsigned long n);
double num_comput_integral_Simps(double a, double b, unsigned long n);
void clear_stdin(void);

/* Обчислення одного методу: I(n), пошук N для точності eps, Delta */
static void compute_method(int method, double a, double b, unsigned long n, double eps,
                           double *out_i_n, unsigned long *out_N, double *out_I_N, double *out_delta);
/* Доопрацювання однієї опції меню (1..5) */
static void run_option(int option);
/* Ввід з валідацією a, b, n, eps */
static void read_inputs(double *a, double *b, unsigned long *n, double *eps);

/* Масив методів та їхніх назв для меню і зведеної таблиці */
static const integral_fn METHODS[N_METHODS] = {
    num_comput_integral_l_re,
    num_comput_integral_r_re,
    num_comput_integral_trap,
    num_comput_integral_Simps
};
static const char *NAMES[N_METHODS] = {
    "Left rectangles",
    "Right rectangles",
    "Trapezoidal",
    "Simpson's rule"
};

int main(void) {
    /* while(1): меню працює, доки користувач не вибере 0 (вихід) */
    for (;;) {
        int option = -1;

        printf("\n=== Numerical integration, Variant 10 (f(x) = x^2 * sin(x)) ===\n");
        printf("Operations:\n");
        printf("  1 - Left rectangles\n");
        printf("  2 - Right rectangles\n");
        printf("  3 - Trapezoidal\n");
        printf("  4 - Simpson's rule\n");
        printf("  5 - Summary table for all methods\n");
        printf("  0 - Exit\n");
        printf("Select operation: ");
        if (scanf("%d", &option) != 1) {
            clear_stdin();
            option = -1;
        }
        if (option == 0) {
            printf("Goodbye!\n");
            break;
        }
        if (option < 1 || option > 5) {
            clear_stdin();
            printf("Invalid operation! Please enter a number from 0 to 5.\n");
            continue;
        }
        clear_stdin();
        run_option(option);
    }
    return 0;
}

/* ==================== РЕАЛІЗАЦІЯ (ОПИС) ФУНКЦІЙ ==================== */

/* Підінтегральна функція Варіанту 10: f(x) = x^2 * sin(x) */
double integrand_expression(double x) {
    return x * x * sin(x);
}

/* Ліві прямокутники: sum(k = 0..n-1) f(x_k) * h, h = (b - a) / n */
double num_comput_integral_l_re(double a, double b, unsigned long n) {
    double h = (b - a) / (double)n;
    double sum = 0.0;

    for (unsigned long k = 0; k < n; k++)
        sum += integrand_expression(a + h * (double)k);
    return sum * h;
}

/* Праві прямокутники: sum(k = 1..n) f(x_k) * h, h = (b - a) / n */
double num_comput_integral_r_re(double a, double b, unsigned long n) {
    double h = (b - a) / (double)n;
    double sum = 0.0;

    for (unsigned long k = 1; k <= n; k++)
        sum += integrand_expression(a + h * (double)k);
    return sum * h;
}

/* Метод трапецій: h * ((f(a) + f(b)) / 2 + sum(k = 1..n-1) f(x_k)) */
double num_comput_integral_trap(double a, double b, unsigned long n) {
    double h = (b - a) / (double)n;
    double sum = 0.0;

    for (unsigned long k = 1; k < n; k++)
        sum += integrand_expression(a + h * (double)k);
    return h * ((integrand_expression(a) + integrand_expression(b)) / 2.0 + sum);
}

/* Метод Сімпсона (парабол): h / 3 * (f(a) + f(b)
    + 4 * sum(непарні k = 1..n-1) + 2 * sum(парні k = 2..n-2)).
   Працює лише для парних n: якщо n непарне, збільшує його на 1 і попереджає. */
double num_comput_integral_Simps(double a, double b, unsigned long n) {
    if (n % 2UL == 1UL) {
        printf("Warning: Simpson's rule needs an even n; n = %lu incremented to %lu.\n",
            n, n + 1UL);
        n++;
    }
    {
        double h = (b - a) / (double)n;
        double sum = integrand_expression(a) + integrand_expression(b);

        for (unsigned long k = 1; k < n; k++)
            sum += ((k % 2UL == 1UL) ? 4.0 : 2.0) * integrand_expression(a + h * (double)k);
        return h / 3.0 * sum;
    }
}

/* Ввід з валідацією a, b, n, eps (стиль лабораторної 1) */
static void read_inputs(double *a, double *b, unsigned long *n, double *eps) {
    do {
        printf("Enter a, b (a < b): ");
        if (scanf("%lf %lf", a, b) != 2) {
            clear_stdin();
            *a = 0.0;
            *b = -1.0; /* змушує повторити ввід */
        }
        if (*a >= *b)
            printf("Error: a must be strictly less than b.\n");
    } while (*a >= *b);

    do {
        printf("Enter initial number of steps n (n > 0): ");
        if (scanf("%lu", n) != 1 || *n == 0UL) {
            clear_stdin();
            *n = 0UL;
        }
        if (*n == 0UL)
            printf("Error: n must be a positive integer.\n");
    } while (*n == 0UL);

    do {
        printf("Enter eps (%.5f <= eps <= %.3f): ", EPS_MIN, EPS_MAX);
        if (scanf("%lf", eps) != 1) {
            clear_stdin();
            *eps = 0.0;
        }
        if (*eps < EPS_MIN || *eps > EPS_MAX)
            printf("Error: eps must be within [%.5f; %.3f].\n", EPS_MIN, EPS_MAX);
    } while (*eps < EPS_MIN || *eps > EPS_MAX);

    clear_stdin();
}

/* Обчислення одного методу за методичкою:
   1) I(n) - значення при початковому n;
   2) пошук N точності eps: I1 = method(a, b, N); N += 2;
      I2 = method(a, b, N); Delta = |I1 - I2|; поки Delta > eps. */
static void compute_method(int method, double a, double b, unsigned long n, double eps,
                           double *out_i_n, unsigned long *out_N, double *out_I_N, double *out_delta) {
    integral_fn fn = METHODS[method];
    unsigned long N = n;
    double i1, i2, delta;

    /* Непарний n для Сімпсона спочатку коректно збільшується на 1
       з попередженням під час першого виклику методу. */
    if (method == M_SIMP && n % 2UL == 1UL)
        N = n + 1UL;

    *out_i_n = fn(a, b, n);

    do {
        i1 = fn(a, b, N);
        N += 2UL;
        i2 = fn(a, b, N);
        delta = fabs(i1 - i2);
    } while (delta > eps && N < N_LIMIT);

    *out_N = N;
    *out_I_N = i2;
    *out_delta = delta;
}

/* Вивід результатів одного методу у вигляді блоку */
static void print_method_block(int method, double a, double b, unsigned long n, double eps) {
    double i_n, i_n2, delta;
    unsigned long N;

    compute_method(method, a, b, n, eps, &i_n, &N, &i_n2, &delta);

    printf("Method           : %s\n", NAMES[method]);
    printf("a, b, n, eps     : %.6f %.6f %lu %.8f\n", a, b, n, eps);
    printf("I at initial n   : %12.6f\n", i_n);
    printf("Found N          : %lu\n", N);
    printf("I at N           : %12.6f\n", i_n2);
    if (delta <= eps)
        printf("Delta            : %.3e (<= eps %.1e)\n", delta, eps);
    else
        printf("Delta            : %.3e (> eps; N limit %lu reached)\n", delta, N);
}

/* Шапка зведеної таблиці для всіх чотирьох методів */
static void print_summary_header(void) {
    printf("\n+-------------------+--------------+-----------+--------------+-----------+\n");
    printf("| Method            |         I(n) |         N |         I(N) |     Delta |\n");
    printf("+-------------------+--------------+-----------+--------------+-----------+\n");
}

static void print_summary_row(int method, double i_n, unsigned long N, double i_n2, double delta) {
    printf("| %-17s | %12.6f | %9lu | %12.6f | %9.3e |\n", NAMES[method], i_n, N, i_n2, delta);
    printf("+-------------------+--------------+-----------+--------------+-----------+\n");
}

/* Доопрацювання вибраної опції меню: ввід даних і обчислення */
static void run_option(int option) {
    double a = 0.0, b = 0.0, eps = 0.0;
    unsigned long n = 0UL;

    printf("\n--- %s ---\n", option == 5 ? "Summary table for all methods" : "Method selected");
    read_inputs(&a, &b, &n, &eps);
    printf("Initial parameters: a = %.6f, b = %.6f, n = %lu, eps = %.8f\n\n", a, b, n, eps);

    if (option == 5) {
        print_summary_header();
        for (int method = 0; method < N_METHODS; method++) {
            double i_n, i_n2, delta;
            unsigned long N;

            compute_method(method, a, b, n, eps, &i_n, &N, &i_n2, &delta);
            print_summary_row(method, i_n, N, i_n2, delta);
        }
    }
    else {
        print_method_block(option - 1, a, b, n, eps);
    }
}

/* Очищення буфера вводу при некоректних даних */
void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
