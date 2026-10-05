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
#include <pthread.h>    /* pthread_*, pthread_mutex_*, pthread_cond_* */
#include <stdio.h>      /* printf(), fprintf() */
#include <stdlib.h>     /* srand(), rand(), exit() */
#include <string.h>     /* strerror() */
#include <time.h>       /* time() */
#include <unistd.h>     /* sleep() */

/**
 * @file alarm_temperature.c
 *
 * Condition variable example: a thread measures a temperature every
 * second; another thread sleeps until the temperature leaves the range
 * 16 to 24 °C, then displays an alarm.
 * Lecture: chapter os-10 « Synchronisation » ("Exemple – alarme et
 * température").
 *
 * Key points:
 *   - the predicate (alarme) is protected by the mutex m;
 *   - pthread_cond_wait() is always called in a while (!predicate) loop
 *     (spurious wakeups);
 *   - the predicate is modified under the mutex, then signalled;
 *   - printf() is called after unlocking: short critical section;
 *   - main() ends with pthread_exit(): the threads go on (Ctrl-C to stop).
 * See alarm_temperature_broadcast.c for several alarm threads.
 *
 * \code{.bash}
 *   $ ./alarm_temperature
 *   Température : 21
 *   Température : 20
 *   ...
 *   Température : 25
 *   ALARME
 * \endcode
 */

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  c = PTHREAD_COND_INITIALIZER;
int alarme = 0;                /* the predicate */

void *send_alarm(void *unused);
void *get_temperature(void *unused);

int main(void) {
    pthread_t t1, t2;
    int err;

    srand(time(NULL));
    if ((err = pthread_create(&t1, NULL, get_temperature, NULL)) != 0 ||
        (err = pthread_create(&t2, NULL, send_alarm, NULL)) != 0) {
        fprintf(stderr, "pthread_create : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    pthread_exit(NULL);        /* the threads go on */
}

void *send_alarm(void *unused) {
    (void) unused;
    while (1) {
        pthread_mutex_lock(&m);
        while (!alarme) {
            pthread_cond_wait(&c, &m);
        }
        alarme = 0;            /* event consumed */
        pthread_mutex_unlock(&m);
        printf("ALARME\n");
    }
    return NULL;
}

void *get_temperature(void *unused) {
    (void) unused;
    int t = 20;
    while (1) {
        t += rand() % 3 - 1;   /* -1, 0 or +1 */
        printf("Température : %d\n", t);
        if (t < 16 || t > 24) {
            pthread_mutex_lock(&m);
            alarme = 1;        /* predicate true */
            pthread_cond_signal(&c);
            pthread_mutex_unlock(&m);
        }
        sleep(1);
    }
    return NULL;
}
