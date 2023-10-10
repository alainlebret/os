/**
 * @file mqueue.h
 *
 * @author Stanislav Pankevich (https://github.com/stanislaw/posix-macos-addons)
 *
 * A memory queue mechanism for Mac OS X
 */

#ifndef _POSIX_MACOS_MQUEUE_H_
#define _POSIX_MACOS_MQUEUE_H_

#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/* \todo: MacOS should have this defined */
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/**
 * Opaque datatype.
 */
typedef struct mq_info *mqd_t;

struct mq_attr {
    long mq_flags;   /*!< Message queue flag: O_NONBLOCK */
    long mq_maxmsg;  /*!< Max number of messages allowed on queue */
    long mq_msgsize; /*!< Max size of a message (in bytes) */
    long mq_curmsgs; /*!< Number of messages currently on queue */
};

/**
 * One mq_hdr{} per queue, at beginning of mapped file.
 */
struct mq_hdr {
    struct mq_attr mqh_attr;   /*!< The queue's attributes */
    long mqh_head;             /*!< Index of the first message */
    long mqh_free;             /*!< Index of the first free message */
    long mqh_nwait;            /*!< #threads blocked in mq_receive() */
    pid_t mqh_pid;             /*!< Nonzero PID if mqh_event set */
    struct sigevent mqh_event; /*!< For mq_notify() */
    pthread_mutex_t mqh_lock;  /*!< Mutex lock */
    pthread_cond_t mqh_wait;   /*!< and condition variable */
};

/**
 * One mymsg_hdr{} at the front of each message in the mapped file.
 */
struct mymsg_hdr {
    long msg_next;   /*!< Index of next on linked list (must be first member in
                      struct). */
    ssize_t msg_len; /*!< Actual length */
    unsigned int msg_prio; /*!< Priority */
};

/**
 * One mq_info{} malloc'ed per process per mq_open().
 */
struct mq_info {
    struct mq_hdr *mqi_hdr; /*!< Start of mmap'ed region. */
    long mqi_magic;         /*!< Magic number if open */
    int mqi_flags;          /*!< Flags for this process */
};

#define MQI_MAGIC 0x98765432

/** Size of message in file is rounded up for alignment. */
#define MSGSIZE(i) ((((i) + sizeof(long) - 1) / sizeof(long)) * sizeof(long))

int mq_close(mqd_t);

int mq_getattr(mqd_t, struct mq_attr *);

int mq_notify(mqd_t, const struct sigevent *);

mqd_t mq_open(const char *, int, ...);

ssize_t mq_receive(mqd_t, char *, size_t, unsigned int *);

int mq_send(mqd_t, const char *, size_t, unsigned int);

int mq_setattr(mqd_t, const struct mq_attr *, struct mq_attr *);

int mq_unlink(const char *name);

int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
                 unsigned msg_prio, const struct timespec *abs_timeout);

ssize_t mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len,
                        unsigned *msg_prio, const struct timespec *abs_timeout);

#ifdef __cplusplus
}
#endif

#endif /* _POSIX_MACOS_MQUEUE_H_ */
