# Makefile for Laboratory work 4: static 2D array filtering (multi-file).
# Requires GCC. On Windows (MSYS2 UCRT64) run: mingw32-make
# On Linux/macOS: make

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
