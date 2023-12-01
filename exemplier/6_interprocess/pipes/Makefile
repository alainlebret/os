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
COLOR_CHANGER_CFLAGS = `pkg-config --cflags gtk+-3.0`
COLOR_CHANGER_LDFLAGS = `pkg-config --libs gtk+-3.0`

all: directories $(bins)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR) $(INC_DIR)

# Rule for color_changer file
$(BIN_DIR)/color_changer: $(SRC_DIR)/color_changer.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(COLOR_CHANGER_CFLAGS) -c $< -o $(OBJ_DIR)/color_changer.o
	$(CC) $(OBJ_DIR)/color_changer.o -o $@ $(LDFLAGS) $(COLOR_CHANGER_LDFLAGS)

# General rule to create object files (excluding color_changer.c)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
ifeq ($<, $(SRC_DIR)/color_changer.c)
	@echo "Skipping color_changer.c for general rule"
else
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
endif

# Pattern rule for other executables (excluding color_changer)
$(BIN_DIR)/%: $(OBJ_DIR)/%.o
ifeq ($<, $(OBJ_DIR)/color_changer.o)
	@echo "Skipping color_changer.o for general pattern rule"
else
	$(CC) $< -o $@ $(LDFLAGS)
endif

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -f $(bins)

.PHONY: all clean distclean
