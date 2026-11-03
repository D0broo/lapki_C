#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* ==================== ЛАБОРАТОРНА РОБОТА 3 (ВАРІАНТ 10) ====================
   Розв'язання нелінійного рівняння f(x) = 0 методом хорд та методом
   бісекції. Функція f(x) взята з Лабораторної роботи 1 (Варіант 10):
       f(x) = 0.25 * (x - 25)^2 + (x + 25)^3 / 100 - 1;
       df(x) = 0.5 * (x - 25) + 3 * (x + 25)^2 / 100. */

/* Границі епсилона за методичкою */
#define EPS_MIN 0.00001
#define EPS_MAX 0.001

/* Кількість контрольних точок для перевірки знаку f'(x) на відрізку */
#define SIGN_SAMPLES 13

/* Результат роботи одного методу */
typedef struct {
    double root;     /* знайдений корінь X */
    double residual; /* f(X) */
    long iterations; /* кількість ітерацій */
    double seconds;  /* час виконання методу (без пауз для користувача) */
    int aborted;     /* 1 - роботу перервав користувач */
} SolverResult;

/* Прототипи функцій */
double f(double x);
double df(double x);
static int interval_ok(double x1, double x2); /* 1 - можна розв'язувати на [x1; x2] */
static SolverResult chord_method(double x1, double x2, double eps, long max_iter, int debug);
static SolverResult bisection_method(double x1, double x2, double eps, long max_iter, int debug);
static int batch_menu(long iterations, long limit, double xi, double fi);
static void debug_line(long iterations, double xi, double fi);
static void print_result(const char *name, const SolverResult *r);
static void read_params(double *x1, double *x2, double *eps, long *max_iter, int *debug);
static void clear_stdin(void);

int main(void) {
    double x1 = 0.0, x2 = 0.0, eps = 0.0;
    long max_iter = 0;
    int debug = 0;

    printf("=== Solving f(x) = 0, Variant 10 ===\n");
    printf("f(x) = 0.25*(x - 25)^2 + (x + 25)^3/100 - 1  (as in Lab work 1)\n\n");

    read_params(&x1, &x2, &eps, &max_iter, &debug);

    printf("\nSolving on [%.10g; %.10g], eps = %.1e, max_iter = %ld, debug = %d\n\n",
        x1, x2, eps, max_iter, debug);

    {
        SolverResult chord = chord_method(x1, x2, eps, max_iter, debug);
        SolverResult bisection = bisection_method(x1, x2, eps, max_iter, debug);

        print_result("Chord method", &chord);
        print_result("Bisection method", &bisection);
    }

    return 0;
}

/* ==================== РЕАЛІЗАЦІЯ (ОПИС) ФУНКЦІЙ ==================== */

/* Вихідна функція f(x) для Варіанту 10 (взята з Лабораторної роботи 1) */
double f(double x) {
    return 0.25 * pow(x - 25.0, 2.0) + pow(x + 25.0, 3.0) / 100.0 - 1.0;
}

/* Похідна f'(x) */
double df(double x) {
    return 0.5 * (x - 25.0) + 3.0 * pow(x + 25.0, 2.0) / 100.0;
}

/* Перевірка інтервалу: зміна знака f(x1) * f(x2) < 0 та монотонність
   (f'(x) зберігає сталий знак на всьому відрізку). Друкує причину відмови. */
static int interval_ok(double x1, double x2) {
    double fx1 = f(x1), fx2 = f(x2);
    int ok = 1, sign = 0, seen = 0;

    if (fx1 * fx2 >= 0.0) {
        double exact = (fx1 == 0.0) ? x1 : (fx2 == 0.0 ? x2 : 0.0);

        if (fx1 == 0.0 || fx2 == 0.0)
            printf("Interval rejected: f(%.10g) = 0, so the root is known exactly: X = %.10g.\n",
                exact, exact);
        else
            printf("Interval rejected: f(x1) * f(x2) = %.6e >= 0 - no sign change on [%.10g; %.10g].\n",
                fx1 * fx2, x1, x2);
        ok = 0;
    }

    for (int i = 0; i < SIGN_SAMPLES; i++) {
        double xi = x1 + (x2 - x1) * (double)i / (double)(SIGN_SAMPLES - 1);
        double di = df(xi);
        int si = (di > 0.0) - (di < 0.0);

        if (si == 0) {
            printf("Interval rejected: f'(%.10g) = %.6e - f' has a zero on the interval, f is not monotone.\n",
                xi, di);
            return 0;
        }
        if (!seen) {
            sign = si;
            seen = 1;
        }
        else if (si != sign) {
            printf("Interval rejected: f' keeps no constant sign on [%.10g; %.10g] (e.g. f' changes around %.6g) - f is not monotone.\n",
                x1, x2, xi);
            return 0;
        }
    }
    return ok;
}

