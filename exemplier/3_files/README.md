# File and Directory Examples

This section contains examples related to file operations in C. Below is a list of the source files with descriptions and links to their content.

| File Name            | Description                                                               | Link |
|----------------------|---------------------------------------------------------------------------|------|
| `file_copy.c`        | Copies the text typed on the keyboard to the file `file.out` with `read()`/`write()` (Ctrl-D to end)            | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/file_copy.c) |
| `file_create.c`      | Demonstrates the use of `open()` to create a new file                     | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/file_create.c) |
| `file_createhuge.c`  | Uses `lseek()` to create a huge file                                      | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/file_createhuge.c) |
| `file_hexdump.c`     | Uses `read()` to display the hexadecimal dump of a file, 20 bytes per line with their ASCII characters                   | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/file_hexdump.c) |
| `fs_block.c`         | Uses `statvfs()` to extract block size information                        | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/fs_block.c) |
| `dup_01.c`           | Demonstrates `dup()`: creates a duplicate file descriptor (lowest available number) sharing the same open file description | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/dup_01.c) |
| `dup_02.c`           | Demonstrates `dup2()`: redirects stdout to the file `sortie.txt` (as in the slide), replicating the shell `> file` operator | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/dup_02.c) |
| `dup_03.c`           | Demonstrates `pipe()` + `dup2()`: replicates the shell pipe operator `\|` by connecting the child's stdout to the pipe read by the parent until EOF | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/dup_03.c) |
| `fork_shared_offset.c` | After `fork()`, parent and child share the file offset: the child reads `abc`, the parent then reads `def` (course, chapter « Fichiers et descripteurs », exercise 1) | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/fork_shared_offset.c) |
