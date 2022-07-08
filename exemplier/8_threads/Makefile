src = $(wildcard *.c)
obj = $(src:.c=.o)

CC=gcc
CPPFLAGS=-DMUTEX -D_REENTRANT
CFLAGS=-Wall -Wextra -pedantic -O2

ifeq ($(uname -s), Darwin)
LDFLAGS=-lpthread
else
LDFLAGS=-lrt -lpthread
endif

BIN= barrier pb_reentrant test_fork test_pthread thread_01 thread_02 thread_02_withmutex \
     thread_with_conditions thread_with_shm_and_mutex thread_with_shm_without_mutex \
     thread_with_signal without_pb_reentrant

all: $(BIN)

barrier: barrier.o
	$(CC) $< -o $@ $(LDFLAGS)

pb_reentrant: pb_reentrant.o
	$(CC) $< -o $@ $(LDFLAGS)

without_pb_reentrant: without_pb_reentrant.o
	$(CC) $< -o $@ $(LDFLAGS)

test_fork: test_fork.o
	$(CC) $< -o $@ $(LDFLAGS)

test_pthread: test_pthread.o
	$(CC) $< -o $@ $(LDFLAGS)

thread_01: thread_01.o
	$(CC) $< -o $@ $(LDFLAGS)

thread_02: thread_02.o
	$(CC) $< -o $@ $(LDFLAGS)

thread_02_withmutex: thread_02_withmutex.o
	$(CC) $< -o $@ $(LDFLAGS)

thread_with_conditions: thread_with_conditions.o
	$(CC) $< -o $@ $(LDFLAGS)

thread_with_shm_and_mutex: thread_with_shm_and_mutex.o
	$(CC) $< -o $@ $(LDFLAGS)

thread_with_shm_without_mutex: thread_with_shm_without_mutex.o
	$(CC) $< -o $@ $(LDFLAGS)

thread_with_signal: thread_with_signal.o
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm *.o

distclean: clean
	rm $(BIN)

.PHONY: all clean distclean
