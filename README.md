# lapki_C

Лабораторні роботи з мови програмування C (Варіант 10).

## Структура проєкту

```
lapki_C/
├── lapka_1-2/          # Лабораторна 1: таблиця значень f(x) (Visual Studio проєкт)
│   ├── main.c
│   └── lapka_1-2.vcxproj
├── lapka_2/            # Лабораторна 2: чисельне інтегрування x²·sin(x) на [-4.5; 10]
│   ├── main.c
│   └── README.md       # звіт (реальні результати запуску)
├── lapka_3/            # Лабораторна 3: розв'язання нелінійного рівняння f(x) = 0
│   ├── main.c
│   └── README.md       # звіт (реальні результати запуску)
├── lab4_dynamic_part1/ # Лабораторна 4, частина 1: 1D динамічні масиви
│   ├── main.c
│   ├── Makefile
│   └── README.md       # звіт (реальні результати запуску)
├── lab4_dynamic_part2/ # Лабораторна 4, частина 2: 2D динамічні матриці int **
│   ├── main.c
│   ├── Makefile
│   └── README.md       # звіт (реальні результати запуску)
├── lab4_static_filtering/ # Лабораторна 4: багатофайлова фільтрація (N=M=10, K=L=3)
│   ├── filter.h
│   ├── init_array.c
│   ├── print_array.c
│   ├── init_filter.c
│   ├── weighted_filter.c
│   ├── impulse_noise.c
│   ├── median_filter.c
│   ├── main.c
│   ├── Makefile
│   └── README.md       # звіт (реальні результати запуску)
├── lab5_2d_arrays/     # Лабораторна 5: 2D динамічні матриці double **
│   ├── main.c
│   ├── Makefile
│   └── README.md       # звіт (реальні результати запуску)
├── Makefile            # збірка всіх лабораторних однією командою
└── lapka_1-2.slnx
```

## Збірка

Потрібен GCC (MinGW / MSYS2). На Windows (MSYS2 UCRT64) використовуйте `mingw32-make`, на Linux/macOS — `make`:

```bash
mingw32-make          # зібрати всі лабораторні
mingw32-make lab2     # зібрати одну лабораторну
mingw32-make clean    # вилучити зібрані виконувані файли
```

Цілі кореневого `Makefile`: `lab1`, `lab2`, `lab3`, `lab4_1`, `lab4_2`, `lab4_filter`, `lab5`. Кожен проєкт лабораторних 4–5 має ще й власний `Makefile` з тими самими цілями `all`, `run`, `clean`.

Або вручну, для кожної лабораторної:

```bash
gcc -std=c99 -Wall -Wextra -O2 -o lapka_1-2/lab1.exe lapka_1-2/main.c -lm
gcc -std=c99 -Wall -Wextra -O2 -o lapka_2/lab2.exe lapka_2/main.c -lm
gcc -std=c99 -Wall -Wextra -O2 -o lapka_3/lab3.exe lapka_3/main.c -lm

gcc -std=c11 -Wall -Wextra -Werror -pedantic -O2 -o lab4_dynamic_part1/lab4_1.exe lab4_dynamic_part1/main.c
gcc -std=c11 -Wall -Wextra -Werror -pedantic -O2 -o lab4_dynamic_part2/lab4_2.exe lab4_dynamic_part2/main.c
gcc -std=c11 -Wall -Wextra -Werror -pedantic -O2 -o lab4_static_filtering/lab4_filter.exe \
    lab4_static_filtering/main.c lab4_static_filtering/init_array.c \
    lab4_static_filtering/print_array.c lab4_static_filtering/init_filter.c \
    lab4_static_filtering/weighted_filter.c lab4_static_filtering/impulse_noise.c \
    lab4_static_filtering/median_filter.c
gcc -std=c11 -Wall -Wextra -Werror -pedantic -O2 -o lab5_2d_arrays/lab5.exe lab5_2d_arrays/main.c -lm
```

Всі лабораторні компілюються без єдиного попередження (`-Wall -Wextra`); лабораторні 4–5 — ще й з `-Werror -pedantic`.

## Лабораторні роботи (Варіант 10)

| № | Тема | Код | Звіт |
| - | ---- | --- | ---- |
| 1 | Таблиця значень функції f(x) = 0.25(x−25)² + (x+25)³/100 − 1 на [X1; X2] | [lapka_1-2/main.c](lapka_1-2/main.c) | — |
| 2 | Чисельне інтегрування ∫₋₄.₅¹⁰ x²·sin(x)dx: ліві/праві прямокутники, трапеції, Сімпсон | [lapka_2/main.c](lapka_2/main.c) | [lapka_2/README.md](lapka_2/README.md) |
| 3 | Розв'язання нелінійного рівняння f(x) = 0 методами хорд і бісекції | [lapka_3/main.c](lapka_3/main.c) | [lapka_3/README.md](lapka_3/README.md) |
| 4.1 | Одномірні динамічні масиви: сума, добуток, середнє, min і max | [lab4_dynamic_part1/main.c](lab4_dynamic_part1/main.c) | [lab4_dynamic_part1/README.md](lab4_dynamic_part1/README.md) |
| 4.2 | Двовимірні динамічні матриці `int **`: max/min під діагоналлю, транспонування, множення, сортування, суми | [lab4_dynamic_part2/main.c](lab4_dynamic_part2/main.c) | [lab4_dynamic_part2/README.md](lab4_dynamic_part2/README.md) |
| 4.3 | Багатофайлова фільтрація статичного масиву 10×10: зважений фільтр 3×3, імпульсний шум, медіанний фільтр | [lab4_static_filtering/main.c](lab4_static_filtering/main.c) | [lab4_static_filtering/README.md](lab4_static_filtering/README.md) |
| 5 | Двовимірні динамічні матриці `double **`: max усієї A, min під діагоналлю, транспонування, множення, сортування рядка | [lab5_2d_arrays/main.c](lab5_2d_arrays/main.c) | [lab5_2d_arrays/README.md](lab5_2d_arrays/README.md) |
