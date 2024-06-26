# Interprocess Examples

## Pipes

This sub-section contains examples related to the use of pipes for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name             | Description                                                                        | Link                                                                                                         |
|-----------------------|------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------|
| `anonymous_pipe_01.c` | Parent and child communicate through an anonymous pipe                             | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/anonymous_pipe_01.c) |
| `anonymous_pipe_02.c` | Parent and child communicate through an anonymous pipe                             | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/anonymous_pipe_02.c) |
| `color_changer.c`     | Gtk process that changes its background color based on the input from a named pipe | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/color_changer.c)     |
| `color_sender.c`      | Process that sends background color to `color_changer` through a named pipe        | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/color_sender.c)      |
| `mkfifo_client.c`     | Client that sends a request to a server through a named pipe                       | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_client.c)     |
| `mkfifo_server.c`     | Server that returns the result of a calculation request sent by a client           | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_server.c)     |
| `mkfifo_consumer.c`   | Producer that sends a message through a named pipe                                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_consumer.c)   |
| `mkfifo_producer.c`   | Producer that reads a message from a named pipe                                    | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_producer.c)   |

## POSIX Message Queues

This sub-section contains examples related to the use of POSIX message queues for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name              | Description                                                      | Link                                                                                                                   |
|------------------------|------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------|
| `message_sender.c`     | Sends messages to `message_viewer` throught a message queue      | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/message_sender.c)     |
| `message_viewer.c`     | Receives messages from `message_sender` throught a message queue | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/message_viewer.c)     |
| `posix_msg_receiver.c` | A Receiver process using POSIX mqueue                            | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/posix_msg_receiver.c) |
| `posix_msg_sender.c`   | A sender process using POSIX mqueue]                             | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/posix_msg_sender.c)   |


## Shared Memory

This sub-section contains examples related to the use of shared memory for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name              | Description                                                                   | Link                                                                                                                  |
|------------------------|-------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|
| `color_displayer.c`    | Gtk window that changes its background color when a shared memory is modified | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/color_displayer.c)    |
| `color_modifier.c`     | Process that writes to a shared memory to interact with `color_displayer`     | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/color_modifier.c)     |
| `mmap_buffer_01.c`     | Producer-consumer using a shared memory that stores a single integer          | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/mmap_buffer_01.c)     |
| `mmap_buffer_02.c`     | Producer-consumer using a shared memory that stores a buffer of integers      | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/mmap_buffer_02.c)     |
| `mmap_buffer_03.c`     | Same as `mmap_buffer_02.c` but synchronized using semaphores                  | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/mmap_buffer_03.c)     |
| `mmap_buffer_04.c`     | Variant of `mmap_buffer_03.c`                                                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/mmap_buffer_04.c)     |
| `posix_shm_client.c`   | A client sharing memory with a server (client version)                        | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_client.c)   |
| `posix_shm_server.c`   | A client sharing memory with a server (server version)                        | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_server.c)   |
| `posix_shm_simple_1.c` | Example using parent and child processes sharing memory                       | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_simple_1.c) |
| `posix_shm_simple_2.c` | Another example using parent and child processes sharing memory               | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_simple_2.c) |
| `posix_shm_simple_3.c` | Parent and child processes sharing a complex structure memory                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_simple_3.c) |
| `unix_shm.c`           | Child processes IPC/sharing System V memory and POSIX semaphore               | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/unix_shm.c)           |
| `unix_shm_consumer.c`  | Consumer using an IPC/System V shared memory                                  | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/unix_shm_consumer.c)  |
| `unix_shm_producer.c`  | Producer using an IPC/System V shared memory                                  | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/unix_shm_producer.c)  |


# Signal Handling

This sub-section contains examples related to signal handling for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name      | Description                                                    | Link                                                                                                   |
|----------------|----------------------------------------------------------------|--------------------------------------------------------------------------------------------------------|
| `signal_01.c`  | Defines the new handler of the SIGINT signal                   | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_01.c)  |
| `signal_02.c`  | Handles SIGALRM                                                | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_02.c)  |
| `signal_03.c`  | Handles SIGALRM to create a clock                              | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_03.c)  |
| `signal_04.c`  | Handles SIGCHLD                                                | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_04.c)  |
| `signal_05.c`  | Handles many signals                                           | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_05.c)  |
| `signal_06.c`  | Manages masks to block signals                                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_06.c)  |
| `signal_07.c`  | Handles SIGUSR1                                                | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_07.c)  |
| `signal_08a.c` | Handles SIGUSR1 without masking other signals                  | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_08a.c) |
| `signal_08b.c` | Handles SIGUSR1 and masking other signals                      | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_08b.c) |
| `signal_09.c`  | Setups a signal handler with 3 arguments including `siginfo_t` | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_09.c)  |


# IPC/System V Messages

This sub-section contains examples related to the use of IPC/System message queues for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name.        | Description                                          | Link                                                                                                             |
|-------------------|------------------------------------------------------|------------------------------------------------------------------------------------------------------------------|
| `message.c`       | Creates and displays messages (used by all programs) | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/unix_messages/src/message.c)       |
| `test_message.c`  | Tests the functions in n                             | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/unix_messages/src/test_message.c)  |
| `unix_msg_recv.c` | Receives a message through an IPC/System V queue     | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/unix_messages/src/unix_msg_recv.c) |
| `unix_msg_send.c` | Sends a message through an IPC/System V queue        | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/unix_messages/src/unix_msg_send.c) |
