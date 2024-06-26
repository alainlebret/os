# Source and Object file directories
SRC_DIR = src
OBJ_DIR = src
BIN_DIR = bin

# Automatic source file discovery
src = $(wildcard $(SRC_DIR)/*.c)
# List of executables
bins = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(src))

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2
LDFLAGS =

all: directories $(bins)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR)

# General rule to create object files (excluding color_changer.c)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for other executables (excluding color_changer)
$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -f $(bins)

.PHONY: all clean distclean
