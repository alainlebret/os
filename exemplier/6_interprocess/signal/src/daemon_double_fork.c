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
#include <stdio.h>     /* perror(), snprintf() */
#include <stdlib.h>    /* exit() */
#include <string.h>    /* strlen() */
#include <unistd.h>    /* fork(), setsid(), chdir(), dup2(), _exit() */
#include <fcntl.h>     /* open() */
#include <signal.h>    /* sigaction() */
#include <time.h>      /* time() */
#include <sys/types.h> /* pid_t */
#include <sys/stat.h>  /* umask() */

/**
 * @file daemon_double_fork.c
 * @brief Creates a daemon using the double fork technique.
 *
 * Course "Operating Systems", chapter « Signaux », part « Démons », slides
 * « Exemple de création d'un démon » and « Après le double fork ».
 *
 * 1. first fork(): the initial parent exits; the child is not a group leader;
 * 2. setsid(): the child creates a new session, without controlling terminal;
 * 3. second fork(): the grandchild is not a session leader and can never
 *    reacquire a controlling terminal.
 *
 * The daemon then goes to "/", sets its umask, redirects the standard
 * descriptors to /dev/null and writes its PID in a file. It appends a line
 * to a log file every 5 seconds and stops cleanly on SIGTERM.
 *
 * The PID file is written in /tmp so that the example runs without
 * privileges (a real daemon uses /run on Linux, /var/run elsewhere).
 * Today, systemd (Linux) or launchd (macOS) start services without any
 * double fork.
 *
 * \code{.bash}
 *   $ ./daemon_double_fork
 *   $ cat /tmp/daemon_double_fork.pid
 *   $ ps -o pid,ppid,pgid,sess,tty,comm -p $(cat /tmp/daemon_double_fork.pid)
 *   $ tail /tmp/daemon_double_fork.log
 *   $ kill $(cat /tmp/daemon_double_fork.pid)
 * \endcode
 */

#define PID_FILE "/tmp/daemon_double_fork.pid"
#define LOG_FILE "/tmp/daemon_double_fork.log"
#define PERIOD   5

static volatile sig_atomic_t stop = 0;

/**
 * @brief Handler of SIGTERM: asks the daemon to stop.
 * @param sig Number of the received signal.
 */
static void handle_sigterm(int sig) {
    (void) sig;
    stop = 1;
}

/**
 * @brief Detaches the process from its terminal (double fork).
 */
static void become_daemon(void) {
    int fd;
    pid_t pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);   /* error */
    if (pid > 0)
        _exit(EXIT_SUCCESS);  /* initial parent: end */

    if (setsid() == -1)       /* new session */
        exit(EXIT_FAILURE);

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);   /* error */
    if (pid > 0)
        _exit(EXIT_SUCCESS);  /* first child: end */

    /* beginning of the daemon */
    if (chdir("/") == -1)     /* do not block the unmounting of a filesystem */
        exit(EXIT_FAILURE);
    umask(0);                 /* control the rights of created files */

    /* Standard descriptors redirected to /dev/null */
    fd = open("/dev/null", O_RDWR);
    if (fd == -1)
        exit(EXIT_FAILURE);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > STDERR_FILENO)
        close(fd);
}

/**
 * @brief Writes the PID of the daemon in PID_FILE.
 * @return 0 on success, -1 on error.
 */
static int write_pid_file(void) {
    char line[32];
    int fd = open(PID_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
        return -1;
    snprintf(line, sizeof(line), "%ld\n", (long) getpid());
    if (write(fd, line, strlen(line)) == -1) {
        close(fd);
        return -1;
    }
    return close(fd);
}

int main(void) {
    struct sigaction sa;
    char line[64];
    int log_fd;

    become_daemon();

    /* Handler first: a SIGTERM sent as soon as the PID file exists is caught,
       and the PID file is removed */
    sa.sa_handler = handle_sigterm;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGTERM, &sa, NULL) == -1)
        exit(EXIT_FAILURE);

    if (write_pid_file() == -1)
        exit(EXIT_FAILURE);

    log_fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd == -1)
        exit(EXIT_FAILURE);

    /* Service loop: sleep() is interrupted by SIGTERM */
    while (!stop) {
        snprintf(line, sizeof(line), "daemon %ld alive at %ld\n",
                 (long) getpid(), (long) time(NULL));
        if (write(log_fd, line, strlen(line)) == -1)
            break;
        sleep(PERIOD);
    }

    snprintf(line, sizeof(line), "daemon %ld stopped\n", (long) getpid());
    (void) write(log_fd, line, strlen(line));
    close(log_fd);
    unlink(PID_FILE);

    return EXIT_SUCCESS;
}
