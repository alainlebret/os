/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2023 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <sys/time.h>
#include <string.h>
#include "message.h"

/**
 * @file test_messages.c
 * @brief Test message handling functions.
 */

int main(int argc, char *argv[]) {
    message_t message;
    struct timeval time;
    double begin;
    double end;

    printf("\n---------- Beginning of %s ----------\n", argv[0]);

    gettimeofday(&time, NULL);
    begin = time.tv_sec + (time.tv_usec / 1000000.0);

    printf("Filling the message with some A\n");
    msg_fill(&message, 'A');

    printf("Displaying of the message\n");
    msg_display(&message);

    gettimeofday(&time, NULL);
    end = time.tv_sec + (time.tv_usec / 1000000.0);

    printf("\nDuration of the program %s = %.6lf seconds.\n", argv[0], end - begin);

    return EXIT_SUCCESS;
}

