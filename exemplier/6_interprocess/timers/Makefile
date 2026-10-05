# Source and Object file directories
SRC_DIR = src
OBJ_DIR = src
BIN_DIR = bin

# Automatic source file discovery
src = $(wildcard $(SRC_DIR)/*.c)
# timer_create() and SIGRTMIN do not exist on macOS: Linux-only examples
ifeq ($(shell uname -s),Darwin)
src := $(filter-out $(SRC_DIR)/linux_clock.c $(SRC_DIR)/posix_timer_tic.c, $(src))
endif
# List of executables
bins = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(src))

CC = gcc
# -std=c11 hides the POSIX API (sigaction()...) on Linux: _DEFAULT_SOURCE restores it
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2 -D_DEFAULT_SOURCE
LDFLAGS =
# glibc < 2.34: timer_create() is in librt
ifneq ($(shell uname -s),Darwin)
LDFLAGS += -lrt
endif

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
	rm -rf $(BIN_DIR)

.PHONY: all clean distclean