/* Керківський рядок режиму налагодження: номер ітерації, x_i, f(x_i) */
static void debug_line(long iterations, double xi, double fi) {
    printf("[debug] it %ld: x = %.12f, f(x) = %.6e\n", iterations, xi, fi);
}

/* Меню паузи після кожних max_iter ітерацій без збіжності.
   Повертає: 1 - ще один пакет, 2 - ліміт знято, 3 - перервати (поточне xi);
   при закінченні вхідного потоку (EOF) повертає 3 (аварійне завершення). */
static int batch_menu(long iterations, long limit, double xi, double fi) {
    int choice = 0;

    printf("\nIteration limit reached: %ld iterations (limit %ld) without convergence.\n",
        iterations, limit);
    printf("Current intermediate: x = %.12f, f(x) = %.6e\n", xi, fi);
    printf("  1 - Continue for another %ld iterations\n", limit);
    printf("  2 - Run until convergence without iteration limits\n");
    printf("  3 - Abort and print current intermediate value\n");
    printf("Choice: ");
    for (;;) {
        int got = scanf("%d", &choice);

        if (got == EOF) {
            printf("\nInput stream ended - treating as abort.\n");
            return 3;
        }
        if (got == 1 && choice >= 1 && choice <= 3) {
            clear_stdin();
            return choice;
        }
        if (got != 1)
            clear_stdin();
        printf("Invalid choice; enter 1, 2 or 3.\nChoice: ");
    }
}

/* Метод хорд:
   x_i = (f(x2) * x1 - f(x1) * x2) / (f(x2) - f(x1));
   підстановка кінця: якщо f(x1) * f(x_i) < 0, то x2 = x_i, інакше x1 = x_i;
   зупинка: |f(x_i)| <= eps. */
static SolverResult chord_method(double x1, double x2, double eps, long max_iter, int debug) {
    SolverResult r = { 0.5 * (x1 + x2), f(0.5 * (x1 + x2)), 0, 0.0, 1 };
    clock_t t0 = clock();
    long limit = max_iter;
    long iters = 0;

    for (;;) {
        double f1 = f(x1), f2 = f(x2);
        double xi, fi, elapsed;

        /* Захист від виродження обох кінців в одну точку */
        if (fabs(f2 - f1) < 1e-300) {
            xi = 0.5 * (x1 + x2);
            fi = f(xi);
            iters++;
            if (debug) debug_line(iters, xi, fi);
            r.root = xi; r.residual = fi; r.iterations = iters; r.aborted = 0;
            break;
        }

        xi = (f2 * x1 - f1 * x2) / (f2 - f1);
        fi = f(xi);
        iters++;
        if (debug) debug_line(iters, xi, fi);

        if (fabs(fi) <= eps) {
            r.root = xi; r.residual = fi; r.iterations = iters; r.aborted = 0;
            break;
        }

        if (f1 * fi < 0.0) x2 = xi; else x1 = xi;

        if (limit > 0 && iters % limit == 0) {
            int choice;

            elapsed = (double)(clock() - t0) / CLOCKS_PER_SEC;
            r.seconds += elapsed;
            choice = batch_menu(iters, limit, xi, fi);
            if (choice == 3) {
                r.root = xi; r.residual = fi; r.iterations = iters; r.aborted = 1;
                break;
            }
            if (choice == 2)
                limit = 0; /* далі без обмежень, до збіжності */
            t0 = clock(); /* час паузи не входить у час роботи методу */
        }
    }
    r.seconds += (double)(clock() - t0) / CLOCKS_PER_SEC;
    return r;
}

/* Метод бісекції (половинного поділу):
   x_i = (x1 + x2) / 2; та сама перевірка знаків для оновлення кінців;
   зупинка: |x1 - x2| <= eps або |f(x_i)| <= eps. */
