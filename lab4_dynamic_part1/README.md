# Лабораторна робота 4, частина 1 — одномірні динамічні масиви (Варіант 10)

**Тема:** робота з одновимірними динамічними масивами, передавання їх у функції, обчислення сум, добутків, середнього, мінімуму та максимуму.

**Варіант 10 (парний).** Для цієї частини номер варіанта визначає рекомендований розмір масивів: під час захисту та в звіті використовується `Size = 10`. Самі формули заповнення від розміру не залежать, тому програма коректно працює для будь-якого `Size` з діапазону `1..1000000`.

Джерело: [`main.c`](main.c). Збірка — [`Makefile`](Makefile).

## 1. Постановка задачі

1. Запитати в користувача розмір `Size` (для Варіанта 10 — `10`).
2. Виділити динамічно:
   - масив `A` типу `unsigned int` через `calloc(Size, sizeof(unsigned int))`;
   - масив `B` типу `float` через `malloc(Size * sizeof(float))`.
3. Перевірити, що `A != NULL && B != NULL`. У разі невдачі — звільнити вже виділене, вивести повідомлення про помилку та завершитися з ненульовим кодом.
4. Заповнити масиви **формулами** (без введення елементів з клавіатури):

   $$A[j] = Size - (j + 1), \qquad B[j] = 5.0 \cdot j + 5.0, \qquad j = 0,\ldots,Size-1.$$

5. Вивести обидва масиви, обчислити та надрукувати:
   - `sum(A)` — суму елементів `A`;
   - `product(B)` — добуток елементів `B` (у `double`, щоб уникнути переповнення `float`);
   - `mean_value(A)` — середнє арифметичне `A`;
   - `find_min(A)` — мінімум `A`;
   - `find_max(B)` — максимум `B`.
6. Звільнити `A` і `B` перед `return 0`.

## 2. Обов'язкові сигнатури функцій

```c
void fill_int_array(unsigned int *A, unsigned int Size);      /* A[j] = Size - (j + 1) */
void fill_float_array(float *B, unsigned int Size);           /* B[j] = 5.0f * j + 5.0f */

void print_int_array(const unsigned int *uiptr, unsigned int size_of_array);
void print_float_array(const float *fptr, unsigned int size_of_array);

unsigned int sum(const unsigned int *uiptr, unsigned int size_of_array);
double product(const float *fptr, unsigned int size_of_array);

double mean_value(const unsigned int *A, unsigned int Size);
unsigned int find_min(const unsigned int *A, unsigned int Size);
float find_max(const float *B, unsigned int Size);
```

Усі функції отримують масиви **через параметри-покажчики**; глобальних змінних немає. Для читання масивів використано `const`-кваліфіковані покажчики, що документує незмінність даних усередині цих функцій.

## 3. Алгоритми

### 3.1. Заповнення

```c
for (j = 0U; j < Size; ++j) {
    A[j] = Size - (j + 1U);      /* спадний ряд: Size-1, Size-2, ..., 0 */
    B[j] = 5.0f * (float)j + 5.0f;
}
```

Для `Size = 10`: `A = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}`, `B = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50}`.

### 3.2. Сума, добуток, середнє

Сума та добуток — один прохід по масиву:

```c
total = 0U;
for (j = 0U; j < Size; ++j) total += A[j];
```

```c
result = 1.0;
for (j = 0U; j < Size; ++j) result *= (double)B[j];
```

Добуток накопичується в `double`: для `Size = 10` це $5^{10}\cdot 10! = 3.54375\cdot 10^{13}$, що виходить за межі точного представлення `float`, але точно представляється в `double`.

Середнє:

$$\text{mean} = \frac{1}{Size}\sum_{j=0}^{Size-1} A[j].$$

### 3.3. Мінімум і максимум

Класичний одновимірний пошук: початкове значення — `A[0]` (відповідно `B[0]`), далі один прохід із порівнянням кожного наступного елемента. Складність — $O(Size)$.

### 3.4. Перевірка результатів для `Size = 10`

| Величина | Формула | Значення |
| --- | --- | --- |
| `sum(A)` | $0+1+\dots+9$ | **45** |
| `product(B)` | $5^{10}\cdot 10!$ | **3.543750·10¹³** |
| `mean_value(A)` | $45/10$ | **4.5** |
| `find_min(A)` | $\min\{9,\dots,0\}$ | **0** |
| `find_max(B)` | $\max\{5,\dots,50\}$ | **50** |

## 4. Перевірка введення та безпека пам'яті

Введення розміру реалізовано через `fgets` + `strtol` з перевіркою всього рядка:

- порожній рядок, нечислове значення, залишок символів після числа або вихід за діапазон `1..1000000` — виводиться повідомлення і запит повторюється;
- `EOF` — коректне завершення без зациклення;
- після невдалого `calloc`/`malloc` обидва вказівники звільняються (`free(NULL)` безпечний), програма друкує `Error: memory allocation failed.` у `stderr` і повертає `1`;
- після роботи обидва масиви звільняються — витоків немає.

Приклад реального запуску з некоректними вхідними даними (`abc`, `0`, потім `10`):

```text
Laboratory work 4, part 1 (Variant 10).
Enter array size (1..1000000, e.g. 10): Invalid input; enter a number in [1; 1000000].
Enter array size (1..1000000, e.g. 10): Invalid input; enter a number in [1; 1000000].
Enter array size (1..1000000, e.g. 10): Array A (10 element(s)):
  A[0] = 9
  A[1] = 8
  ...
```

## 5. Реальний вивід програми (`Size = 10`)

```text
Laboratory work 4, part 1 (Variant 10).
Enter array size (1..1000000, e.g. 10): Array A (10 element(s)):
  A[0] = 9
  A[1] = 8
  A[2] = 7
  A[3] = 6
  A[4] = 5
  A[5] = 4
  A[6] = 3
  A[7] = 2
  A[8] = 1
  A[9] = 0
Array B (10 element(s)):
  B[0] = 5.00
  B[1] = 10.00
  B[2] = 15.00
  B[3] = 20.00
  B[4] = 25.00
  B[5] = 30.00
  B[6] = 35.00
  B[7] = 40.00
  B[8] = 45.00
  B[9] = 50.00

Results:
  sum(A)        = 45
  product(B)    = 3.543750e+13
  mean_value(A) = 4.500000
  find_min(A)   = 0
  find_max(B)   = 50.00
```

## 6. Збірка та запуск

```bash
mingw32-make            # або: make
mingw32-make run
mingw32-make clean
```

Вручну:

```bash
gcc -std=c11 -Wall -Wextra -Werror -pedantic -O2 -o lab4_1.exe main.c
./lab4_1.exe
```

Компілятор: GCC, стандарт C11. Проєкт збирається **без жодного попередження** за `-Wall -Wextra -Werror -pedantic`.
