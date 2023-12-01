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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#ifdef __APPLE__
#include "macosx/mqueue.h"
#elif __linux__
#include <mqueue.h>
#endif

/**
 * @file posix_msg_receiver.c
 * @brief A Receiver application using POSIX mqueue
 *
 * Compilation: gcc -o mqueue_receiver posix_msg_receiver.c -lrt
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2016-11-01
 */

#define FOREVER for (;;)

int main(int argc, char *argv[])
{
    int size;
    int nb_messages;
    long int duration;
    long int max_duration;
    long int min_duration;
    long int sum_durations;
    mqd_t mq;
    char *buffer;
    struct mq_attr attr;
    struct timeval hour;
    struct timeval *received_hour;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s mqueue_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mq = mq_open(argv[1], O_RDONLY, 0600, NULL);
    if (mq == (mqd_t) - 1) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    if (mq_getattr(mq, &attr) != 0) {
        perror("Unable to get attributes from the message queue.");
        exit(EXIT_FAILURE);
    }

    size = attr.mq_msgsize;
    buffer = (char *) malloc(size);
    if (buffer == NULL) {
        perror("Unable to allocate memory for the buffer");
        exit(EXIT_FAILURE);
    }

    received_hour = (struct timeval *) buffer;

    FOREVER {
        nb_messages = 0;
        max_duration = 0;
        min_duration = -1;
        sum_durations = 0;

        do {
            mq_receive(mq, buffer, size, NULL);

            gettimeofday(&hour, NULL);
            duration = hour.tv_sec - received_hour->tv_sec;
            duration *= 1000000;
            duration += hour.tv_usec - received_hour->tv_usec;

            if (nb_messages > 0) { /* Ignore first message */
                if (max_duration < duration) {
                    max_duration = duration;
                    if ((min_duration == -1) || (min_duration > duration)) {
                        min_duration = duration;
                        sum_durations += duration;
                    }
                }
            }
            nb_messages++;
        } while (nb_messages < 100000); /* approximately 1 sec */

        fprintf(stdout, "min = %3ld   max = %3ld moy= %5.1f\n",
                min_duration, max_duration, ((float) sum_durations) / (nb_messages - 1));
    }

}

