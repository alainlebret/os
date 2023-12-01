# Process Examples

This section contains examples related to process management in C. These examples demonstrate various aspects of process creation, management, and communication in Unix-like systems. Below is a list of the source files with descriptions and links to their content.

| File Name        | Description                                                               | Link |
|------------------|---------------------------------------------------------------------------|------|
| process_01.c     | Demonstrates the cloning process using fork()                             | [process_01.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_01.c) |
| process_02.c     | Illustrates that processes have their own memory, even if they are parent | [process_02.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_02.c) |
| process_03.c     | Clones a process using fork() and both processes sleep a while            | [process_03.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_03.c) |
| process_04a.c    | Clones a process using fork() and gets a zombie                           | [process_04a.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_04a.c) |
| process_04b.c    | Clones a process using fork() and sleeps to prevent a zombie              | [process_04b.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_04b.c) |
| process_05.c     | Clones a process using fork() and waits properly to prevent a zombie      | [process_05.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_05.c) |
| process_06a.c    | Uses fork() + exec() with a simple Unix command                           | [process_06a.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_06a.c) |
| process_06b.c    | Uses fork() + exec() with Gnuplot                                         | [process_06b.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_06b.c) |
| process_07.c     | Shows details about the PID and the group of a process                    | [process_07.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_07.c) |
| process_08.c     | Uses fork() + exec() with moving_window                                   | [process_08.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_08.c) |
| process_09.c     | Uses fork() + exec() with different applications                          | [process_09.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/process_09.c) |
| test_thread.c    | Shows that a process has a default multithread structure                  | [test_thread.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/test_thread.c) |
| update_w1.sh     | Bash script to send SIGUSR1/SIGUSR2 signals to a process                  | [update_w1.sh](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/update_w1.sh) |
| moving_window.c  | Gtk window that is used by process_08.c and update_w1.sh                  | [moving_window.c](https://github.com/alainlebret/os/blob/master/exemplier/1_process/src/moving_window.c) |

