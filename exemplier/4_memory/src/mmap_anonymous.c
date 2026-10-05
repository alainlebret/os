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
#define _DEFAULT_SOURCE 1  /* MAP_ANONYMOUS with glibc */
#define _DARWIN_C_SOURCE 1 /* MAP_ANONYMOUS with macOS */
#include <stdio.h>     /* printf(), perror() */
#include <stdlib.h>    /* exit() */
#include <sys/mman.h>  /* mmap(), munmap() */

/**
 * @file mmap_anonymous.c
 * @brief Allocates 1 GiB with an anonymous projection (MAP_ANONYMOUS).
 *
 * Lecture: chapter os-09a « Mémoire virtuelle » ("Projection anonyme").
 *
 * The area is linked to no file and is filled with zeros. Only the pages
 * actually touched receive a physical frame: here, only the first one.
 *
 * Key points:
 *   - works like a dynamic allocation, without fragmenting the heap:
 *     munmap() returns the whole area to the system;
 *   - MAP_ANONYMOUS is standard since POSIX 2024; before: MAP_ANON (BSD)
 *     or a projection of /dev/zero;
 *   - with MAP_SHARED | MAP_ANONYMOUS, the area is shared with the
 *     children created by fork().
 *
 * \code{.bash}
 *   $ ./mmap_anonymous
 *   t[0] = 42, t[1] = 0
 * \endcode
 */

int main(void) {
    size_t taille = 1UL << 30;              /* 1 GiB */
    int *t = mmap(NULL, taille, PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (t == MAP_FAILED) { perror("mmap"); exit(EXIT_FAILURE); }

    t[0] = 42;     /* only this page receives a frame */
    printf("t[0] = %d, t[1] = %d\n", t[0], t[1]);

    munmap(t, taille);  /* returned to the system, without a hole */

    return EXIT_SUCCESS;
}
