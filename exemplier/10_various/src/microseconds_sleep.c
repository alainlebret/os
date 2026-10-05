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
#define _POSIX_C_SOURCE 200809L /* select(), clock_gettime() */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <time.h>

/**
 * @file microseconds_sleep.c
 *
 * A simple program to provide microsecond sleeping.
 *
 * The elapsed time is measured with clock_gettime(CLOCK_MONOTONIC): clock()
 * would measure the CPU time, which stays close to 0 while the process sleeps.
 */

/**
 * Sleeps for a number of microseconds: select() with no descriptor only
 * waits for its timeout. The unit is the microsecond, but the real delay
 * depends on the scheduler (nanosleep() is the standard alternative).
 */
void us_sleep(int nb_usec) {
    struct timeval waiting;

    waiting.tv_sec = nb_usec / 1000000;
    waiting.tv_usec = nb_usec % 1000000;
    if (select(0, NULL, NULL, NULL, &waiting) == -1) {
        perror("select"); /* e.g. EINTR: interrupted by a signal */
    }
}

int main(void) {
    struct timespec begin_time;
    struct timespec end_time;
    double duration;

    clock_gettime(CLOCK_MONOTONIC, &begin_time);
    us_sleep(2000000);
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    duration = (double) (end_time.tv_sec - begin_time.tv_sec)
               + (double) (end_time.tv_nsec - begin_time.tv_nsec) / 1e9;
    printf("%2.1f seconds\n", duration);

    return EXIT_SUCCESS;
}
