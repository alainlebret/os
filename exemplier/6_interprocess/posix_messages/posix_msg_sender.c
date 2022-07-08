/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret (alain.lebret [at] ensicaen [dot] fr)
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

#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define FOREVER for (;;)

/**
 * @file posix_msg_sender.c
 * @author Alain Lebret
 * @version	1.0
 * @date 2016-11-01
 */

/*
 * Sender application using POSIX mqueue
 * Compilation: gcc -o mqueue_sender posix_msg_sender.c -lrt
 */
int main(int argc, char *argv[])
{
    mqd_t mq;
    struct timeval hour;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s mqueue_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mq = mq_open(argv[1], O_WRONLY | O_CREAT, 0600, NULL);
    if (mq == (mqd_t) - 1) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    FOREVER {
        gettimeofday(&hour, NULL);
        mq_send(mq, (char *) &hour, sizeof(hour), 1);
    }

    /* Unreachable: use <Ctrl-C> to exit */
}
