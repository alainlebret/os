CC=gcc
INCLUDE_DIR=include
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

all: initdir message_viewer_2 message_viewer_1 message_sender posix_message_receiver posix_message_sender

initdir:
	mkdir .tmp

# ----------------- MESSAGE_VIEWER UNDER GTK 3.0 ----------------------
ifeq ($(UNAME_S),Darwin)
message_viewer: .tmp/message_viewer.o .tmp/mq_close.o .tmp/mq_getattr.o .tmp/mq_internal_fs.o .tmp/mq_notify.o .tmp/mq_open.o .tmp/mq_receive.o .tmp/mq_send.o .tmp/mq_setattr.o .tmp/mq_timedreceive.o .tmp/mq_timedsend.o .tmp/mq_unlink.o .tmp/logger.o
	$(CC) $^ -o $@ $(LDFLAGS) $(shell pkg-config --libs gtk+-3.0)
	chmod u+x $@
else
message_viewer: .tmp/message_viewer.o
	$(CC) $^ -o $@ $(LDFLAGS) $(shell pkg-config --libs gtk+-3.0)
	chmod u+x $@
endif

.tmp/message_viewer_2.o: message_viewer_2.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(shell pkg-config --cflags gtk+-3.0) -c $< -o $(@)

# -------------------------- MESSAGE_SENDER ---------------------------
ifeq ($(UNAME_S),Darwin)
message_sender: .tmp/message_sender.o .tmp/mq_close.o .tmp/mq_getattr.o .tmp/mq_internal_fs.o .tmp/mq_notify.o .tmp/mq_open.o .tmp/mq_receive.o .tmp/mq_send.o .tmp/mq_setattr.o .tmp/mq_timedreceive.o .tmp/mq_timedsend.o .tmp/mq_unlink.o .tmp/logger.o
	$(CC) $^ -o $@ $(LDFLAGS)
else
message_sender: .tmp/message_sender.o
	$(CC) $^ -o $@ $(LDFLAGS)
endif

.tmp/message_sender.o: message_sender.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

# ---------------------- POSIX_MESSAGE_RECEIVER -----------------------
ifeq ($(UNAME_S),Darwin)
posix_message_receiver: .tmp/posix_message_receiver.o .tmp/mq_close.o .tmp/mq_getattr.o .tmp/mq_internal_fs.o .tmp/mq_notify.o .tmp/mq_open.o .tmp/mq_receive.o .tmp/mq_send.o .tmp/mq_setattr.o .tmp/mq_timedreceive.o .tmp/mq_timedsend.o .tmp/mq_unlink.o .tmp/logger.o
	$(CC) $^ -o $@ $(LDFLAGS)
else
posix_message_receiver: .tmp/posix_message_receiver.o
	$(CC) $^ -o $@ $(LDFLAGS)
endif

# ---------------------- POSIX_MESSAGE_SENDER -------------------------
ifeq ($(UNAME_S),Darwin)
posix_message_sender: .tmp/posix_message_sender.o .tmp/mq_close.o .tmp/mq_getattr.o .tmp/mq_internal_fs.o .tmp/mq_notify.o .tmp/mq_open.o .tmp/mq_receive.o .tmp/mq_send.o .tmp/mq_setattr.o .tmp/mq_timedreceive.o .tmp/mq_timedsend.o .tmp/mq_unlink.o .tmp/logger.o
	$(CC) $^ -o $@ $(LDFLAGS)
else
posix_message_sender: .tmp/posix_message_sender.o
	$(CC) $^ -o $@ $(LDFLAGS)
endif

# ----------------------- MQUEUE FOR MACOSX ---------------------------
.tmp/mq_close.o: macosx_mqueue/mq_close.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_getattr.o: macosx_mqueue/mq_getattr.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_internal_fs.o: macosx_mqueue/mq_internal_fs.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_notify.o: macosx_mqueue/mq_notify.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_open.o: macosx_mqueue/mq_open.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_receive.o: macosx_mqueue/mq_receive.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_send.o: macosx_mqueue/mq_send.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_setattr.o: macosx_mqueue/mq_setattr.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_timedreceive.o: macosx_mqueue/mq_timedreceive.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/mq_timedsend.o: macosx_mqueue/mq_timedsend.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@  -c

.tmp/mq_unlink.o: macosx_mqueue/mq_unlink.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

.tmp/logger.o: macosx_mqueue/logger.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -c

# -----------------------------------------------------------------------------
#                                 Cleaning
# -----------------------------------------------------------------------------
#
clean:
	rm -rf .tmp/*.o

