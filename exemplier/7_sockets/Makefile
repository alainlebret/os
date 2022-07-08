src = $(wildcard *.c)
obj = $(src:.c=.o)

CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -pedantic -O2
LDFLAGS=
BIN= rotn_server test_sockets testbind1 testbind2 testgethostbyname

all: $(BIN)

rotn_server: rotn_server.o
	$(CC) $< -o $@ $(LDFLAGS)

test_sockets: test_sockets.o
	$(CC) $< -o $@ $(LDFLAGS)

testbind1: testbind1.o
	$(CC) $< -o $@ $(LDFLAGS)

testbind2: testbind2.o
	$(CC) $< -o $@ $(LDFLAGS)

testgethostbyname: testgethostbyname.o
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm *.o

distclean: clean
	rm $(BIN)

.PHONY: all clean distclean
