# Source and Object file directories
SRC_DIR = src
OBJ_DIR = src
INC_DIR = include
BIN_DIR = bin

CC = gcc
CPPFLAGS = -I $(INC_DIR)
CFLAGS = -Wall -Wextra -pedantic -O2
LDFLAGS =
BINS = $(BIN_DIR)/test_message $(BIN_DIR)/unix_msg_recv $(BIN_DIR)/unix_msg_send

ifeq ($(shell uname -s),Darwin)
LDFLAGS+=
else
LDFLAGS+=-lrt
endif

all: directories $(BINS)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR) $(INC_DIR)

$(BIN_DIR)/test_message: $(OBJ_DIR)/test_message.o $(OBJ_DIR)/message.o
	$(CC) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/unix_msg_recv: $(OBJ_DIR)/unix_msg_recv.o $(OBJ_DIR)/message.o
	$(CC) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/unix_msg_send: $(OBJ_DIR)/unix_msg_send.o $(OBJ_DIR)/message.o
	$(CC) $^ -o $@ $(LDFLAGS)

# General rule to create object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -f $(BINS)

.PHONY: all clean distclean
