# Source and Object file directories
SRC_DIR = src
OBJ_DIR = src
INC_DIR = include
BIN_DIR = bin

# Automatic source file discovery (excluding pthread_barrier.c)
src = $(filter-out $(SRC_DIR)/pthread_barrier.c, $(wildcard $(SRC_DIR)/*.c))
# List of executables (excluding barrier)
bins = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(src))

CC = gcc
CPPFLAGS = -I $(INC_DIR)
# -std=c11 hides the POSIX API under Linux: _DEFAULT_SOURCE restores it
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2 -D_DEFAULT_SOURCE -pthread
# -pthread (compile and link) rather than -lpthread alone
LDFLAGS = -pthread

# Add the barrier executable
all_bins = $(bins) $(BIN_DIR)/barrier

all: directories $(all_bins)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR) $(INC_DIR)

BARRIER_OBJ=$(OBJ_DIR)/barrier.o

# macOS has no pthread_barrier_t: link our own implementation
ifeq ($(shell uname -s),Darwin)
BARRIER_OBJ+=$(OBJ_DIR)/pthread_barrier.o
endif

# Special rule for the barrier executable
$(BIN_DIR)/barrier: $(BARRIER_OBJ)
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
	rm -rf $(BIN_DIR)

.PHONY: all clean distclean
