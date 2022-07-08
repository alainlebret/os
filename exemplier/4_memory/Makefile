src = $(wildcard *.c)
obj = $(src:.c=.o)

CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -pedantic -O2
LDFLAGS=
BIN= mmap1 mmap2 memory_01 memory_02 memory_03 memory_04 memory_05a \
     memory_05b memory_06 memory_07

all: $(BIN)

mmap1: mmap1.o
	$(CC) $< -o $@ $(LDFLAGS)

mmap2: mmap2.o
	$(CC) $< -o $@ $(LDFLAGS)

memory_01: memory_01.o
	$(CC) $< -o $@ $(LDFLAGS)

memory_02: memory_02.o
	$(CC) $< -o $@ $(LDFLAGS)

memory_03: memory_03.o
	$(CC) $< -o $@ $(LDFLAGS)

memory_04: memory_04.o
	$(CC) $< -o $@ $(LDFLAGS)

memory_05a: memory_05a.o
	$(CC) $< -o $@ $(LDFLAGS)

memory_05b: memory_05b.o
	$(CC) $< -o $@ $(LDFLAGS)

memory_06: memory_06.o
	$(CC) $< -o $@ $(LDFLAGS)

memory_07: memory_07.o
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm *.o

distclean: clean
	rm $(BIN)

.PHONY: all clean distclean
