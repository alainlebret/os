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

# General rule to create object files (excluding moving_window.c)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
ifeq ($<, $(SRC_DIR)/moving_window.c)
	@echo "Skipping moving_window.c for general rule"
else
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
endif

# Pattern rule for other executables (excluding moving_window.c)
$(BIN_DIR)/%: $(OBJ_DIR)/%.o
ifeq ($<, $(OBJ_DIR)/moving_window.o)
	@echo "Skipping moving_window.o for general pattern rule"
else 
ifeq ($<, $(OBJ_DIR)/moving_window.o)
	@echo "Skipping moving_window.o for general pattern rule"
else
	$(CC) $< -o $@ $(LDFLAGS)
endif
endif

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -f $(bins)

.PHONY: all clean distclean
