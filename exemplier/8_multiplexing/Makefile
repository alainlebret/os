SRC_DIR = src
OBJ_DIR = src
BIN_DIR = bin

src  = $(wildcard $(SRC_DIR)/*.c)
bins = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(src))

CC     = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2

all: directories $(bins)

directories:
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	$(CC) $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -rf $(BIN_DIR)

.PHONY: all clean distclean
