# Makefile for the labs of D0broo/lapki_C (Variant 10).
# Requires GCC. On Windows (MSYS2 UCRT64) run with:  mingw32-make lab2
# On Linux/macOS:                                    make lab2

CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -O2
LDLIBS  = -lm

LAB1_DIR = lapka_1-2
LAB2_DIR = lapka_2
LAB3_DIR = lapka_3

TARGETS = lab1 lab2 lab3

.PHONY: all $(TARGETS) clean

all: $(TARGETS)

lab1: $(LAB1_DIR)/lab1.exe
lab2: $(LAB2_DIR)/lab2.exe
lab3: $(LAB3_DIR)/lab3.exe

$(LAB1_DIR)/lab1.exe: $(LAB1_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

$(LAB2_DIR)/lab2.exe: $(LAB2_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

$(LAB3_DIR)/lab3.exe: $(LAB3_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
ifeq ($(OS),Windows_NT)
	del /f /q lapka_1-2\lab1.exe lapka_2\lab2.exe lapka_3\lab3.exe
else
	rm -f $(LAB1_DIR)/lab1.exe $(LAB2_DIR)/lab2.exe $(LAB3_DIR)/lab3.exe
endif
