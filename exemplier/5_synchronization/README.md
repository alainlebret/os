# Synchronization Examples

## File locking

This sub-section focuses on file locking mechanisms in C for synchronization purposes. Below is the source file with a link to its content.

| File Name      | Description | Link |
|----------------|-------------|------|
| `lock_file.c`  | Demonstrates file locking by attempting to lock a file named test_lock. | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/file_locking/src/lock_file.c) |


## Semaphores

This sub-section focuses on the use of semaphores for synchronization in C. Below is a list of the source files with links to their content.

| File Name             | Description | Link |
|-----------------------|-------------|------|
| `posix_prod_cons.c`   | Demonstrates a synchronized producer/consumer mechanisme between two child processes (POSIX version) | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/posix_prod_cons.c) |
| `posix_semaphore.c`   | Shows how a POSIX semaphore work | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/posix_semaphore.c) |
| `unix_prod_cons.c`    | Demonstrates a synchronized producer/consumer mechanism (System V version)   | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/unix_prod_cons.c) |
| `unix_semaphore.c`    | Shows how a System V semaphore work | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/unix_semaphore.c) |


## Various Examples

This sub-section contains various examples related to synchronization mechanisms in C. Below is a list of the source files with links to their content.

| File Name                            | Description | Link |
|--------------------------------------|-------------|------|
| `color_displayer.c`                  | Gtk window that reads its background RGB value from a shared memory | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/color_displayer.c) |
| `color_displayer_without_synchro.c`  | Gtk window that reads its background RGB value from a shared memory (unsynchronized version) | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/color_displayer_without_synchro.c) |
| `color_writer.c`                     | A process that modifies RGB value in a shared memory | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/color_writer.c) |
| `color_writer_without_synchro.c`     | A process that modifies RGB value in a shared memory (unsynchronized version) | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/color_writer_without_synchro.c) |
| `without_synchro.c`                  | Shows access to a critical section without synchronization | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/without_synchro.c) |
