src = $(wildcard file_locking/*.c semaphores/*.c)
obj = $(src:.c=.o)

CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -pedantic -O2
LDFLAGS=-lrt -lpthread
BIN= without_synchro file_locking/lock_file semaphores/posix_prod_cons semaphores/posix_semaphore \
     semaphores/unix_prod_cons semaphores/unix_semaphore

all: $(BIN)

without_synchro: without_synchro.o
	$(CC) $< -o $@ $(LDFLAGS)

file_locking/lock_file: file_locking/lock_file.o
	$(CC) $< -o $@ $(LDFLAGS)

semaphores/posix_prod_cons: semaphores/posix_prod_cons.o
	$(CC) $< -o $@ $(LDFLAGS)

semaphores/posix_semaphore: semaphores/posix_semaphore.o
	$(CC) $< -o $@ $(LDFLAGS)

semaphores/unix_prod_cons: semaphores/unix_prod_cons.o
	$(CC) $< -o $@ $(LDFLAGS)

semaphores/unix_semaphore: semaphores/unix_semaphore.o
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm *.o file_locking/*.o semaphores/*.o

distclean: clean
	rm $(BIN)

.PHONY: all clean distclean
