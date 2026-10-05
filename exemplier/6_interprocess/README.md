# Interprocess Examples

## Pipes

This sub-section contains examples related to the use of pipes for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name             | Description                                                                        | Link                                                                                                         |
|-----------------------|------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------|
| `anonymous_pipe_01.c` | Parent and child communicate through an anonymous pipe                             | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/anonymous_pipe_01.c) |
| `anonymous_pipe_02.c` | Parent and child communicate through an anonymous pipe                             | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/anonymous_pipe_02.c) |
| `color_changer.c`     | Gtk process that creates the named pipe `colorpipe` and changes its background color based on its input (run it first) | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/color_changer.c)     |
| `color_sender.c`      | Process that sends background color to `color_changer` through a named pipe        | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/color_sender.c)      |
| `mkfifo_client.c`     | Client that sends a request to a server through a named pipe                       | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_client.c)     |
| `mkfifo_server.c`     | Server that returns the result of a calculation request sent by a client through two FIFOs; ignores `SIGPIPE` so that it can remove the FIFOs if the client leaves           | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_server.c)     |
| `mkfifo_producer.c`   | Producer that creates the named pipe `testfifo` (accepts `EEXIST`) and sends a message through it | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_producer.c)   |
| `mkfifo_consumer.c`   | Consumer that reads a message from a named pipe until EOF, then removes it with `unlink()` | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/mkfifo_consumer.c)   |
| `pipe_ls_grep.c`      | Reproduces the shell pipeline `ls \| grep txt` with `pipe()`, two `fork()`, `dup2()` and `execlp()` (course, chapter « Tubes ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/pipe_ls_grep.c)      |
| `pipe_deadlock.c`     | Deadlock with two pipes: parent and child both read first; an alarm of `DELAY` seconds ends the demonstration (course, chapter « Tubes ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/pipes/src/pipe_deadlock.c)     |

## POSIX Message Queues

This sub-section contains examples related to the use of POSIX message queues for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name              | Description                                                      | Link                                                                                                                   |
|------------------------|------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------|
| `message_sender.c`     | Sends messages to `message_viewer` throught a message queue      | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/message_sender.c)     |
| `message_viewer.c`     | Receives messages from `message_sender` throught a message queue | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/message_viewer.c)     |
| `posix_msg_receiver.c` | A Receiver process using POSIX mqueue                            | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/posix_msg_receiver.c) |
| `posix_msg_sender.c`   | A sender process using POSIX mqueue: creates the queue, sends timestamps until Ctrl-C, then removes it with `mq_unlink()` | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/posix_msg_sender.c)   |
| `mq_news_receiver.c`   | Creates the queue `/news`, receives one message with its priority, then calls `mq_unlink()` (Linux; macOS through `macosx_mqueue`) (course, chapter « Files de messages ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/mq_news_receiver.c)   |
| `mq_news_sender.c`     | Sends one message with a given priority to `/news` (`./mq_news_sender 10 BBB`) (Linux; macOS through `macosx_mqueue`) (course, chapter « Files de messages ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/posix_messages/src/mq_news_sender.c)     |


## Shared Memory

This sub-section contains examples related to the use of shared memory for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name              | Description                                                                   | Link                                                                                                                  |
|------------------------|-------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|
| `color_displayer.c`    | Gtk window that changes its background color when a shared memory is modified | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/color_displayer.c)    |
| `color_modifier.c`     | Process that writes random colors to a shared memory read by `color_displayer` (no synchronization; matrix size between 1 and 800; needs GTK 3 for the displayer)     | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/color_modifier.c)     |
| `mmap_buffer_01.c`     | Parent and child share one integer in an anonymous mapping, without synchronization (`sleep()` only)          | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/mmap_buffer_01.c)     |
| `mmap_buffer_02.c`     | Producer-consumer with a circular buffer in shared memory and active waiting (no semaphore)      | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/mmap_buffer_02.c)     |
| `mmap_buffer_03.c`     | Same as `mmap_buffer_02.c` but synchronized with two System V semaphores (functions `P()` and `V()`)                  | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/mmap_buffer_03.c)     |
| `mmap_buffer_04.c`     | Several producers and consumers on the same buffer: two counting semaphores plus a mutex semaphore (System V, `P()`/`V()`)                                                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/mmap_buffer_04.c)     |
| `posix_shm_client.c`   | A client sharing memory with a server (client version)                        | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_client.c)   |
| `posix_shm_server.c`   | A client sharing memory with a server (server version)                        | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_server.c)   |
| `posix_shm_simple_1.c` | Example using parent and child processes sharing memory                       | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_simple_1.c) |
| `posix_shm_simple_2.c` | Another example using parent and child processes sharing memory               | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_simple_2.c) |
| `posix_shm_simple_3.c` | Parent and child processes sharing a structure of one page (`/pipeautique_simple3`; `PAGESIZE` fixed to 4096: fails on macOS Apple Silicon, whose pages are 16 KiB)                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_simple_3.c) |
| `unix_shm.c`           | Child processes IPC/sharing System V memory and POSIX semaphore               | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/unix_shm.c)           |
| `unix_shm_consumer.c`  | Consumer using an IPC/System V shared memory                                  | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/unix_shm_consumer.c)  |
| `unix_shm_producer.c`  | Producer using an IPC/System V shared memory                                  | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/unix_shm_producer.c)  |
| `posix_shm_spacebattle.c` | Example 1: creates, sizes, projects and frees a POSIX shared memory segment (course, chapter « Mémoire partagée ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_spacebattle.c) |
| `posix_shm_creator.c` | Creator of the segment `/exemple`: writes a value and a message, waits for Enter, then removes it (course, chapter « Mémoire partagée ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_creator.c) |
| `posix_shm_consumer.c` | Consumer reading the existing segment `/exemple` read-only (checks its size first) (course, chapter « Mémoire partagée ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_consumer.c) |
| `posix_shm_fork_wait.c` | Example 2: the child writes 25 integers into the segment, the parent waits for it, then reads (course, chapter « Mémoire partagée ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_fork_wait.c) |
| `posix_shm_two_pages.c` | Example 3: segment of two pages projected twice (offset from `sysconf(_SC_PAGESIZE)`), exchange between parent and child (course, chapter « Mémoire partagée ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_two_pages.c) |
| `posix_shm_ping_pong.c` | Parent/child ping-pong synchronized by two anonymous semaphores placed in the segment (**Linux only**: `sem_init()` is not implemented on macOS) (course, chapter « Mémoire partagée ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/shared_memory/src/posix_shm_ping_pong.c) |


