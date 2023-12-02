/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>

#ifdef __APPLE__
#include "macosx/mqueue.h"
#elif __linux__
#include <mqueue.h>
#endif

/**
 * @file posix_msg_sender.c
 * @brief A sender application using POSIX mqueue.
 *
 * @author Alain Lebret
 * @version	1.1
 * @date 2023-12-01
 */

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

int main(int argc, char *argv[])
{
    mqd_t mq;
    struct timeval hour;	
    struct sigaction action;

    /* Clean up the structure before using it */
    memset(&action, '\0', sizeof(action));

    /* Set the new handler */
    action.sa_handler = &handle_sigint;

    /* Install the new handler of the SIGINT signal */
    sigaction(SIGINT, &action, NULL);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s mqueue_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mq = mq_open(argv[1], O_WRONLY | O_CREAT, 0600, NULL);
    if (mq == (mqd_t) - 1) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    while (keep_running) {
        gettimeofday(&hour, NULL);
        if (mq_send(mq, (char *) &hour, sizeof(hour), 1) == -1) {
            perror("mq_send");
            break;
        }
    }

    mq_close(mq);
}
