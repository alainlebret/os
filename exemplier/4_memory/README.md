# Memory Examples

This section contains examples related to memory management in C. Below is a list of the source files with descriptions and links to their content.

| File Name         | Description                                                               | Link |
|-------------------|---------------------------------------------------------------------------|------|
| `memory_01.c`     | Demonstrates getting the page size from the virtual memory                | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/memory_01.c) |
| `memory_02.c`     | Shows the memory map of a simple process                                  | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/memory_02.c) |
| `memory_03.c`     | Shows the memory map of a process using a dynamic library                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/memory_03.c) |
| `memory_04.c`     | Heap allocations to observe in the memory map (`/proc/<PID>/maps` on Linux, `vmmap <PID>` on macOS); stopping with Ctrl-C before `free()` is not a real leak (see `memory_05a.c` for one)                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/memory_04.c) |
| `memory_05a.c`    | Demonstrates a memory leak issue in a different context                   | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/memory_05a.c) |
| `memory_05b.c`    | Further illustrates how avoid memory leak problem                         | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/memory_05b.c) |
| `memory_06.c`     | Displays the full memory layout of a process (text, data, BSS, heap, stack) using external symbols `etext`, `edata`, `end` (on macOS: `get_etext()`, `get_edata()`, `get_end()`, which the compiler marks as deprecated: the warnings are expected) | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/memory_06.c) |
| `memory_07.c`     | Provides insights into stack memory usage                                 | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/memory_07.c) |
| `mmap1.c`         | Projects a file into memory with `mmap()` (size from `fstat()` on the open descriptor), writes it to the terminal, then reads the projection like an array                         | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/mmap1.c) |
| `mmap2.c`         | Demonstrates file projection onto a memory segment with a different approach | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/mmap2.c) |
| `page_size.c` | Displays the page size with the POSIX function `sysconf(_SC_PAGESIZE)` (POSIX variant of `memory_01.c`) (course, chapter « Mémoire virtuelle ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/page_size.c) |
| `mmap_toupper.c` | Modifies a file in place through a `MAP_SHARED` projection (converts it to upper case) (course, chapter « Mémoire virtuelle ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/mmap_toupper.c) |
| `mmap_anonymous.c` | Allocates 1 GiB with an anonymous projection (`MAP_ANONYMOUS`); only the touched page receives a frame (course, chapter « Mémoire virtuelle ») | [View](https://github.com/alainlebret/os/blob/master/exemplier/4_memory/src/mmap_anonymous.c) |
