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
 * @file exec_variantes.c
 *
 * @brief Les fonctions de la famille exec, côte à côte : chaque variante est
 * lancée dans un enfant, puis le parent affiche le code de sortie.
 *
 * - l / v : arguments en liste ou en tableau ;
 * - p     : recherche dans le PATH (sinon, un chemin) ;
 * - e     : environnement fourni.
 * Les trois dernières variantes sont des erreurs classiques, volontaires.
 *
 * Cours, chapitre « Processus », diapositives « La même commande, quatre
 * écritures » et « Les erreurs classiques avec exec » : les diapositives
 * utilisent ls -l /tmp ; ici, ls -ld /tmp n'affiche qu'une ligne.
 */

extern char **environ;

/** Le code de l'enfant : une variante d'exec par numéro. */
static void executer_variante(int numero)
{
    char *args[] = {"ls", "-ld", "/tmp", NULL};
    char *env_vide[] = {NULL};

    switch (numero) {
    case 1: /* l : liste + chemin */
        execl("/bin/ls", "ls", "-ld", "/tmp", (char *)NULL);
        break;
    case 2: /* lp : liste + PATH */
        execlp("ls", "ls", "-ld", "/tmp", (char *)NULL);
        break;
    case 3: /* v : tableau + chemin */
        execv("/bin/ls", args);
        break;
    case 4: /* vp : tableau + PATH */
        execvp(args[0], args);
        break;
    case 5: /* le : liste + chemin + environnement (vide ici) */
        execle("/usr/bin/env", "env", (char *)NULL, env_vide);
        break;
    case 6: /* ve : tableau + chemin + environnement (celui du parent) */
        execve("/bin/ls", args, environ);
        break;
    case 7: /* ERREUR : argv[0] oublié, "bonjour" devient argv[0] */
        execlp("echo", "bonjour", "le monde", (char *)NULL);
        break;
    case 8: /* ERREUR : "ls -ld" n'est pas un nom de programme */
        execlp("ls -ld", "ls -ld", (char *)NULL);
        break;
    default: /* ERREUR : sans p, "ls" est un chemin relatif au dossier courant */
        execl("ls", "ls", (char *)NULL);
        break;
    }
    perror("exec"); /* atteint seulement si exec a échoué */
    _exit(127);
}

/** Crée un enfant pour une variante, l'attend et affiche son code de sortie. */
static void lancer_variante(int numero, const char *titre)
{
    int status;
    pid_t pid;

    printf("\n--- %d. %s\n", numero, titre);
    fflush(stdout); /* rien à dupliquer dans l'enfant */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        executer_variante(numero);
    }
    if (attendre(pid, &status) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status)) {
        printf("    code de sortie : %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("    tué par le signal %d\n", WTERMSIG(status));
    }
}

int main(void)
{
    lancer_variante(1, "execl(\"/bin/ls\", \"ls\", \"-ld\", \"/tmp\", NULL)");
    lancer_variante(2, "execlp(\"ls\", \"ls\", \"-ld\", \"/tmp\", NULL)");
    lancer_variante(3, "execv(\"/bin/ls\", args)");
    lancer_variante(4, "execvp(args[0], args)");
    lancer_variante(5, "execle(\"/usr/bin/env\", \"env\", NULL, env_vide)  : n'affiche rien");
    lancer_variante(6, "execve(\"/bin/ls\", args, environ)");
    lancer_variante(7, "ERREUR execlp(\"echo\", \"bonjour\", \"le monde\", NULL)");
    lancer_variante(8, "ERREUR execlp(\"ls -ld\", \"ls -ld\", NULL)");
    lancer_variante(9, "ERREUR execl(\"ls\", \"ls\", NULL)");
    return EXIT_SUCCESS;
}
