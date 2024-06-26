CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS =
BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = src
 
# Detect the operating system
UNAME = $(shell uname)
GTK_LDFLAGS = $(LDFLAGS)

# Linux-specific settings
ifeq ($(UNAME), Linux)
    LDFLAGS +=  -lrt
    CFLAGS += $(shell pkg-config --cflags gtk+-3.0)
    GTK_LDFLAGS += $(shell pkg-config --libs gtk+-3.0) -lrt
endif

# macOS-specific settings
ifeq ($(UNAME), Darwin)
    CFLAGS += $(shell pkg-config --cflags gtk+-3.0)
    GTK_LDFLAGS += $(shell pkg-config --libs gtk+-3.0)
endif

# Source files and target executables
SRCS := $(SRC_DIR)/color_writer.c $(SRC_DIR)/color_displayer.c $(SRC_DIR)/color_writer_without_synchro.c \
        $(SRC_DIR)/color_displayer_without_syncro.c $(SRC_DIR)/without_synchro.c
TARGETS := $(BIN_DIR)/color_writer $(BIN_DIR)/color_displayer $(BIN_DIR)/color_writer_without_synchro\
        $(BIN_DIR)/color_displayer_without_synchro $(BIN_DIR)/without_synchro

.PHONY: all clean

all: directories $(TARGETS)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR) $(INC_DIR)

$(bins)$(BIN_DIR)/color_writer: $(OBJ_DIR)/color_writer.o
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LDFLAGS)

$(BIN_DIR)/color_displayer: $(OBJ_DIR)/color_displayer.o
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LDFLAGS)

$(BIN_DIR)/color_writer_without_synchro: $(OBJ_DIR)/color_writer_without_synchro.o
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LDFLAGS)

$(BIN_DIR)/color_displayer_without_synchro: $(OBJ_DIR)/color_displayer_without_synchro.o
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LDFLAGS)

$(BIN_DIR)/without_synchro: $(OBJ_DIR)/without_synchro.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -f $(TARGETS)
