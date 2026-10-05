# Source and Object file directories
SRC_DIR = src
OBJ_DIR = src
BIN_DIR = bin

# Automatic source and object file discovery
src = $(wildcard $(SRC_DIR)/*.c)
obj = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(src))

CC = gcc
# -std=c11 hides the POSIX API under Linux: _DEFAULT_SOURCE restores it
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2 -D_DEFAULT_SOURCE
LDFLAGS =

BIN = lock_file

all: directories $(BIN_DIR)/$(BIN)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(BIN): $(obj)
	$(CC) $^ -o $@ $(LDFLAGS)

# Rule to create object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -rf $(BIN_DIR)

.PHONY: all clean distclean