# Signal Handling

This sub-section contains examples related to signal handling for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name      | Description                                                    | Link                                                                                                   |
|----------------|----------------------------------------------------------------|--------------------------------------------------------------------------------------------------------|
| `signal_01.c`  | Defines the new handler of the SIGINT signal                   | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_01.c)  |
| `signal_02.c`  | Handles SIGALRM                                                | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_02.c)  |
| `signal_03.c`  | Handles SIGALRM to create a clock                              | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_03.c)  |
| `signal_04.c`  | Handles `SIGCHLD` (handler installed before `fork()`): the parent works until its child has ended, without leaving a zombie                                                | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_04.c)  |
| `signal_05.c`  | Handles many signals                                           | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_05.c)  |
| `signal_06.c`  | Manages masks to block signals                                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_06.c)  |
| `signal_07.c`  | Handles SIGUSR1                                                | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_07.c)  |
| `signal_08a.c` | Handles SIGUSR1 without masking other signals                  | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_08a.c) |
| `signal_08b.c` | Handles SIGUSR1 and masking other signals                      | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_08b.c) |
| `signal_08c.c` | Variant of `signal_08a/b`: handles SIGUSR1 and SIGUSR2 with distinct handlers | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_08c.c) |
| `signal_09.c`  | Sets up a signal handler with 3 arguments including `siginfo_t` | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_09.c)  |
| `signal_kill_child.c` | The parent sends SIGUSR1 to its child with `kill()`; the default action kills the child (course, chapter « Signaux ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_kill_child.c)  |
| `signal_sigsuspend.c` | Waits for SIGUSR1 without race condition: `sigprocmask()` + `sigsuspend()` and a `volatile sig_atomic_t` flag (course, chapter « Signaux ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_sigsuspend.c)  |
| `daemon_double_fork.c` | Creates a daemon with the double fork technique and `setsid()`; stops on SIGTERM (course, chapter « Signaux », part « Démons ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/daemon_double_fork.c)  |

# Timer Handling

This sub-section contains examples related to timer handling for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name      | Description                                                    | Link                                                                                                   |
|----------------|----------------------------------------------------------------|--------------------------------------------------------------------------------------------------------|
| `linux_clock.c`  | Handles POSIX timer to create a clock (Linux only: `timer_create()`, `SIGRTMIN`; see [signal_03.c](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_03.c))                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/timers/src/linux_clock.c)  |
| `macosx_clock.c`  | Handles timer to create a clock (see [signal_03.c](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/signal/src/signal_03.c))                                                | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/timers/src/macosx_clock.c)  |
| `posix_timer_tic.c`  | POSIX timer on `CLOCK_MONOTONIC` notifying SIGRTMIN: prints `tic` after 1.02 s, then every 500 ms (Linux) (course, chapter « Signaux », part « Minuteries POSIX ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/timers/src/posix_timer_tic.c)  |

# IPC/System V Messages

This sub-section contains examples related to the use of IPC/System message queues for interprocess communication in C. Below is a list of the source files with links to their content.

| File Name.        | Description                                          | Link                                                                                                             |
|-------------------|------------------------------------------------------|------------------------------------------------------------------------------------------------------------------|
| `message.c`       | Creates and displays messages (used by all programs) | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/unix_messages/src/message.c)       |
| `test_message.c`  | Tests the message creation and display functions     | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/unix_messages/src/test_message.c)  |
| `unix_msg_recv.c` | Receives a message through a System V queue; `-r` removes the queue afterwards (`msgctl(IPC_RMID)`)     | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/unix_messages/src/unix_msg_recv.c) |
| `unix_msg_send.c` | Sends a message (at most 1023 characters) through a System V queue (created if needed)        | [View](https://github.com/alainlebret/os/blob/master/exemplier/6_interprocess/unix_messages/src/unix_msg_send.c) |
