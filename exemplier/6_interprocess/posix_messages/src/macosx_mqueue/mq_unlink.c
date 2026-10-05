#include "macosx/mqueue-internal.h"
#include "macosx/mqueue.h"

#include <errno.h>

int mq_unlink(const char *pathname) {
    char fs_pathname[MQ_FS_NAME_MAX];
    int n;

    if ((n = mq_get_fs_pathname(pathname, fs_pathname)) != 0) {
        errno = n; /* EINVAL or ENAMETOOLONG */
        return -1;
    }
    if (unlink(fs_pathname) == -1) {
        return -1;
    }
    return 0;
}
