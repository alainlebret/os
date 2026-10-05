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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @file knowing_nbcores.c
 *
 * A simple program used to know how many cores (online processors) your
 * system has, using sysconf(_SC_NPROCESSORS_ONLN). This value is not
 * required by POSIX, but it is provided by Linux and macOS.
 */

int main(void) {
    long cores = sysconf(_SC_NPROCESSORS_ONLN);

    if (cores == -1) {
        perror("sysconf");
        exit(EXIT_FAILURE);
    }
    printf("Number of cores: %ld\n", cores);

    return EXIT_SUCCESS;
}
