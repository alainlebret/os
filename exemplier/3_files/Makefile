rc = $(wildcard *.c)
obj = $(src:.c=.o)

CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -pedantic -O2
LDFLAGS=
BIN= file_copy file_create file_create_huge file_hexdump fs_block

all: $(BIN)

file_copy: file_copy.o
	$(CC) $< -o $@ $(LDFLAGS)

file_create: file_create.o
	$(CC) $< -o $@ $(LDFLAGS)

file_create_huge: file_createhuge.o
	$(CC) $< -o $@ $(LDFLAGS)

file_hexdump: file_hexdump.o
	$(CC) $< -o $@ $(LDFLAGS)

fs_block: fs_block.o
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm *.o

distclean: clean
	rm $(BIN)

.PHONY: all clean distclean