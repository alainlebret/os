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
COLOR_PROGS_CFLAGS = `pkg-config --cflags gtk+-3.0`
COLOR_PROGS_LDFLAGS = `pkg-config --libs gtk+-3.0`

# libraries to link (-lXXX ex: -lm for maths)
# Detect OS
UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  LDFLAGS=-lpthread
else
  LDFLAGS=-lrt -lpthread
endif

all: directories $(bins)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR) $(INC_DIR)

# Rule for color_displayer file
$(BIN_DIR)/color_displayer: $(SRC_DIR)/color_displayer.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(COLOR_PROGS_CFLAGS) -c $< -o $(OBJ_DIR)/color_displayer.o
	$(CC) $(OBJ_DIR)/color_displayer.o -o $@ $(LDFLAGS) $(COLOR_PROGS_LDFLAGS)

# Rule for color_modifier file
$(BIN_DIR)/color_modifier: $(SRC_DIR)/color_modifier.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(COLOR_PROGS_CFLAGS) -c $< -o $(OBJ_DIR)/color_modifier.o
	$(CC) $(OBJ_DIR)/color_modifier.o -o $@ $(LDFLAGS) $(COLOR_PROGS_LDFLAGS)

# General rule to create object files (excluding color_displayer.c and color_modifier.c)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
ifeq ($<, $(SRC_DIR)/color_displayer.c)
	@echo "Skipping color_displayer.c for general rule"
else 
ifeq ($<, $(SRC_DIR)/color_modifier.c)
	@echo "Skipping color_modifier.c for general rule"
else
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
endif
endif

# Pattern rule for other executables (excluding color_displayer and color_modifier)
$(BIN_DIR)/%: $(OBJ_DIR)/%.o
ifeq ($<, $(OBJ_DIR)/color_displayer.o)
	@echo "Skipping color_displayer.o for general pattern rule"
else 
ifeq ($<, $(OBJ_DIR)/color_modifier.o)
	@echo "Skipping color_modifier.o for general pattern rule"
else
	$(CC) $< -o $@ $(LDFLAGS)
endif
endif

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -f $(bins)

.PHONY: all clean distclean
