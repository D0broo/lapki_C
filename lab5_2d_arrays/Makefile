# Makefile для лабораторної роботи 5 (2D динамічні матриці double, Варіант 10).
# Потрібен GCC. На Windows (MSYS2 UCRT64): mingw32-make
# На Linux/macOS: make

CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic -O2
TARGET = lab5.exe

.PHONY: all clean run

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ main.c -lm

run: $(TARGET)
	./$(TARGET)

clean:
ifeq ($(OS),Windows_NT)
	del /f /q $(TARGET)
else
	rm -f $(TARGET)
endif
