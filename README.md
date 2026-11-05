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
├── Makefile            # збірка всіх лабораторних однією командою
└── lapka_1-2.slnx
```

## Збірка

Потрібен GCC (MinGW / MSYS2). На Windows (MSYS2 UCRT64) використовуйте `mingw32-make`, на Linux/macOS — `make`:

```bash
mingw32-make          # зібрати всі лабораторні (lab1.exe, lab2.exe, lab3.exe)
mingw32-make lab2     # зібрати одну лабораторну
mingw32-make clean    # вилучити зібрані виконувані файли
```

Або вручну, для кожної лабораторної:

```bash
gcc -std=c99 -Wall -Wextra -O2 -o lapka_1-2/lab1.exe lapka_1-2/main.c -lm
gcc -std=c99 -Wall -Wextra -O2 -o lapka_2/lab2.exe lapka_2/main.c -lm
gcc -std=c99 -Wall -Wextra -O2 -o lapka_3/lab3.exe lapka_3/main.c -lm
```

Всі лабораторні компілюються без єдиного попередження (`-Wall -Wextra`).

## Лабораторні роботи (Варіант 10)

| № | Тема | Код | Звіт |
| - | ---- | --- | ---- |
| 1 | Таблиця значень функції f(x) = 0.25(x−25)² + (x+25)³/100 − 1 на [X1; X2] | [lapka_1-2/main.c](lapka_1-2/main.c) | — |
| 2 | Чисельне інтегрування ∫₋₄.₅¹⁰ x²·sin(x)dx: ліві/праві прямокутники, трапеції, Сімпсон | [lapka_2/main.c](lapka_2/main.c) | [lapka_2/README.md](lapka_2/README.md) |
| 3 | Розв'язання нелінійного рівняння f(x) = 0 методами хорд і бісекції | [lapka_3/main.c](lapka_3/main.c) | [lapka_3/README.md](lapka_3/README.md) |
