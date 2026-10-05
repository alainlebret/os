CC = gcc
# -std=c11 hides the POSIX API under Linux: _DEFAULT_SOURCE restores it
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2 -D_DEFAULT_SOURCE
LDFLAGS =
BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = src
 
# GTK programs are built only when GTK 3 is installed (pkg-config gtk+-3.0)
HAVE_GTK := $(shell pkg-config --exists gtk+-3.0 && echo yes)

# Detect the operating system
UNAME = $(shell uname)
GTK_LDFLAGS = $(LDFLAGS)

# Linux-specific settings
ifeq ($(UNAME), Linux)
    LDFLAGS +=  -lrt
    GTK_LDFLAGS += -lrt
endif

# macOS-specific settings
ifeq ($(HAVE_GTK), yes)
    CFLAGS += $(shell pkg-config --cflags gtk+-3.0)
    GTK_LDFLAGS += $(shell pkg-config --libs gtk+-3.0)
endif

# Target executables
TARGETS := $(BIN_DIR)/without_synchro
ifeq ($(HAVE_GTK), yes)
TARGETS += $(BIN_DIR)/color_writer $(BIN_DIR)/color_displayer $(BIN_DIR)/color_writer_without_synchro \
        $(BIN_DIR)/color_displayer_without_synchro
else
$(info GTK 3 not found: color_* programs are skipped)
endif

# Course examples (chapter « Synchronisation »): threads, no GTK
COURSE := cnt_thread cnt_fork cnt_thread_mutex cnt_thread_atomic \
          alarm_temperature alarm_temperature_broadcast prod_cons_conditions
TARGETS += $(addprefix $(BIN_DIR)/,$(COURSE))

.PHONY: all clean distclean

all: directories $(TARGETS)

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/color_writer: $(OBJ_DIR)/color_writer.o
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LDFLAGS)

$(BIN_DIR)/color_displayer: $(OBJ_DIR)/color_displayer.o
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LDFLAGS)

$(BIN_DIR)/color_writer_without_synchro: $(OBJ_DIR)/color_writer_without_synchro.o
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LDFLAGS)

$(BIN_DIR)/color_displayer_without_synchro: $(OBJ_DIR)/color_displayer_without_synchro.o
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LDFLAGS)

$(BIN_DIR)/without_synchro: $(OBJ_DIR)/without_synchro.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(addprefix $(BIN_DIR)/,$(COURSE)): $(BIN_DIR)/%: $(OBJ_DIR)/%.o
	$(CC) $(CFLAGS) -pthread -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o

distclean: clean
	rm -rf $(BIN_DIR)
