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
#include <fcntl.h>     /* O_* */
#include <stdio.h>     /* fprintf(), perror() */
#include <stdlib.h>    /* strtoul() */
#include <string.h>    /* strlen() */

#ifdef __APPLE__
#include "macosx/mqueue.h"
#elif __linux__
#include <mqueue.h>
#endif

/**
 * @file mq_news_sender.c
 * @brief Sender of one message with a priority to the POSIX queue "/news".
 *
 * Course "Operating Systems", chapter « Files de messages », slide
 * « Exemple – émetteur ». To be used with mq_news_receiver.c.
 *
 * The sender opens the existing queue (created by the receiver) in write
 * only mode, deposits one message with the given priority and closes the
 * queue. It does not call mq_unlink(): only the creator removes the queue.
 *
 * \code{.bash}
 *   $ ./mq_news_receiver &
 *   $ ./mq_news_sender 10 BBB
 *   [10] BBB
 * \endcode
 */

#define NOM "/news"      /* created by the receiver */

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage : %s prio msg\n", argv[0]);
        return EXIT_FAILURE;
    }
    mqd_t mq = mq_open(NOM, O_WRONLY);
    if (mq == (mqd_t) -1) {
        perror("mq_open");
        return EXIT_FAILURE;
    }
    /* strtoul: string -> unsigned integer */
    unsigned int prio = (unsigned int) strtoul(argv[1], NULL, 10);
    int r = mq_send(mq, argv[2], strlen(argv[2]), prio);
    if (r == -1)
        perror("mq_send");
    mq_close(mq);        /* no mq_unlink here */
    return r == -1 ? EXIT_FAILURE : EXIT_SUCCESS;
}
