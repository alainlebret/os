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
 * @file alarm_temperature_broadcast.c
 *
 * Variant of alarm_temperature.c with two alarm threads, both woken up by
 * pthread_cond_broadcast().
 * Lecture: chapter os-10 « Synchronisation » ("Plusieurs threads en
 * attente : broadcast" and, in the appendices, "Broadcast – un prédicat
 * par thread").
 *
 * With the predicate alarme = 0 of alarm_temperature.c, the first thread
 * woken up would consume the event and the other would go back to sleep.
 * Here the shared predicate is a counter of alarms (nb_alarmes), and each
 * thread remembers in a private variable (vues) the last value it saw:
 * both threads are informed of each alarm.
 * Ctrl-C to stop.
 *
 * \code{.bash}
 *   $ ./alarm_temperature_broadcast
 *   Température : 21
 *   ...
 *   Température : 15
 *   ALARME (thread 2)
 *   ALARME (thread 3)
 * \endcode
 */

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  c = PTHREAD_COND_INITIALIZER;
int nb_alarmes = 0;            /* shared */

void *send_alarm(void *arg) {
    long num = (long) arg;
    int vues = 0;              /* private */
    while (1) {
        pthread_mutex_lock(&m);
        while (nb_alarmes == vues) {
            pthread_cond_wait(&c, &m);
        }
        vues = nb_alarmes;
        pthread_mutex_unlock(&m);
        printf("ALARME (thread %ld)\n", num);
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
            nb_alarmes++;      /* new event */
            pthread_cond_broadcast(&c);
            pthread_mutex_unlock(&m);
        }
        sleep(1);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2, t3;
    int err;

    srand(time(NULL));
    if ((err = pthread_create(&t1, NULL, get_temperature, NULL)) != 0 ||
        (err = pthread_create(&t2, NULL, send_alarm, (void *) 2L)) != 0 ||
        (err = pthread_create(&t3, NULL, send_alarm, (void *) 3L)) != 0) {
        fprintf(stderr, "pthread_create : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    pthread_exit(NULL);        /* the threads go on */
}
