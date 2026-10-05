# Various Examples in System Programming

This section contains a variety of examples related to system programming in C. Below is the source file with a link to its content on GitHub.

| File Name              | Description | Link |
|------------------------|-------------|------|
| `microseconds_sleep.c` | Uses `select()` with all fd_sets set to NULL and a non-zero timeout to sleep for a delay given in microseconds (the real delay depends on the scheduler); `sleep()` only counts whole seconds, `usleep()` is obsolete and `nanosleep()` is the standard alternative | [View](https://github.com/alainlebret/os/blob/master/exemplier/10_various/src/microseconds_sleep.c) |
