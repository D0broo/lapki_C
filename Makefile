# Makefile for the labs of D0broo/lapki_C (Variant 10).
# Requires GCC. On Windows (MSYS2 UCRT64) run with:  mingw32-make lab4_1
# On Linux/macOS:                                    make lab4_1

CC       = gcc
CFLAGS   = -std=c99 -Wall -Wextra -O2
CFLAGS11 = -std=c11 -Wall -Wextra -Werror -pedantic -O2
LDLIBS   = -lm

LAB1_DIR = lapka_1-2
LAB2_DIR = lapka_2
LAB3_DIR = lapka_3
LAB4_1_DIR = lab4_dynamic_part1
LAB4_2_DIR = lab4_dynamic_part2
LAB4_F_DIR = lab4_static_filtering
LAB5_DIR   = lab5_2d_arrays

FILTER_SRCS = $(LAB4_F_DIR)/main.c $(LAB4_F_DIR)/init_array.c \
              $(LAB4_F_DIR)/print_array.c $(LAB4_F_DIR)/init_filter.c \
              $(LAB4_F_DIR)/weighted_filter.c $(LAB4_F_DIR)/impulse_noise.c \
              $(LAB4_F_DIR)/median_filter.c

TARGETS = lab1 lab2 lab3 lab4_1 lab4_2 lab4_filter lab5

.PHONY: all $(TARGETS) clean

all: $(TARGETS)

lab1: $(LAB1_DIR)/lab1.exe
lab2: $(LAB2_DIR)/lab2.exe
lab3: $(LAB3_DIR)/lab3.exe
lab4_1: $(LAB4_1_DIR)/lab4_1.exe
lab4_2: $(LAB4_2_DIR)/lab4_2.exe
lab4_filter: $(LAB4_F_DIR)/lab4_filter.exe
lab5: $(LAB5_DIR)/lab5.exe

$(LAB1_DIR)/lab1.exe: $(LAB1_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

$(LAB2_DIR)/lab2.exe: $(LAB2_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

$(LAB3_DIR)/lab3.exe: $(LAB3_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

$(LAB4_1_DIR)/lab4_1.exe: $(LAB4_1_DIR)/main.c
	$(CC) $(CFLAGS11) -o $@ $<

$(LAB4_2_DIR)/lab4_2.exe: $(LAB4_2_DIR)/main.c
	$(CC) $(CFLAGS11) -o $@ $<

$(LAB4_F_DIR)/lab4_filter.exe: $(FILTER_SRCS) $(LAB4_F_DIR)/filter.h
	$(CC) $(CFLAGS11) -o $@ $(FILTER_SRCS)

$(LAB5_DIR)/lab5.exe: $(LAB5_DIR)/main.c
	$(CC) $(CFLAGS11) -o $@ $< $(LDLIBS)

clean:
ifeq ($(OS),Windows_NT)
	del /f /q $(LAB1_DIR)\lab1.exe $(LAB2_DIR)\lab2.exe $(LAB3_DIR)\lab3.exe $(LAB4_1_DIR)\lab4_1.exe $(LAB4_2_DIR)\lab4_2.exe $(LAB4_F_DIR)\lab4_filter.exe $(LAB5_DIR)\lab5.exe
else
	rm -f $(LAB1_DIR)/lab1.exe $(LAB2_DIR)/lab2.exe $(LAB3_DIR)/lab3.exe \
	      $(LAB4_1_DIR)/lab4_1.exe $(LAB4_2_DIR)/lab4_2.exe \
	      $(LAB4_F_DIR)/lab4_filter.exe $(LAB5_DIR)/lab5.exe
endif
