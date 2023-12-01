# Section

This section contains examples related to process management in C. Below is a list of the source files with links to their content.

| File Name          | Description | Link |
|--------------------|-------------|------|
| `process_01.c`     | Show the cloning process using `fork()` | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_01.c) |
| `process_02.c`     | Show that processes have their own memory, even if they are parent | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_02.c) |
| `process_03.c`     | Clone a process using `fork()` and both processes sleep a while | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_03.c) |
| `process_04a.c`    | Clone a process using `fork()` and getting a zombie          | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_04a.c) |
| `process_04b.c`    | Clone a process using `fork()` and sleeping to abort zombie  | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_04b.c) |
| `process_05.c`     | Clone a process using `fork()` and waiting properly to abort zombie | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_05.c) |
| `process_06a.c`    | Use `fork()` + `exec()` with a simple Unix command           | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_06a.c) |
| `process_06b.c`    | Use `fork()` + `exec()` with Gnuplot                         | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_06b.c) |
| `process_07.c`     | Show about the PID and the group of a process                | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_07.c) |
| `process_08.c`     | Use `fork()` + `exec()` with `moving_window`                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_08.c) |
| `process_09.c`     | Use `fork()` + `exec()` with different applications          | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_09.c) |
| `test_thread.c`    | Show that a process has a default multithread structure      | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/test_thread.c) |
| `update_w1.sh`     | Bash script to send `SIGUSR1`/`SIGUSR2` signals to a process | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/update_w1.sh) |
| `moving_window.c`  | Gtk window that is used by `process_08.c` and `update_w1.sh` | [View](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/moving_window.c) |
