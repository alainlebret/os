#include "macosx/logger.h"
#include "macosx/mqueue.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>

ssize_t mq_timedreceive(mqd_t mqd, char *ptr, size_t maxlen, unsigned *priop,
                        const struct timespec *abs_timeout) {
    int n;
    long index;
    int8_t *mptr;
    ssize_t len;
    struct mq_hdr *mqhdr;
    struct mq_attr *attr;
    struct mymsg_hdr *msghdr;
    struct mq_info *mqinfo;

    mqinfo = mqd;
    if (mqinfo->mqi_magic != MQI_MAGIC) {
        errno = EBADF;
        return (-1);
    }
    mqhdr = mqinfo->mqi_hdr; /* struct pointer */
    mptr = (int8_t *) mqhdr;  /* byte pointer */
    attr = &mqhdr->mqh_attr;
    if ((n = pthread_mutex_lock(&mqhdr->mqh_lock)) != 0) {
        errno = n;
        return (-1);
    }

    if (maxlen < (size_t) attr->mq_msgsize) {
        errno = EMSGSIZE;
        goto err;
    }
    if (attr->mq_curmsgs == 0) { /* queue is empty */
        if (mqinfo->mqi_flags & O_NONBLOCK) {
            errno = EAGAIN;
            goto err;
        }
        /* wait for a message to be placed onto queue */
        mqhdr->mqh_nwait++;
        while (attr->mq_curmsgs == 0) {
            int wait_result = pthread_cond_timedwait(&mqhdr->mqh_wait,
                                                     &mqhdr->mqh_lock,
                                                     abs_timeout);
            /* Only ETIMEDOUT ends the wait. On macOS, a process-shared
             * condition variable waited on from two processes that map the
             * queue at different addresses returns EINVAL at once: the wait
             * then degrades into active polling, but remains correct. */
            if (wait_result == ETIMEDOUT) {
                mqhdr->mqh_nwait--; /* no longer waiting */
                errno = ETIMEDOUT;
                goto err;
            }
        }
        mqhdr->mqh_nwait--;
    }

    if ((index = mqhdr->mqh_head) == 0) {
        log_info("mq_receive: curmsgs = %ld; head = 0\n", attr->mq_curmsgs);
    }

    msghdr = (struct mymsg_hdr *) &mptr[index];
    mqhdr->mqh_head = msghdr->msg_next; /* new head of list */
    len = msghdr->msg_len;
    memcpy(ptr, msghdr + 1, len); /* copy the message itself */
    if (priop != NULL)
        *priop = msghdr->msg_prio;

    /* just-read message goes to front of free list */
    msghdr->msg_next = mqhdr->mqh_free;
    mqhdr->mqh_free = index;

    /* wake up everyone blocked in mq_send waiting for room: signaling only
     * when the queue was full loses wake-ups with several blocked senders.
     * (no assert() around these calls: compiled out with NDEBUG) */
    pthread_cond_broadcast(&mqhdr->mqh_wait);
    attr->mq_curmsgs--;

    pthread_mutex_unlock(&mqhdr->mqh_lock);
    return (len);

    err:
    pthread_mutex_unlock(&mqhdr->mqh_lock);
    return (-1);
}
