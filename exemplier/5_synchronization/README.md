# Synchronization Examples

## File locking

This subsection focuses on file locking mechanisms in C for synchronization purposes. Below is the source file with a link to its content.

| File Name     | Description                                                             | Link                                                                                                           |
|---------------|-------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------|
| `lock_file.c` | Locks the file `/tmp/test_lock` with `lockf()`: run it twice (`./lock_file & ./lock_file`): the second process tries again every second (« déjà verrouillé ») until the first one unlocks (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/file_locking/src/lock_file.c) |


## Semaphores

This subsection focuses on the use of semaphores for synchronization in C. Below is a list of the source files with links to their content.

| File Name           | Description                                                                                          | Link                                                                                                               |
|---------------------|------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------|
| `posix_prod_cons.c` | Producer/consumer between two child processes: circular buffer in a POSIX shared memory, three named semaphores `empty`, `full`, `mutex` (Ctrl-C to stop) | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/posix_prod_cons.c) |
| `posix_semaphore.c` | Interactive P/V on a named POSIX semaphore (run it in two terminals; `x` destroys it)                                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/posix_semaphore.c) |
| `unix_prod_cons.c`  | Producer/consumer with System V semaphores (Ctrl-C stops the children; the parent, which ignores it from the start, removes the IPC objects)              | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/unix_prod_cons.c)  |
| `unix_semaphore.c`  | Interactive P/V on a System V semaphore; usage: `unix_semaphore key` (run it in two terminals)       | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/unix_semaphore.c)  |
| `cnt_thread_semaphore.c` | Running example, correction 1: shared counter protected by an anonymous semaphore (**Linux only**: `sem_init()` is not implemented on macOS) (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/cnt_thread_semaphore.c) |
| `cnt_fork_named_semaphore.c` | Running example, correction 1, fork version: counter in shared memory protected by a named semaphore (very slow on macOS with the default `NB_ITERS`) (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/cnt_fork_named_semaphore.c) |
| `prod_cons_3_semaphores.c` | Producer/consumer between two threads with a circular buffer and three semaphores `vides`, `pleins`, `mutex` (**Linux only**: `sem_init()` is not implemented on macOS) (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/semaphores/src/prod_cons_3_semaphores.c) |


## Various Examples

This subsection contains various examples related to synchronization mechanisms in C. Below is a list of the source files with links to their content.

| File Name                           | Description                                                                                  | Link                                                                                                                            |
|-------------------------------------|----------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------|
| `color_displayer.c`                 | Gtk window that reads its background RGB value from a shared memory                          | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/color_displayer.c)                 |
| `color_displayer_without_synchro.c` | Gtk window that reads its background RGB value from a shared memory (unsynchronized version) | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/color_displayer_without_synchro.c) |
| `color_writer.c`                    | A process that modifies an RGB value in a shared memory, protected by a named semaphore; Ctrl-C ends the loop and removes the shared memory and the semaphore (needs GTK 3 for the displayer)         | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/color_writer.c)                    |
| `color_writer_without_synchro.c`    | Same without semaphore (unsynchronized version); Ctrl-C removes the shared memory                | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/color_writer_without_synchro.c)    |
| `without_synchro.c`                 | Parent and child decrement a shared value without synchronization (deliberate race)         | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/without_synchro.c)                 |
| `cnt_thread.c` | Running example: two threads increment a shared counter without protection (lost updates) (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/cnt_thread.c) |
| `cnt_fork.c` | Running example: same failed synchronization with two child processes and a POSIX shared memory (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/cnt_fork.c) |
| `cnt_thread_mutex.c` | Running example, correction 2: shared counter protected by a mutex (also in chapter « Threads ») (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/cnt_thread_mutex.c) |
| `cnt_thread_atomic.c` | Running example, correction 3: shared counter as a C11 `atomic_uint` (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/cnt_thread_atomic.c) |
| `alarm_temperature.c` | Condition variable: an alarm thread waits until the measured temperature leaves the range 16 to 24 °C (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/alarm_temperature.c) |
| `alarm_temperature_broadcast.c` | Variant with two alarm threads woken up by `pthread_cond_broadcast()` (one predicate per thread) (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/alarm_temperature_broadcast.c) |
| `prod_cons_conditions.c` | Producer/consumer between two threads with a circular buffer, a mutex and two condition variables (course, chapter « Synchronisation ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/5_synchronization/various/src/prod_cons_conditions.c) |
