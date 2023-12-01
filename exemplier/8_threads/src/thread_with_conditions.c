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

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @file thread_with_conditions.c
 *
 * Example using pthreads and conditions.
 * The program creates NUM_THREADS threads. (NUM_THREADS-1) of those threads
 * increment a "count" variable, while the last one reads the value of "count".
 * When "count" reaches a limit (COUNT_LIMIT), the waiting thread is signaled by
 * one of the incrementing threads. The waiting thread "awakens" and then
 * modifies count. The program continues until the incrementing threads reach
 * TOTAL_COUNT. The main program prints the final value of count.
 * This example is adapted from the code at:
 * https://computing.llnl.gov/tutorials/pthreads/
 *
 * @author Blaise Barney -- Lawrence Livermore National Laboratory (03/07/2017)
 * @author Alain Lebret (06/08/2017)
 * @version	1.0
 * @date 2017-06-08
 */

#define NUM_THREADS  4
#define TOTAL_COUNT 10
#define COUNT_LIMIT 12

int count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *inc_count(void *t)
{
    int i;
    long my_id = (long) t;

    for (i = 0; i < TOTAL_COUNT; i++) {
        pthread_mutex_lock(&count_mutex);
        count++;

        /*
         * Check the value of count and signal waiting thread when
         * condition is reached. Note that this occurs while mutex
         * is locked.
         */
        if (count == COUNT_LIMIT) {
            printf("inc_count(): thread %ld, count = %d  Threshold reached. ",
                   my_id, count);
            pthread_cond_signal(&count_threshold_cv);
            printf("Just sent signal.\n");
        }
        printf("inc_count(): thread %ld, count = %d, unlocking mutex\n",
               my_id, count);
        pthread_mutex_unlock(&count_mutex);

        /* Do some work so threads can alternate on mutex lock */
        sleep(1);
    }
    pthread_exit(NULL);
}

void *watch_count(void *t)
{
    long my_id = (long) t;

    printf("watch_count(): thread %ld\n", my_id);

    /*
     * Lock mutex and wait for signal.  Note that the pthread_cond_wait
     * routine will automatically and atomically unlock mutex while it
     * waits. Also, note that if COUNT_LIMIT is reached before this routine
     * is run by the waiting thread, the loop will be skipped to prevent
     * pthread_cond_wait from never returning.
     */
    pthread_mutex_lock(&count_mutex);
    while (count < COUNT_LIMIT) {
        printf("watch_count(): thread %ld Count= %d. Going into wait...\n", my_id, count);
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
        printf("watch_count(): thread %ld Condition signal received. Count= %d\n", my_id, count);
    }
    printf("watch_count(): thread %ld Updating the value of count...\n", my_id);
    count += 125;
    printf("watch_count(): thread %ld count now = %d.\n", my_id, count);
    printf("watch_count(): thread %ld Unlocking mutex.\n", my_id);
    pthread_mutex_unlock(&count_mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i;
    long t1;
    long t2;
    long t3;
    long t4;
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;

    t1 = 1;
    t2 = 2;
    t3 = 3;
    t4 = 4;

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_threshold_cv, NULL);

    /* For portability, explicitly create threads in a joinable state */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], &attr, watch_count, (void *) t1);
    pthread_create(&threads[1], &attr, inc_count, (void *) t2);
    pthread_create(&threads[2], &attr, inc_count, (void *) t3);
    pthread_create(&threads[3], &attr, inc_count, (void *) t4);

    /* Wait for all threads to complete */
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("main(): waited and joined with %d threads. Final value of count = %d. Done.\n",
           NUM_THREADS, count);

    /* Clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);
	
    return EXIT_SUCCESS;
}
