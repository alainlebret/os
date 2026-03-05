# I/O Multiplexing Examples

This section contains examples demonstrating I/O multiplexing with `select()` and `poll()`. These system calls allow a process to monitor multiple file descriptors simultaneously, blocking until at least one becomes ready for I/O or a timeout expires. This pattern is fundamental for servers handling multiple clients and for any program that must react to events on several sources without blocking.

## select()

`select()` monitors three sets of file descriptors (read, write, exception), represented as `fd_set` bitmasks. It is limited to `FD_SETSIZE` descriptors (typically 1024) and requires rebuilding the descriptor sets before every call.

| File Name      | Description | Link |
|----------------|-------------|------|
| `select_01.c`  | Monitors stdin with a 5-second timeout: echoes input or reports timeout if no data arrives | [View](https://github.com/alainlebret/os/blob/master/exemplier/8_multiplexing/src/select_01.c) |
| `select_02.c`  | Monitors two anonymous pipes simultaneously: reads from whichever child writes first, without blocking on the other | [View](https://github.com/alainlebret/os/blob/master/exemplier/8_multiplexing/src/select_02.c) |

## poll()

`poll()` takes an array of `struct pollfd` descriptors. It has no `FD_SETSIZE` limit, does not require rebuilding the descriptor set each call, and supports removing a descriptor by setting `.fd = -1`.

| File Name    | Description | Link |
|--------------|-------------|------|
| `poll_01.c`  | Same behaviour as `select_01.c` but using `poll()`: compare both APIs side by side | [View](https://github.com/alainlebret/os/blob/master/exemplier/8_multiplexing/src/poll_01.c) |
| `poll_02.c`  | Same behaviour as `select_02.c` but using `poll()`: shows the stable `pollfd` array and `.fd = -1` removal idiom | [View](https://github.com/alainlebret/os/blob/master/exemplier/8_multiplexing/src/poll_02.c) |
