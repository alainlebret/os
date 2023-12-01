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
CFLAGS = -Wall -Wextra -pedantic -O2
LDFLAGS = -lpthread

ifeq ($(shell uname -s),Darwin)
LDFLAGS+=
else
LDFLAGS+=-lrt
endif

# Add the barrier executable
all_bins = $(bins) $(BIN_DIR)/barrier

all: directories $(all_bins)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR) $(INC_DIR)

BARRIER_OBJ=$(OBJ_DIR)/barrier.o

ifeq ($(shell uname -s),Darwin)
BARRIER_OBJ+=$(OBJ_DIR)/pthread_barrier.o
else
BARRIER_OBJ=
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
	rm -f $(OBJ_DIR)/*.o $(all_bins)

distclean: clean

.PHONY: all clean distclean
