------------------------------------------------------------------------------
# Memory
------------------------------------------------------------------------------

------------------------------------------------------------------------------
## File projection on memory
------------------------------------------------------------------------------
The `mmap`function allows to project a file onto a memory segment in memory.
The `munmap` function frees this memory segment.

Files: `mmap_01.c`, `mmap_02.c` in memory_mapping directory

------------------------------------------------------------------------------
## Shared memory
------------------------------------------------------------------------------

1. Sharing memory between parents and child processes
   The shared memory uses the previous memory mapping mechanism, but without
   using files.

Files: `mmap_buffer_01.c`, `mmap_buffer_02.c`, `mmap_buffer_03.c` and
`mmap_buffer_04.c`

2. Sharing memory between distinct processes
   This mechanism allows multiple processes to share memory segments. Each memory
   segment is identified by a *key*. When a segment is attached to a process, data
   are accessible in memory through a pointer.

Files: `shm_producer.c` and `shm_consumer.c` in shared_memory directory

