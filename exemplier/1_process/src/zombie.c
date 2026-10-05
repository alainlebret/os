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
 * @file zombie.c
 *
 * @brief Montre un enfant zombie : l'enfant se termine, le parent ne le
 * récupère qu'après un appui sur Entrée (à observer avec ps).
 */

int main(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        _exit(7);
    }
    printf("parent=%ld enfant=%ld\n", (long)getpid(), (long)pid);
    puts("Dans un autre terminal : ps -o pid,ppid,stat,comm -p PID_ENFANT");
    puts("Puis appuyer sur Entrée ici pour récupérer l'état de terminaison.");
    fflush(stdout);
    (void)getchar();
    int status;
    if (attendre(pid, &status) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    if (WIFEXITED(status)) {
        printf("état de terminaison récupéré : code %d\n", WEXITSTATUS(status));
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
