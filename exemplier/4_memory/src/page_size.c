/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2026 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>     /* printf(), perror() */
#include <stdlib.h>    /* EXIT_SUCCESS, EXIT_FAILURE */
#include <unistd.h>    /* sysconf() */

/**
 * @file page_size.c
 * @brief Displays the page size using the POSIX function sysconf().
 *
 * Lecture: chapter os-09a « Mémoire virtuelle » ("Connaître la taille des
 * pages").
 *
 * POSIX variant of memory_01.c, which uses getpagesize() (removed from
 * POSIX). A portable program never assumes 4096: it asks the system.
 *
 * \code{.bash}
 *   $ ./page_size             # Linux, x86-64
 *   taille de page : 4096 octets
 *   $ ./page_size             # macOS, Apple Silicon
 *   taille de page : 16384 octets
 * \endcode
 */

int main(void) {
    long taille = sysconf(_SC_PAGESIZE);    /* POSIX */
    if (taille == -1) {
        perror("sysconf");
        return EXIT_FAILURE;
    }
    printf("taille de page : %ld octets\n", taille);
    return EXIT_SUCCESS;
}
