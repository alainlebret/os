src = $(wildcard pipes/*.c posix_messages/*.c shared_memory/*.c unix_messages/*.c)
obj = $(src:.c=.o)

CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -pedantic -O2

ifeq ($(shell uname -s), Darwin)
LDFLAGS=
BIN= pipes/anonymous_pipe_01 pipes/anonymous_pipe_02 pipes/mkfifo_client pipes/mkfifo_server \
     pipes/mkfifo_producer pipes/mkfifo_consumer \
     shared_memory/mmap_buffer_01 shared_memory/mmap_buffer_02 shared_memory/mmap_buffer_03 \
     shared_memory/mmap_buffer_04 shared_memory/posix_shm_client shared_memory/posix_shm_server \
     shared_memory/posix_shm_simple_1 shared_memory/posix_shm_simple_2 shared_memory/posix_shm_simple_3 \
     shared_memory/unix_shm shared_memory/unix_shm_consumer shared_memory/unix_shm_producer \
     unix_messages/test_message unix_messages/test_msg_messages unix_messages/unix_msg_recv \
     unix_messages/unix_msg_send
else
LDFLAGS=-lrt
BIN= pipes/anonymous_pipe_01 pipes/anonymous_pipe_02 pipes/mkfifo_client pipes/mkfifo_server \
     pipes/mkfifo_producer pipes/mkfifo_consumer \
     posix_messages/posix_msg_receiver posix_messages/posix_msg_sender \
     shared_memory/mmap_buffer_01 shared_memory/mmap_buffer_02 shared_memory/mmap_buffer_03 \
     shared_memory/mmap_buffer_04 shared_memory/posix_shm_client shared_memory/posix_shm_server \
     shared_memory/posix_shm_simple_1 shared_memory/posix_shm_simple_2 shared_memory/posix_shm_simple_3 \
     shared_memory/unix_shm shared_memory/unix_shm_consumer shared_memory/unix_shm_producer \
     unix_messages/test_message unix_messages/test_msg_messages unix_messages/unix_msg_recv \
     unix_messages/unix_msg_send
endif

all: $(BIN)

pipes/anonymous_pipe_01: pipes/anonymous_pipe_01.o
	$(CC) $< -o $@ $(LDFLAGS)

pipes/anonymous_pipe_02: pipes/anonymous_pipe_02.o
	$(CC) $< -o $@ $(LDFLAGS)

pipes/mkfifo_client: pipes/mkfifo_client.o
	$(CC) $< -o $@ $(LDFLAGS)

pipes/mkfifo_server: pipes/mkfifo_server.o
	$(CC) $< -o $@ $(LDFLAGS)

pipes/mkfifo_producer: pipes/mkfifo_producer.o
	$(CC) $< -o $@ $(LDFLAGS)

posix_messages/posix_msg_receiver: posix_messages/posix_msg_receiver.o
	$(CC) $< -o $@ $(LDFLAGS)

posix_messages/posix_msg_sender: posix_messages/posix_msg_sender.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/mmap_buffer_01: shared_memory/mmap_buffer_01.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/mmap_buffer_02: shared_memory/mmap_buffer_02.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/mmap_buffer_03: shared_memory/mmap_buffer_03.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/mmap_buffer_04: shared_memory/mmap_buffer_04.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/posix_shm_client: shared_memory/posix_shm_client.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/posix_shm_server: shared_memory/posix_shm_server.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/posix_shm_simple_1: shared_memory/posix_shm_simple_1.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/posix_shm_simple_2: shared_memory/posix_shm_simple_2.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/posix_shm_simple_3: shared_memory/posix_shm_simple_3.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/unix_shm: shared_memory/unix_shm.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/unix_shm_consumer: shared_memory/unix_shm_consumer.o
	$(CC) $< -o $@ $(LDFLAGS)

shared_memory/unix_shm_producer: shared_memory/unix_shm_producer.o
	$(CC) $< -o $@ $(LDFLAGS)

unix_messages/test_message: unix_messages/test_message.o unix_messages/message.o
	$(CC) $^ -o $@ $(LDFLAGS)

unix_messages/test_msg_messages: unix_messages/test_msg_messages.o unix_messages/message.o
	$(CC) $^ -o $@ $(LDFLAGS)

unix_messages/unix_msg_recv: unix_messages/unix_msg_recv.o unix_messages/message.o
	$(CC) $^ -o $@ $(LDFLAGS)

unix_messages/unix_msg_send: unix_messages/unix_msg_send.o unix_messages/message.o
	$(CC) $^ -o $@ $(LDFLAGS)

ifeq ($(shell uname -s), Darwin)
clean:
	rm pipes/*.o shared_memory/*.o unix_messages/*.o
else
clean:
	rm pipes/*.o posix_messages/*.o shared_memory/*.o unix_messages/*.o
endif

distclean: clean
	rm $(BIN)

.PHONY: all clean distclean
