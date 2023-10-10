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
#include <string.h>
#include <unistd.h>

#ifdef __APPLE__
#include "macosx/mqueue.h"
#elif __linux__
#include <mqueue.h>
#endif

#define QUEUE_NAME "/myqueue"
#define MAX_MSG_SIZE 1024

typedef struct {
    char sentence[MAX_MSG_SIZE];
    int priority;
} Message;

int main(void) {
    mqd_t mq;
    struct mq_attr attr;
    Message msg;

    /* Open the message queue */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10; /* Maximum number of messages in the queue */
    attr.mq_msgsize = sizeof(Message); /* Maximum message size */
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0600, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Enter a sentence (or 'exit' to quit): ");
        fgets(msg.sentence, MAX_MSG_SIZE, stdin);

        if (strcmp(msg.sentence, "exit\n") == 0) {
            break;
        }

        printf("Enter priority (0-9): ");
        scanf("%d", &msg.priority);
        getchar(); /* Consume newline */

        /* Send the message to the queue */
        if (mq_send(mq, (const char *)&msg, sizeof(Message), msg.priority) == -1) {
            perror("mq_send");
            continue;
        }
    }

    /* Close and unlink the message queue */
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}
