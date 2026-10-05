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
#include <stdio.h>     /* printf(), perror() */
#include <stdlib.h>    /* malloc(), free() */
#include <sys/types.h> /* ssize_t */

#ifdef __APPLE__
#include "macosx/mqueue.h"
#elif __linux__
#include <mqueue.h>
#endif

/**
 * @file mq_news_receiver.c
 * @brief Receiver (and creator) of the POSIX message queue "/news".
 *
 * Course "Operating Systems", chapter « Files de messages », slide
 * « Exemple – récepteur (créateur) ». To be used with mq_news_sender.c.
 *
 * The receiver creates the queue, blocks in mq_receive() while the queue is
 * empty, prints the oldest message of highest priority, then removes the
 * queue: it is the creator and the exchange is over (close != unlink).
 * The reception buffer must be at least mq_msgsize bytes long (EMSGSIZE
 * otherwise), and a message is not a '\0'-terminated string.
 *
 * \code{.bash}
 *   $ ./mq_news_receiver &
 *   $ ./mq_news_sender 10 BBB
 *   [10] BBB
 * \endcode
 */

#define NOM "/news"

int main(void) {
    struct mq_attr attr;
    unsigned int prio;
    mqd_t mq = mq_open(NOM, O_CREAT | O_RDONLY, 0600, NULL);
    if (mq == (mqd_t) -1) {
        perror("mq_open");
        return EXIT_FAILURE;
    }
    if (mq_getattr(mq, &attr) == -1) {  /* before malloc */
        perror("mq_getattr");
        mq_close(mq);
        mq_unlink(NOM);
        return EXIT_FAILURE;
    }
    char *buf = malloc(attr.mq_msgsize);
    if (buf == NULL) {
        perror("malloc");
        mq_close(mq);
        mq_unlink(NOM);
        return EXIT_FAILURE;
    }
    ssize_t n = mq_receive(mq, buf, attr.mq_msgsize, &prio);
    if (n != -1)
        printf("[%u] %.*s\n", prio, (int) n, buf);
    else
        perror("mq_receive");
    free(buf);
    mq_close(mq);
    mq_unlink(NOM);      /* creator, exchange over */
    return n == -1 ? EXIT_FAILURE : EXIT_SUCCESS;
}
