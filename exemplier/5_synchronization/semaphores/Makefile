# Source and Object file directories
SRC_DIR = src
OBJ_DIR = src
BIN_DIR = bin

# Automatic source file discovery
src = $(wildcard $(SRC_DIR)/*.c)
# List of executables
bins = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(src))

CC = gcc
# -std=c11 hides the POSIX API under Linux: _DEFAULT_SOURCE restores it
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2 -D_DEFAULT_SOURCE
LDFLAGS =

# libraries to link (-lXXX ex: -lm for maths)
# Detect OS
UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  LDFLAGS+=
else
  LDFLAGS+=-lrt
endif

all: directories $(bins)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR)

# General rule to create object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for other executables
$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -rf $(BIN_DIR)

.PHONY: all clean distclean
