# Source and Object file directories
SRC_DIR = src
OBJ_DIR = src
BIN_DIR = bin

# Automatic source file discovery
src = $(wildcard $(SRC_DIR)/*.c)
# List of executables
bins = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(src))

# GTK programs are built only when GTK 3 is installed (pkg-config gtk+-3.0)
HAVE_GTK := $(shell pkg-config --exists gtk+-3.0 && echo yes)
ifneq ($(HAVE_GTK), yes)
bins := $(filter-out $(BIN_DIR)/moving_window, $(bins))
$(info GTK 3 not found: moving_window is skipped)
endif

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2
LDFLAGS =
COLOR_PROGS_CFLAGS = `pkg-config --cflags gtk+-3.0`
COLOR_PROGS_LDFLAGS = `pkg-config --libs gtk+-3.0`

all: directories $(bins)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR)

# Specific rule for moving_window file
$(BIN_DIR)/moving_window: $(SRC_DIR)/moving_window.c
	$(CC) $(CFLAGS) $(COLOR_PROGS_CFLAGS) -c $< -o $(OBJ_DIR)/moving_window.o
	$(CC) $(OBJ_DIR)/moving_window.o -o $@ $(LDFLAGS) $(COLOR_PROGS_LDFLAGS)

# General rules (moving_window has its own explicit rule above, which wins)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	$(CC) $< -o $@ $(LDFLAGS)

# Programs that include attendre.h are rebuilt when it changes
$(patsubst %.c,%.o,$(shell grep -l '"attendre.h"' $(SRC_DIR)/*.c)): $(SRC_DIR)/attendre.h

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -rf $(BIN_DIR)

.PHONY: all clean distclean
