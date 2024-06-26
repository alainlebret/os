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
#include <sys/types.h>
#include <sys/select.h>
#include <time.h>

/**
 * @file microseconds_sleep.c
 *
 * A simple program to provide microsecond sleeping.
 */

/**
 * Sleeps for a number of microseconds.
 */
void us_sleep(int nb_usec) {
    struct timeval waiting;

    waiting.tv_sec = nb_usec / 1000000;
    waiting.tv_usec = nb_usec % 1000000;
    select(0, NULL, NULL, NULL, &waiting);
}

int main(int argc, char *argv[]) {
    clock_t begin_time;
    clock_t end_time;
    double duration;

    begin_time = clock();
    us_sleep(2000000);
    end_time = clock();
    duration = (double) (end_time - begin_time) / CLOCKS_PER_SEC;
    printf("%2.1f seconds\n", duration);

    return EXIT_SUCCESS;
}
