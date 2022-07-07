
all: unix_msg_recv unix_msg_send test_message

unix_msg_recv: unix_msg_recv.o message.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
unix_msg_send: unix_msg_send.o message.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_message: test_message.o message.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -c

veryclean:
	rm *.o test_message unix_msg_recv unix_msg_send

.PHONY: all veryclean
 
