# Makefile для лабораторної роботи 4: фільтрація статичного 2D масиву (багатофайловий).
# Потрібен GCC. На Windows (MSYS2 UCRT64): mingw32-make
# На Linux/macOS: make

CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic -O2
TARGET = lab4_filter.exe
SRCS   = main.c init_array.c print_array.c init_filter.c \
         weighted_filter.c impulse_noise.c median_filter.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS) filter.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
ifeq ($(OS),Windows_NT)
	del /f /q $(TARGET)
else
	rm -f $(TARGET)
endif
