# Makefile for Laboratory work 4, part 2 (2D dynamic integer matrices, Variant 10).
# Requires GCC. On Windows (MSYS2 UCRT64) run: mingw32-make
# On Linux/macOS: make

CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic -O2
TARGET = lab4_2.exe

.PHONY: all clean run

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ main.c

run: $(TARGET)
	./$(TARGET)

clean:
ifeq ($(OS),Windows_NT)
	del /f /q $(TARGET)
else
	rm -f $(TARGET)
endif
