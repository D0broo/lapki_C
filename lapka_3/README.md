# Лабораторна робота 3 — Solving a Nonlinear Equation f(x) = 0 (Variant 10)

**Task (Variant 10):** find the root of the nonlinear equation $f(x) = 0$ by the **chord (regula falsi)** method and the **bisection (half-division)** method, starting from an interval $[x_1, x_2]$ that is validated for a sign change and monotonicity, with a per-batch user control menu, debug tracing and `clock()` timing.

The function is taken from Lab work 1 (Variant 10):

$$f(x) = 0.25\,(x-25)^2 + \frac{(x+25)^3}{100} - 1, \qquad f'(x) = 0.5\,(x-25) + \frac{3\,(x+25)^2}{100}$$

Source: [`lapka_3/main.c`](main.c). All numbers below are **real program output** (compiled with `gcc -std=c99 -Wall -Wextra -O2`, zero warnings), cross-checked against an independent high-precision bisection (true root: **X = −96.8676105235** with f(X) = 9·10⁻¹³).

## 1. Mathematical background

**Conditions for a unique root on [x₁; x₂]:** ① $f(x_1)\cdot f(x_2) < 0$ (sign change) and ② f is strictly monotone on the interval ($f'(x)$ keeps a constant sign). For this f, $f'(x) = 0$ at $x \approx -63.3796$ and $x \approx -3.2871$, so $f' > 0$ left of −63.38, $f' < 0$ between −63.38 and −3.29, and $f' > 0$ right of −3.29. Any interval containing one of these zeros is non-monotone and must be rejected.

**Bisection:** compute the midpoint $x_i = (x_1 + x_2)/2$ and keep the half containing the sign change (if $f(x_1)\cdot f(x_i) < 0$ then $x_2 = x_i$, else $x_1 = x_i$). Every iteration halves the bracket: after $k$ iterations the width is $(b-a)/2^k$, so the guaranteed error bound is $|X - x_i| \le (b-a)/2^{k+1}$. Stopping condition: $|x_1 - x_2| \le \varepsilon$ **or** $|f(x_i)| \le \varepsilon$.

**Chord (regula falsi):** $x_i$ is the root of the secant through the current bracket ends:

$$x_i = \frac{f(x_2)\,x_1 - f(x_1)\,x_2}{f(x_2) - f(x_1)}$$

The bracket is updated exactly as in bisection (keep the sign change); the endpoint whose side never changes stays fixed. Stopping condition: $|f(x_i)| \le \varepsilon$. For this f (concave down on all working intervals, since $f''(x) = 0.5 + 0.06\,(x+25) < 0$ for $x < -33.3$) the left endpoint $x_1$ stays fixed and the iterates converge monotonically to the root from the right.

## 2. Algorithm schemes

### 2.1 Chord method

```text
+--------------------------------------------------------------------+
|                               START                                |
|                 Input: x1, x2, eps, max_iter, debug                |
+--------------------------------------------------------------------+
                                 |
                                 v
                  +-----------------------------+
                  |   Validate [x1; x2]:        |
                  |   f(x1)*f(x2) < 0  ?        |---no---> print rejection
                  |   sign of f' constant ?     |          reason, re-enter
                  +-----------------------------+          x1, x2
                                 | yes
                                 v
         iters = 0;  limit = max_iter;  T0 = clock()
                                 |
                                 v
   +---------------------------------------------------------------+
   |                    ITERATION  (loop)                          |
   |   xi = (f(x2)*x1 - f(x1)*x2) / (f(x2) - f(x1))                |
   |   fi = f(xi);   iters = iters + 1                             |
   |   debug mode 1: print iters, xi, fi                           |
   +---------------------------------------------------------------+
                                 |
                                 v
                       |f(xi)| <= eps ? ---- yes ----+
                                 |                   v
                                 no          X = xi; print X, f(X),
                                 |           iters, clock()-T0; STOP
                                 v
              f(x1)*fi < 0 ?   x2 = xi   :   x1 = xi
                                 |
                                 v
             limit > 0  and  iters % limit == 0  ?
                    | yes                    | no
                    v                        +---------> next ITERATION
          +---------------------------+
          |       PAUSE MENU          |
          | 1: another max_iter batch |---------> next ITERATION
          | 2: limit = 0 (unlimited)  |---------> next ITERATION
          | 3: X = xi, f(X) = fi      |---------> STOP (abort, print
          +---------------------------+           intermediate)
```

### 2.2 Bisection method

Same input, validation, update and pause-menu block as above; the iteration block differs:

```text
   +---------------------------------------------------------------+
   |                    ITERATION  (loop)                          |
   |   |x1 - x2| <= eps ? ---- yes ------------------+             |
   |                                                 v             |
   |                                    X = (x1+x2)/2; STOP (width) |
   |   xi = (x1 + x2) / 2;  fi = f(xi);  iters = iters + 1          |
   |   debug mode 1: print iters, xi, fi                           |
   |   |fi| <= eps ?  -- yes --> X = xi; STOP (small residual)     |
   +---------------------------------------------------------------+
                                 |
                                 v
              f(x1)*fi < 0 ?   x2 = xi   :   x1 = xi
                          (then: pause-menu check as in 2.1)
```

## 3. Program features

- **Input validation, one pass:** interval must satisfy $x_1 < x_2$ **and** pass the uniqueness check above (rejection reason printed; interval re-entered); `eps` must lie in `[1e-5; 1e-3]`; `max_iter ≥ 1` (batch length); `debug` is 0 or 1.
- **Batch control menu** (shown after every `max_iter` iterations without convergence): `1` — continue for another `max_iter`; `2` — remove the iteration limit and run to convergence; `3` — abort and print the current intermediate `xi`, `f(xi)`.
- **Graceful end of input (EOF):** if the input stream ends while the pause menu is open, the program reports `Input stream ended - treating as abort.` and terminates cleanly with the intermediate values (needed for scripted/redirected runs).
- **Timing:** `clock()` from `<time.h>`; the pause time is excluded; output in ms (<1 s) or s.
- **Debug mode** prints `iters, xi, f(xi)` at every iteration.

## 4. Convergence study (real runs)

All runs with `eps = 0.00001`, `max_iter = 10`, `debug = 0`, pause-menu answer `2` (unlimited). Iterations counted by the program; time — `clock()`.

| Interval [x₁; x₂] | Width b−a | Method | Root X | Residual f(X) | Iterations | Time |
| --- | --- | --- | --- | --- | --- | --- |
| [−110; −85] | 25 | Chord | −96.8676104788 | 4.205609e−06 | 13 | 0 ms |
| [−110; −85] | 25 | Bisection | −96.8676096201 | 8.493679e−05 | 22 | 0 ms |
| [−100; −94] | 6 | Chord | −96.8676105145 | 8.490110e−07 | 7 | 0 ms |
| [−100; −94] | 6 | Bisection | −96.8676118851 | −1.280045e−04 | 20 | 0 ms |
| [−98; −95] | 3 | Chord | −96.8676105123 | 1.058664e−06 | 5 | 0 ms |
| [−98; −95] | 3 | Bisection | −96.8676099777 | 5.131448e−05 | 19 | 0 ms |
| [−97.1; −96.8] | 0.3 | Chord | −96.8676105165 | 6.585024e−07 | 3 | 0 ms |
| [−97.1; −96.8] | 0.3 | Bisection | −96.8676071167 | 3.202930e−04 | 15 | 0 ms |

All found roots agree with the true root −96.8676105235 to within the requested tolerance (max deviation 3.4·10⁻⁶ ≤ eps). Every method overshoots *fewer* iterations on a narrower starting interval; time is below `clock()`'s 1 ms resolution in all runs (reported as `0 ms`).

**Bisection iteration count vs theory.** The guaranteed estimate is $k = \lceil \log_2 (b-a)/\varepsilon \rceil$ (width criterion):

| Interval | log₂((b−a)/eps) | Theory k | Measured k |
| --- | --- | --- | --- |
| [−110; −85] | 21.25 | 22 | 22 |
| [−100; −94] | 19.19 | 20 | 20 |
| [−98; −95] | 18.19 | 19 | 19 |
| [−97.1; −96.8] | 14.87 | 15 | 15 |

Measured counts match theory **exactly** in all four cases — the residual test never fired first because with $|f'(X_*)| \approx 94$ the residual bound shrinks far slower than the width.

**Chord method observations.** The chord stop is $|f(X)| \le \varepsilon$, so its residual is always ≤ eps (4.2e−06 … 6.6e−07 in the table). Its iteration cost depends on the bracket position: 13 iterations on the wide [−110; −85] (fixed left endpoint at x₁ = −110, far from the root) vs 3 on [−97.1; −96.8]. The debug trace confirms the theory for concave-down f: iterates approach the root monotonically from the right (f(x₁) < 0 endpoint stays fixed), while bisection midpoints alternate in sign.

## 5. Interval validation: real rejection examples

| Interval | f(x₁)·f(x₂) | Sign change? | f' one sign? | Program verdict (verbatim) |
| --- | --- | --- | --- | --- |
| [−90; −60] | 7.694635e+05 | no | no (f' changes around −62.5) | rejected — both conditions |
| [−100; −50] | &lt; 0 | yes | no | rejected — only monotonicity |
| [−60; −10] | 4.666335e+05 | no | yes (decreasing) | rejected — only sign change |
| [−97.05; −96.9] | 5.244278e+01 | no (root outside!) | yes | rejected — only sign change |
| [−98; −85] | &lt; 0 | yes | yes | **accepted** → solved, see below |

For the accepted [−98; −85] the run continued to convergence: chord −96.8676104393, f = 7.921991e−06, 5 iterations; bisection −96.8676097393, f = 7.372935e−05, 21 iterations (theory: ⌈log₂(13/eps)⌉ = 21 ✓). The narrow [−97.05; −96.9] rejection is instructive: both endpoints lie *left* of the root (f(−97.05) = −17.21, f(−96.9) = −3.05), so bisection cannot bracket it — exactly what the sign test catches.

## 6. Debug mode and batch control (real excerpts)

Debug run `[-98; -95]`, eps = 1e−05, max_iter = 5, debug = 1 (chord converges at iteration 5; bisection pauses at 5, user answers `2`, continues to 19):

```text
[debug] it 1: x = -96.824265975820, f(x) = 4.071449e+00
[debug] it 2: x = -96.866631490112, f(x) = 9.204180e-02
[debug] it 3: x = -96.867588423666, f(x) = 2.077714e-03
[debug] it 4: x = -96.867610024678, f(x) = 4.689990e-05
[debug] it 5: x = -96.867610512274, f(x) = 1.058664e-06
[debug] it 1: x = -96.500000000000, f(x) = 3.430375e+01   <-- bisection starts
...
Iteration limit reached: 5 iterations (limit 5) without convergence.
Current intermediate: x = -96.781250000000, f(x) = 8.104958e+00
  1 - Continue for another 5 iterations
  2 - Run until convergence without iteration limits
  3 - Abort and print current intermediate value
Choice: [debug] it 6: x = -96.828125000000, f(x) = 3.709252e+00   <-- answer "2"
...
[debug] it 19: x = -96.867612838745, f(x) = -2.176640e-04

=== Bisection method ===
Root X        : -96.8676099777
Residual f(X) : 5.131448e-05
Iterations    : 19
Elapsed time  : 0 ms
```

Chord shrinks its residual roughly ×20 per iteration here (4.07 → 9.2e−02 → 2.1e−03 → 4.7e−05 → 1.1e−06) — visibly faster than bisection's ×0.5-per-iteration shrink of the bracket, which also explains why chord needed only 5 iterations from the same interval.

Abort demo `[-110; -85]`, eps = 1e−05, max_iter = 10: chord's menu answered `3` (aborted at iteration 10), then bisection's menu met the end of redirected input and both terminated cleanly with intermediates:

```text
=== Chord method ===
Root X        : -96.8676064094
Residual f(X) : 3.867923e-04
Iterations    : 10
Aborted by user; X and f(X) above are the current intermediate values.

=== Bisection method ===
Root X        : -96.8896484375
Residual f(X) : -2.072816e+00
Iterations    : 10
Aborted by user; X and f(X) above are the current intermediate values.
```

## 7. Control questions

**Q1. What is the root of a nonlinear equation, and when is it unique on an interval?**
A root (solution) of $f(x)=0$ is a number $X_*$ with $f(X_*)=0$. On $[x_1;x_2]$ the root exists and is unique if $f(x_1)\cdot f(x_2)<0$ and $f$ is strictly monotone there ($f'$ keeps one sign). The program checks both conditions before solving.

**Q2. Why must the interval be validated before iterating?**
Without a sign change the iterations have no bracket to shrink and may diverge or "converge" to a non-root; without monotonicity there may be several roots in the interval and the methods may converge to an unrequested one or oscillate between brackets. Real examples in §5 show both failures caught.

**Q3. Describe the bisection method and its guaranteed accuracy.**
Compute the midpoint $x_i=(x_1+x_2)/2$; keep the half with the sign change; each iteration halves the bracket. The error after $k$ iterations satisfies $|X_*-x_i|\le (b-a)/2^{k+1}$; convergence is unconditional once the interval is valid.

**Q4. How many bisection iterations guarantee accuracy eps?**
From $(b-a)/2^k \le \varepsilon$: $k \ge \log_2\big((b-a)/\varepsilon\big)$. E.g. for [−110; −85], eps = 1e−5: $\log_2 2.5\cdot 10^6 = 21.25 \Rightarrow k = 22$ — exactly the measured count (see §4 table; 20/19/15 for the other intervals, all matching).

**Q5. Describe the chord method: formula, geometry, stopping test.**
Each iteration replaces the function by the secant (chord) through the current bracket ends and takes its zero: $x_i = \frac{f(x_2)x_1 - f(x_1)x_2}{f(x_2)-f(x_1)}$; the bracket then keeps the half with the sign change. With $f''$ of constant sign one endpoint stays fixed (here the left one, since $f(x_1)<0$ and $f''<0$) and iterates converge monotonically from the other side. Stop when $|f(x_i)| \le \varepsilon$.

**Q6. Write a C function computing the minimum of four numbers.**

```c
double min4(double a, double b, double c, double d) {
    double m = a;

    if (b < m) m = b;
    if (c < m) m = c;
    if (d < m) m = d;

    return m;
}
```

(A one-expression alternative: `return fmin(a, fmin(b, fmin(c, d)));` with `<math.h>`. The explicit form above uses no library calls and compares sequentially, matching the style of the rest of the lab.)

## 8. Build and run

```bash
gcc -std=c99 -Wall -Wextra -O2 -o lapka_3/lab3.exe lapka_3/main.c -lm
./lapka_3                                            # interactive: interval, eps,
                                                     # max_iter, debug, menu answers
```

All report numbers were produced from real runs of this exact binary (`lapka_3/main.c`, gcc 15.1.0 / MSYS2 UCRT64, `-Wall -Wextra` clean).
