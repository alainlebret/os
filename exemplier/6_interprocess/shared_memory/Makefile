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
bins := $(filter-out $(BIN_DIR)/color_displayer $(BIN_DIR)/color_modifier, $(bins))
$(info GTK 3 not found: color_displayer and color_modifier are skipped)
endif

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2 -D_DEFAULT_SOURCE  # POSIX + BSD APIs with glibc
LDFLAGS =
COLOR_PROGS_CFLAGS = `pkg-config --cflags gtk+-3.0`
COLOR_PROGS_LDFLAGS = `pkg-config --libs gtk+-3.0`

# libraries to link (-lXXX ex: -lm for maths)
# Detect OS
UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  LDFLAGS=-lpthread -lm
else
  LDFLAGS=-lrt -lpthread -lm
endif

all: directories $(bins)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR)

# Rule for color_displayer file
$(BIN_DIR)/color_displayer: $(SRC_DIR)/color_displayer.c
	$(CC) $(CFLAGS) $(COLOR_PROGS_CFLAGS) -c $< -o $(OBJ_DIR)/color_displayer.o
	$(CC) $(OBJ_DIR)/color_displayer.o -o $@ $(LDFLAGS) $(COLOR_PROGS_LDFLAGS)

# Rule for color_modifier file
$(BIN_DIR)/color_modifier: $(SRC_DIR)/color_modifier.c
	$(CC) $(CFLAGS) $(COLOR_PROGS_CFLAGS) -c $< -o $(OBJ_DIR)/color_modifier.o
	$(CC) $(OBJ_DIR)/color_modifier.o -o $@ $(LDFLAGS) $(COLOR_PROGS_LDFLAGS)

# General rules (GTK programs have their own explicit rules above, which win)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	$(CC) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -rf $(BIN_DIR)

.PHONY: all clean distclean
