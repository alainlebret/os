src = $(wildcard *.c)
obj = $(src:.c=.o)

CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -pedantic -O2
LDFLAGS=
BIN= process_01 process_02 process_03 process_04a process_04b process_05 \
     process_06a process_06b process_07 process_08 process_09 moving_window

all: $(BIN)

process_01: process_01.o
	$(CC) $< -o $@ $(LDFLAGS)

process_02: process_02.o
	$(CC) $< -o $@ $(LDFLAGS)

process_03: process_03.o
	$(CC) $< -o $@ $(LDFLAGS)

process_04a: process_04a.o
	$(CC) $< -o $@ $(LDFLAGS)

process_04b: process_04b.o
	$(CC) $< -o $@ $(LDFLAGS)

process_05: process_05.o
	$(CC) $< -o $@ $(LDFLAGS)

process_06a: process_06a.o
	$(CC) $< -o $@ $(LDFLAGS)

process_06b: process_06b.o
	$(CC) $< -o $@ $(LDFLAGS)

process_07: process_07.o
	$(CC) $< -o $@ $(LDFLAGS)

process_08: process_08.o
	$(CC) $< -o $@ $(LDFLAGS)

moving_window: moving_window.c
	$(CC) -o $@ $< `pkg-config --cflags --libs gtk+-3.0` -lm

clean:
	rm *.o

distclean: clean
	rm $(BIN)

.PHONY: all clean distclean
