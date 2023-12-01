# Interprocess Examples

## Pipes

This sub-section contains examples related to the use of pipes for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name               | Description | Link |
|-------------------------|-------------|------|
| anonymous_pipe_01.c     | Parent and child communicate through an anonymous pipe | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/anonymous_pipe_01.c) |
| anonymous_pipe_02.c     | Parent and child communicate through an anonymous pipe | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/anonymous_pipe_02.c) |
| color_changer.c         | Gtk process that changes its background color based on the input from a named pipe | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/color_changer.c) |
| color_sender.c          | Process that sends background color to `color_changer` through a named pipe | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/color_sender.c) |
| mkfifo_client.c         | Client that sends a request to a server through a named pipe | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_client.c) |
| mkfifo_server.c         | Server that returns the result of a calculation request sent by a client | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_server.c) |
| mkfifo_consumer.c       | Producer that sends a message through a named pipe | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_consumer.c) |
| mkfifo_producer.c       | Producer that reads a message from a named pipe | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_producer.c) |

## POSIX Message Queues

This sub-section contains examples related to the use of POSIX message queues for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name             | Description | Link |
|-----------------------|-------------|------|
| message_sender.c      | Sends messages to `message_viewer` throught a message queue | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/message_sender.c) |
| message_viewer.c      | Receives messages from `message_sender` throught a message queue | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/message_viewer.c) |
| posix_msg_receiver.c  | A Receiver process using POSIX mqueue | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/posix_msg_receiver.c) |
| posix_msg_sender.c    | A sender process using POSIX mqueue] | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/posix_msg_sender.c) |


