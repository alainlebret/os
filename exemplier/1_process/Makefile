src = $(wildcard process/*.c signal/*.c)
obj = $(src:.c=.o)

CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -pedantic -O2
LDFLAGS=
BIN= process/process_01 process/process_02 process/process_03 process/process_04a \
     process/process_04b process/process_05 process/process_06a process/process_06b \
     process/process_07 signal/signal_01 signal/signal_02 signal/signal_03 \
     signal/signal_04 signal/signal_05 signal/signal_06 signal/signal_07 \
     signal/signal_08 signal/signal_09

all: $(BIN)

process/process_01: process/process_01.o
	$(CC) $< -o $@ $(LDFLAGS)

process/process_02: process/process_02.o
	$(CC) $< -o $@ $(LDFLAGS)

process/process_03: process/process_03.o
	$(CC) $< -o $@ $(LDFLAGS)

process/process_04a: process/process_04a.o
	$(CC) $< -o $@ $(LDFLAGS)

process/process_04b: process/process_04b.o
	$(CC) $< -o $@ $(LDFLAGS)

process/process_05: process/process_05.o
	$(CC) $< -o $@ $(LDFLAGS)

process/process_06a: process/process_06a.o
	$(CC) $< -o $@ $(LDFLAGS)

process/process_06b: process/process_06b.o
	$(CC) $< -o $@ $(LDFLAGS)

process/process_07: process/process_07.o
	$(CC) $< -o $@ $(LDFLAGS)

signal/signal_01: signal/signal_01.o
	$(CC) $< -o $@ $(LDFLAGS)

signal/signal_02: signal/signal_02.o
	$(CC) $< -o $@ $(LDFLAGS)

signal/signal_03: signal/signal_03.o
	$(CC) $< -o $@ $(LDFLAGS)

signal/signal_04: signal/signal_04.o
	$(CC) $< -o $@ $(LDFLAGS)

signal/signal_05: signal/signal_05.o
	$(CC) $< -o $@ $(LDFLAGS)

signal/signal_06: signal/signal_06.o
	$(CC) $< -o $@ $(LDFLAGS)

signal/signal_07: signal/signal_07.o
	$(CC) $< -o $@ $(LDFLAGS)

signal/signal_08: signal/signal_08.o
	$(CC) $< -o $@ $(LDFLAGS)

signal/signal_09: signal/signal_08.o
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm process/*.o signal/*.o

distclean: clean
	rm $(BIN)

.PHONY: all clean distclean
