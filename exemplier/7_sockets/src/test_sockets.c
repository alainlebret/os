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
#include <stdio.h>
#include <stdlib.h>     /* exit() */
#include <string.h>     /* memset() */
#include <unistd.h>     /* close() */
#include <signal.h>
#include <sys/types.h>
#include <netinet/in.h> /* Internet structures and functions. */
#include <sys/socket.h> /* Socket functions. */

/**
 * @file test_sockets.c
 *
 * @brief Opens 8 sockets of different domains and types, then waits for
 * Ctrl-C, so that they can be observed with "lsof -p <PID>". The raw
 * sockets (SOCK_RAW) need the root privileges: without them, socket() fails
 * (EPERM on macOS, EPROTONOSUPPORT or EPERM on Linux).
 */

/**
 * New handler of the SIGINT signal: it only interrupts pause() (printf()
 * and exit() are not async-signal-safe, so main() displays the message).
 * @param sig Number of the signal
 */
void handle(int sig) {
    (void) sig;
}

int main(void) {
    int s1; /* socket descriptors for the test */
    int s2;
    int s3;
    int s4;
    int s5;
    int s6;
    int s7;
    int s8;
    struct sigaction action;

    memset(&action, 0, sizeof(action));
    action.sa_handler = &handle;
    sigemptyset(&action.sa_mask);

    /* install the new handler of the SIGINT signal */
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* open sockets in different modes */
    s1 = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s1 == -1) {
        perror("Error opening a stream Unix socket using default protocol");
    } else {
        printf("A stream Unix socket (s1) using default protocol has been opened\n");
    }

    s2 = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (s2 == -1) {
        perror("Error opening a datagram Unix socket using default protocol");
    } else {
        printf("A datagram Unix socket (s2) using default protocol has been opened\n");
    }

    s3 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s3 == -1) {
        perror("Error opening a stream socket using TCP");
    } else {
        printf("A stream socket (s3) using TCP has been opened\n");
    }

    s4 = socket(AF_INET, SOCK_STREAM, 0);
    if (s4 == -1) {
        perror("Error opening a stream socket using default protocol");
    } else {
        printf("A stream socket (s4) using default protocol has been opened\n");
    }

    s5 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s5 == -1) {
        perror("Error opening a datagram socket using UDP");
    } else {
        printf("A datagram socket (s5) using UDP has been opened\n");
    }

    s6 = socket(AF_INET, SOCK_DGRAM, 0);
    if (s6 == -1) {
        perror("Error opening a datagram socket using default protocol");
    } else {
        printf("A datagram socket (s6) using default protocol has been opened\n");
    }

    s7 = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (s7 == -1) {
        perror("Error opening a raw socket using IP");
    } else {
        printf("A raw socket (s7) using IP has been opened\n");
    }

    s8 = socket(AF_INET, SOCK_RAW, 0);
    if (s8 == -1) {
        perror("Error opening a raw socket using default protocol");
    } else {
        printf("A raw socket (s8) using default protocol has been opened\n");
    }

    /* wait for Ctrl-C (SIGINT) */
    printf("PID %ld: observe the sockets with \"lsof -p %ld\", then Ctrl-C\n",
           (long) getpid(), (long) getpid());
    pause();
    printf("SIGINT signal received!\n");

    int sockets[] = {s1, s2, s3, s4, s5, s6, s7, s8};
    for (int i = 0; i < 8; i++) {
        if (sockets[i] != -1) { /* only the sockets actually opened */
            close(sockets[i]);
        }
    }

    return EXIT_SUCCESS;
} 
