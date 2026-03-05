# File and Directory Examples

This section contains examples related to file operations in C. Below is a list of the source files with descriptions and links to their content.

| File Name            | Description                                                               | Link |
|----------------------|---------------------------------------------------------------------------|------|
| `file_copy.c`        | Illustrates how to copy typed keys from the keyboard to a file            | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/file_copy.c) |
| `file_create.c`      | Demonstrates the use of `open()` to create a new file                     | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/file_create.c) |
| `file_createhuge.c`  | Uses `lseek()` to create a huge file                                      | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/file_createhuge.c) |
| `file_hexdump.c`     | Uses `read()` to display the hexadecimal dump of a file                   | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/file_hexdump.c) |
| `fs_block.c`         | Uses `statvfs()` to extract block size information                        | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/fs_block.c) |
| `dup_01.c`           | Demonstrates `dup()`: creates a duplicate file descriptor (lowest available number) sharing the same open file description | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/dup_01.c) |
| `dup_02.c`           | Demonstrates `dup2()`: redirects stdout to a file, replicating the shell `> file` operator | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/dup_02.c) |
| `dup_03.c`           | Demonstrates `pipe()` + `dup2()`: replicates the shell pipe operator `\|` by connecting child stdout to parent stdin | [View](https://github.com/alainlebret/os/blob/master/exemplier/3_files/src/dup_03.c) |
