# Source and Object file directories
SRC_DIR = src
OBJ_DIR = src
BIN_DIR = bin

# Automatic source and object file discovery
src = $(wildcard $(SRC_DIR)/*.c)
obj = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(src))

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2
LDFLAGS =

BIN = microseconds_sleep

all: directories $(BIN_DIR)/$(BIN)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(BIN): $(obj)
	$(CC) $^ -o $@ $(LDFLAGS)

# Rule to create object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -rf $(BIN_DIR)

.PHONY: all clean distclean
