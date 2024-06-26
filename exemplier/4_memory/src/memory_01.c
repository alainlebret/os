/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and EXIT_SUCCESS */
#include <unistd.h>    /* getpagesize() */

/**
 * @file memory_01.c
 *
 * Demonstrates how to retrieve and display the system's memory page size.
 * Knowing the page size can be critical for performance tuning and memory
 * management tasks, as it influences how memory is allocated and managed
 * at the system level.
 */

int main(void) {
    int pageSize = getpagesize();  /* Retrieve the system's page size */
    printf("Size of page on this system: %d bytes\n", pageSize);
    return EXIT_SUCCESS;
}
