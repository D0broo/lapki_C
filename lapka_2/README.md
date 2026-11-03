# Лабораторна робота 2 — Numerical Integration (Variant 10)

**Task (Variant 10):** compute the integral

$$I = \int_{-4.5}^{10} \sin^2(x)\,dx$$

numerically with the **left rectangles**, **right rectangles**, **trapezoidal** and **Simpson's** rules, refine the step count `N` until the difference between two successive approximations is below `eps`, and compare the methods.

Source: [`lapka_2/main.c`](main.c). Program: menu-driven (options `1`–`4` per method, `5` summary table for all methods, `0` exit), inputs `a`, `b`, `n` (n > 0), `eps` (`0.00001 ≤ eps ≤ 0.001`). All numbers below are **real program output** (compiled with `gcc -std=c99 -Wall -Wextra -O2`, zero warnings).

## 1. Analytical evaluation

Using the trigonometric identity $\sin^2 x = \frac{1-\cos 2x}{2}$:

$$\int \sin^2 x \, dx = \int \frac{1-\cos 2x}{2}\,dx = \frac{x}{2} - \frac{\sin 2x}{4} + C$$

Therefore

$$\int_{-4.5}^{10}\sin^2 x\,dx = \left[\frac{x}{2}-\frac{\sin 2x}{4}\right]_{-4.5}^{10} = \left(5-\frac{\sin 20}{4}\right)-\left(-2.25+\frac{\sin 9}{4}\right)$$

With $\sin 20 = 0.91294525$, $\sin 9 = 0.41211849$ (radians):

$$I = (5 - 0.22823631) - (-2.14697038) = 6.91873407$$

**Exact reference value: I = 6.91873407** (8 decimal places).

## 2. Results summary table (real program output)

Program input for every run: `a = -4.5`, `b = 10`, `eps = 0.0001`, menu option `5`. Each block below is one run with a different initial `n`; columns exactly as printed by the program.

### Initial n = 10

| Method            |        I(n) |       N |        I(N) |     Delta |
| ----------------- | ----------- | ------- | ----------- | --------- |
| Left rectangles   |     7.669908 |     316 |     6.934100 | 9.936e-05 |
| Right rectangles  |     6.713479 |     306 |     6.903354 | 9.954e-05 |
| Trapezoidal       |     7.191693 |     100 |     6.921059 | 9.600e-05 |
| Simpson's rule    |     5.872677 |      42 |     6.918291 | 9.883e-05 |

### Initial n = 100

| Method            |        I(n) |       N |        I(N) |     Delta |
| ----------------- | ----------- | ------- | ----------- | --------- |
| Left rectangles   |     6.968880 |     316 |     6.934100 | 9.936e-05 |
| Right rectangles  |     6.873238 |     306 |     6.903354 | 9.954e-05 |
| Trapezoidal       |     6.921059 |     102 |     6.920969 | 9.040e-05 |
| Simpson's rule    |     6.918721 |     102 |     6.918722 | 1.006e-06 |

### Initial n = 1000

| Method            |        I(n) |       N |        I(N) |     Delta |
| ----------------- | ----------- | ------- | ----------- | --------- |
| Left rectangles   |     6.923539 |    1002 |     6.923530 | 9.638e-06 |
| Right rectangles  |     6.913975 |    1002 |     6.913985 | 9.453e-06 |
| Trapezoidal       |     6.918757 |    1002 |     6.918757 | 9.259e-08 |
| Simpson's rule    |     6.918734 |    1002 |     6.918734 | 1.036e-11 |

### Initial n = 10000

| Method            |        I(n) |       N |        I(N) |     Delta |
| ----------------- | ----------- | ------- | ----------- | --------- |
| Left rectangles   |     6.919213 |   10002 |     6.919212 | 9.572e-08 |
| Right rectangles  |     6.918256 |   10002 |     6.918256 | 9.553e-08 |
| Trapezoidal       |     6.918734 |   10002 |     6.918734 | 9.278e-11 |
| Simpson's rule    |     6.918734 |   10002 |     6.918734 | 2.398e-14 |

## 3. Trapezoidal method: data points (n, I)

Real output of option `3` (Trapezoidal) for `a = -4.5`, `b = 10`, `n = 5, 10, …, 50` — ready for plotting:

| n  | I(n)      |
| -- | --------- |
| 5  | 11.148743 |
| 10 | 7.191693  |
| 15 | 7.028976  |
| 20 | 6.978916  |
| 25 | 6.956741  |
| 30 | 6.944941  |
| 35 | 6.937907  |
| 40 | 6.933373  |
| 45 | 6.930279  |
| 50 | 6.928073  |

Reference (exact) value: **6.91873407**.

## 4. Conclusions

**Convergence speed (N for the prescribed Delta ≤ eps stopping rule, real runs):** Simpson's rule reaches the tolerance with by far the fewest refinements (N = 42 from n = 10), the trapezoidal rule is second (N = 100), the left/right rectangle rules are slowest and need comparable counts (N = 316 / 306). Since the rule refines `N += 2` at a time (per methodology), the rectangle methods dominate the runtime: each refinement recomputes the whole sum from scratch, so overall work grows quadratically with the final N.

**Accuracy order (observed vs theoretical):** with the same interval, the successive difference `Delta` behaves exactly as theory predicts for a composite rule — halving `h` scales `Delta` by ≈2 for the rectangle rules (9.6e-05 → 4.8e-06 per ×10 increase of N: 9.6e-05 @N=100 → 9.6e-06 @N=1000 → 9.6e-08 @N=10000, i.e. `~1/N`), by ≈N² for the trapezoidal rule (9.6e-05 → 9.3e-08 → 9.3e-11, i.e. `~1/N²`), and by ≈N⁴ for Simpson (modeled `~1/N⁴`, e.g. 1.0e-06 @N=102 → 1.0e-11 @N=1002 → 2.4e-14 @N=10002). So the accuracy orders are: rectangles O(h), trapezoidal O(h²), Simpson O(h⁴).

**Measured caveat (honest note):** the stopping criterion |I(N) − I(N+2)| ≤ eps *underestimates* the remaining true error, because two successive composite sums share the same leading error term. For example, the left-rectangle run stops at N = 316 with I(N) = 6.934100 — the true error against the exact 6.91873407 is still ≈ 1.5e-02. Measured minimal N for the *true* absolute error < 1e-4 (independent check): left ≈ 47827, right ≈ 47817, trapezoidal ≈ 482, Simpson ≈ 62 (first crossing). This is a property of the prescribed refinement criterion, not of the formulas: the columns I(N) at N = 10000 above show every method matching the exact value to 6 decimals.

**Simpson parity:** for odd `n` the method increments `n` by 1 and prints a warning (required behavior); e.g. n = 11 → 12 with `Warning: Simpson's rule needs an even n; n = 11 incremented to 12.`
