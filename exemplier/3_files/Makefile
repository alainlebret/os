# Source and Object file directories
SRC_DIR = src
OBJ_DIR = src
INC_DIR = include
BIN_DIR = bin

# Automatic source file discovery
src = $(wildcard $(SRC_DIR)/*.c)
# List of executables
bins = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(src))

CC = gcc
CPPFLAGS = -I $(INC_DIR)
CFLAGS = -Wall -Wextra -pedantic -O2
LDFLAGS =

all: $(bins)

$(BIN_DIR)/$(BIN): $(obj)
	$(CC) $^ -o $@ $(LDFLAGS)

# General rule to create object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Pattern rule for other executables
$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -f $(BIN_DIR)/$(bins)

.PHONY: all clean distclean
