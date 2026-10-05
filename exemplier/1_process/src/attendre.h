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

/**
 * @file attendre.h
 *
 * @brief Fonction attendre() : waitpid() bloquant sur un enfant précis,
 * relancé si un signal l'interrompt (EINTR).
 */

#ifndef ATTENDRE_H
#define ATTENDRE_H

#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Attente bloquante d'un enfant précis ; recommencer si un signal interrompt. */
static int attendre(pid_t enfant, int *status)
{
    pid_t resultat;
    do {
        resultat = waitpid(enfant, status, 0);
    } while (resultat == -1 && errno == EINTR);
    return resultat == -1 ? -1 : 0;
}

#endif
