# Threads in Concurrent Programming

This section contains examples related to the use of threads for concurrent programming in C. Below is a list of the source files with links to their content.

| File Name                         | Description | Link |
|-----------------------------------|-------------|------|
| `test_pthread.c`                  | Basic `pthread_create()` / `pthread_join()` demonstration: creates one thread and waits for it | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/test_pthread.c) |
| `test_fork.c`                     | Shows the interaction between `fork()` and threads: only the calling thread is duplicated | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/test_fork.c) |
| `knowing_nbcores.c`               | Queries the number of online CPUs using `sysconf(_SC_NPROCESSORS_ONLN)` | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/knowing_nbcores.c) |
| `thread_01.c`                     | Two threads increment a shared global variable without synchronization, intentionally showing a race condition | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/thread_01.c) |
| `thread_02.c`                     | Three threads with a hierarchy: main creates two workers, one of which creates a sub-thread | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/thread_02.c) |
| `thread_02_withmutex.c`           | Same as `thread_02.c` but protects the shared counter with a `pthread_mutex_t` | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/thread_02_withmutex.c) |
| `thread_with_shm_without_mutex.c` | Parent and child threads access shared memory without synchronization, showing a race condition | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/thread_with_shm_without_mutex.c) |
| `thread_with_shm_and_mutex.c`     | Same as above but uses a mutex to safely serialize access to shared memory | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/thread_with_shm_and_mutex.c) |
| `thread_with_conditions.c`        | Producer-consumer pattern using `pthread_cond_wait()` / `pthread_cond_signal()` | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/thread_with_conditions.c) |
| `thread_with_signal.c`            | Demonstrates signal handling in a multithreaded program using `sigwait()` in a dedicated thread | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/thread_with_signal.c) |
| `pb_reentrant.c`                  | Shows the problem with non-reentrant functions (e.g., `strtok`) called from multiple threads | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/pb_reentrant.c) |
| `without_pb_reentrant.c`          | Fix for `pb_reentrant.c` using the reentrant variant `strtok_r()` | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/without_pb_reentrant.c) |
| `pthread_barrier.c`               | Custom implementation of `pthread_barrier_t` for macOS, which does not provide it natively | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/pthread_barrier.c) |
| `barrier.c`                       | Demonstrates `pthread_barrier_wait()`: all threads block until the last one arrives | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/barrier.c) |
| `timer_and_citizens.c`            | Simulation where citizen threads perform actions driven by a periodic SIGALRM timer | [View](https://github.com/alainlebret/os/blob/master/exemplier/9_threads/src/timer_and_citizens.c) |