static SolverResult bisection_method(double x1, double x2, double eps, long max_iter, int debug) {
    SolverResult r = { 0.5 * (x1 + x2), f(0.5 * (x1 + x2)), 0, 0.0, 1 };
    clock_t t0 = clock();
    long limit = max_iter;
    long iters = 0;

    for (;;) {
        double f1 = f(x1);
        double xi, fi;

        if (fabs(x1 - x2) <= eps) {
            xi = 0.5 * (x1 + x2);
            r.root = xi; r.residual = f(xi); r.iterations = iters; r.aborted = 0;
            break;
        }

        xi = 0.5 * (x1 + x2);
        fi = f(xi);
        iters++;
        if (debug) debug_line(iters, xi, fi);

        if (fabs(fi) <= eps) {
            r.root = xi; r.residual = fi; r.iterations = iters; r.aborted = 0;
            break;
        }

        if (f1 * fi < 0.0) x2 = xi; else x1 = xi;

        if (limit > 0 && iters % limit == 0) {
            double elapsed = (double)(clock() - t0) / CLOCKS_PER_SEC;
            int choice;

            r.seconds += elapsed;
            choice = batch_menu(iters, limit, xi, fi);
            if (choice == 3) {
                r.root = xi; r.residual = fi; r.iterations = iters; r.aborted = 1;
                break;
            }
            if (choice == 2)
                limit = 0; /* далі без обмежень, до збіжності */
            t0 = clock(); /* час паузи не входить у час роботи методу */
        }
    }
    r.seconds += (double)(clock() - t0) / CLOCKS_PER_SEC;
    return r;
}

/* Форматований вивід результату одного методу */
static void print_result(const char *name, const SolverResult *r) {
    printf("=== %s ===\n", name);
    printf("Root X        : %.10f\n", r->root);
    printf("Residual f(X) : %.6e\n", r->residual);
    printf("Iterations    : %ld\n", r->iterations);
    if (r->seconds < 1.0)
        printf("Elapsed time  : %.0f ms\n", r->seconds * 1000.0);
    else
        printf("Elapsed time  : %.3f s\n", r->seconds);
    if (r->aborted)
        printf("Aborted by user; X and f(X) above are the current intermediate values.\n");
    printf("\n");
}

/* Ввід з валідацією x1, x2, eps, max_iter, debug (стиль лабораторної 1) */
static void read_params(double *x1, double *x2, double *eps, long *max_iter, int *debug) {
    int valid = 0;

    do {
        printf("Enter interval [x1, x2] (x1 < x2): ");
        if (scanf("%lf %lf", x1, x2) != 2) {
            if (feof(stdin)) { printf("Input stream ended.\n"); exit(1); }
            clear_stdin();
            *x1 = *x2 = 0.0;
            continue;
        }
        if (*x1 >= *x2) {
            printf("Error: x1 must be strictly less than x2.\n");
            continue;
        }
        valid = interval_ok(*x1, *x2);
        if (!valid)
            printf("Please enter another interval.\n");
    } while (!valid);

    do {
        printf("Enter eps (%.5f <= eps <= %.3f): ", EPS_MIN, EPS_MAX);
        int got = scanf("%lf", eps);
        if (got == EOF) { printf("Input stream ended.\n"); exit(1); }
        if (got != 1) { clear_stdin(); *eps = 0.0; }
        if (*eps < EPS_MIN || *eps > EPS_MAX)
            printf("Error: eps must be within [%.5f; %.3f].\n", EPS_MIN, EPS_MAX);
    } while (*eps < EPS_MIN || *eps > EPS_MAX);

    do {
        printf("Enter max_iter (iteration batch limit, >= 1): ");
        int got = scanf("%ld", max_iter);
        if (got == EOF) { printf("Input stream ended.\n"); exit(1); }
        if (got != 1 || *max_iter < 1) { clear_stdin(); *max_iter = 0; }
        if (*max_iter < 1)
            printf("Error: max_iter must be at least 1.\n");
    } while (*max_iter < 1);

    do {
        printf("Enter debug mode (1 - show each iteration, 0 - hidden): ");
        int got = scanf("%d", debug);
        if (got == EOF) { printf("Input stream ended.\n"); exit(1); }
        if (got != 1) { clear_stdin(); *debug = -1; }
        if (*debug != 0 && *debug != 1)
            printf("Error: enter 0 or 1.\n");
    } while (*debug != 0 && *debug != 1);

    clear_stdin();
}

/* Очищення буфера вводу при некоректних даних */
static void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
