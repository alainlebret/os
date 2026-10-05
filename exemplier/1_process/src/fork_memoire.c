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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "attendre.h"

/**
 * @file fork_memoire.c
 *
 * @brief Après fork(), le parent et l'enfant ont chacun leur propre copie
 * de la mémoire : modifier x dans l'enfant ne change pas x dans le parent.
 */

int main(void)
{
    int x = 10;
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        x = 20;
        printf("enfant : x = %d\n", x);
        return EXIT_SUCCESS;
    }
    printf("parent : x = %d\n", x);
    int status;
    if (attendre(pid, &status) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}
