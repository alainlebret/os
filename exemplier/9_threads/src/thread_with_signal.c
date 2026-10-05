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

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

/**
 * @file thread_with_signal.c
 *
 * A simple program using threads and signal: after 10 seconds, main()
 * sends SIGINT to the secondary thread with pthread_kill(); the handler
 * (async-signal-safe: write() only) sets a flag that stops the thread.
 *
 * Compile with gcc -Wall -Wextra -pedantic -std=c11 -pthread thread_with_signal.c
 */

typedef struct data {
    char name[12];
    int age;
} data_t;

static volatile sig_atomic_t stop_requested = 0;

/**
 * Stops the program if a pthread_*() call failed: these functions return an
 * error number (0 on success) and do not set errno.
 */
static void check(int err, const char *what) {
    if (err != 0) {
        fprintf(stderr, "%s : %s\n", what, strerror(err));
        exit(EXIT_FAILURE);
    }
}

void handle_signal(int sig) {
    (void) sig;  /* Mark sig as unused */
    int saved_errno = errno;   /* write() may modify errno */
    write(STDOUT_FILENO, "Caught signal SIGINT\n", 21);
    errno = saved_errno;
    stop_requested = 1;
}

void *func(void *arg) {
    data_t *p = (data_t *) arg;

    while (!stop_requested) {
        fprintf(stderr, "This is from thread function\n");
        snprintf(p->name, sizeof(p->name), "Linux Lover");
        p->age = 55;
        sleep(2);
    }
    return NULL;
}

int main(void) {
    pthread_t tid;
    pthread_attr_t attr;
    data_t d;
    data_t *ptr = &d;
    struct sigaction action;

    action.sa_handler = &handle_signal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    pthread_attr_init(&attr);
    check(pthread_create(&tid, &attr, func, ptr), "pthread_create");
    sleep(10);
    pthread_kill(tid, SIGINT);

    check(pthread_join(tid, NULL), "pthread_join");
    pthread_attr_destroy(&attr);
    fprintf(stderr, "Name: %s\n", ptr->name);
    fprintf(stderr, "Age: %d\n", ptr->age);

    return EXIT_SUCCESS;
}
