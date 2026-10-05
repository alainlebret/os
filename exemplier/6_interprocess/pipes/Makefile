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
bins := $(filter-out $(BIN_DIR)/color_changer, $(bins))
$(info GTK 3 not found: color_changer is skipped)
endif

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2
LDFLAGS =
COLOR_CHANGER_CFLAGS = `pkg-config --cflags gtk+-3.0`
COLOR_CHANGER_LDFLAGS = `pkg-config --libs gtk+-3.0`

all: directories $(bins)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR)

# Rule for color_changer file
$(BIN_DIR)/color_changer: $(SRC_DIR)/color_changer.c
	$(CC) $(CFLAGS) $(COLOR_CHANGER_CFLAGS) -c $< -o $(OBJ_DIR)/color_changer.o
	$(CC) $(OBJ_DIR)/color_changer.o -o $@ $(LDFLAGS) $(COLOR_CHANGER_LDFLAGS)

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
