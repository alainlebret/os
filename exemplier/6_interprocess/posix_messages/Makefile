CC=gcc
INCLUDE_DIR=include
SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=.tmp
MAC_SRC_DIR=$(SRC_DIR)/macosx_mqueue
CPPFLAGS=-I $(INCLUDE_DIR) -D_REENTRANT
CFLAGS=-Wall -Wextra -pedantic -std=c11 -g

# libraries to link (-lXXX ex: -lm for maths)
# Detect OS
UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  LDFLAGS=-lpthread
else
  LDFLAGS=-lrt -lpthread
endif

all: directories message_viewer message_sender posix_msg_receiver posix_msg_sender

# Rule to create directories
directories:
	@mkdir -p $(BIN_DIR) $(INC_DIR) $(OBJ_DIR)

# ----------------- MESSAGE_VIEWER UNDER GTK 3.0 ----------------------
ifeq ($(UNAME_S),Darwin)
message_viewer: .tmp/message_viewer.o .tmp/mq_close.o .tmp/mq_getattr.o .tmp/mq_internal_fs.o .tmp/mq_notify.o .tmp/mq_open.o .tmp/mq_receive.o .tmp/mq_send.o .tmp/mq_setattr.o .tmp/mq_timedreceive.o .tmp/mq_timedsend.o .tmp/mq_unlink.o .tmp/logger.o
	$(CC) $^ -o $(BIN_DIR)/$@ $(LDFLAGS) $(shell pkg-config --libs gtk+-3.0)
else
message_viewer: .tmp/message_viewer.o
	$(CC) $^ -o $(BIN_DIR)/$@ $(LDFLAGS) $(shell pkg-config --libs gtk+-3.0)
endif

.tmp/message_viewer.o: $(SRC_DIR)/message_viewer.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(shell pkg-config --cflags gtk+-3.0) -c $< -o $(@)

# -------------------------- MESSAGE_SENDER ---------------------------
ifeq ($(UNAME_S),Darwin)
message_sender: .tmp/message_sender.o .tmp/mq_close.o .tmp/mq_getattr.o .tmp/mq_internal_fs.o .tmp/mq_notify.o .tmp/mq_open.o .tmp/mq_receive.o .tmp/mq_send.o .tmp/mq_setattr.o .tmp/mq_timedreceive.o .tmp/mq_timedsend.o .tmp/mq_unlink.o .tmp/logger.o
	$(CC) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
else
message_sender: .tmp/message_sender.o
	$(CC) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
endif

.tmp/message_sender.o: $(SRC_DIR)/message_sender.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

# ---------------------- POSIX_MESSAGE_RECEIVER -----------------------
ifeq ($(UNAME_S),Darwin)
posix_msg_receiver: .tmp/posix_msg_receiver.o .tmp/mq_close.o .tmp/mq_getattr.o .tmp/mq_internal_fs.o .tmp/mq_notify.o .tmp/mq_open.o .tmp/mq_receive.o .tmp/mq_send.o .tmp/mq_setattr.o .tmp/mq_timedreceive.o .tmp/mq_timedsend.o .tmp/mq_unlink.o .tmp/logger.o
	$(CC) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
else
posix_msg_receiver: .tmp/posix_msg_receiver.o
	$(CC) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
endif

.tmp/posix_msg_receiver.o: $(SRC_DIR)/posix_msg_receiver.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

# ---------------------- POSIX_MESSAGE_SENDER -------------------------
ifeq ($(UNAME_S),Darwin)
posix_msg_sender: .tmp/posix_msg_sender.o .tmp/mq_close.o .tmp/mq_getattr.o .tmp/mq_internal_fs.o .tmp/mq_notify.o .tmp/mq_open.o .tmp/mq_receive.o .tmp/mq_send.o .tmp/mq_setattr.o .tmp/mq_timedreceive.o .tmp/mq_timedsend.o .tmp/mq_unlink.o .tmp/logger.o
	$(CC) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
else
posix_msg_sender: .tmp/posix_msg_sender.o
	$(CC) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
endif

.tmp/posix_msg_sender.o: $(SRC_DIR)/posix_msg_sender.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

# ----------------------- MQUEUE FOR MACOSX ---------------------------
.tmp/mq_close.o: $(MAC_SRC_DIR)/mq_close.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_getattr.o: $(MAC_SRC_DIR)/mq_getattr.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_internal_fs.o: $(MAC_SRC_DIR)/mq_internal_fs.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_notify.o: $(MAC_SRC_DIR)/mq_notify.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_open.o: $(MAC_SRC_DIR)/mq_open.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_receive.o: $(MAC_SRC_DIR)/mq_receive.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_send.o: $(MAC_SRC_DIR)/mq_send.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_setattr.o: $(MAC_SRC_DIR)/mq_setattr.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_timedreceive.o: $(MAC_SRC_DIR)/mq_timedreceive.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_timedsend.o: $(MAC_SRC_DIR)/mq_timedsend.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@  -c

.tmp/mq_unlink.o: $(MAC_SRC_DIR)/mq_unlink.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/logger.o: $(MAC_SRC_DIR)/logger.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

# -----------------------------------------------------------------------------
#                                 Cleaning
# -----------------------------------------------------------------------------
#
clean:
	rm -rf .tmp/*.o .tmp/.DS_Store ; rmdir .tmp

distclean: clean
	rm -f $(BIN_DIR)/message_viewer $(BIN_DIR)/message_sender $(BIN_DIR)/posix_msg_receiver $(BIN_DIR)/posix_msg_sender