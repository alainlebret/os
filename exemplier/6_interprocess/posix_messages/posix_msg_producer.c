/* Emetteur avec file de messages POSIX */
/* http://www.blaess.fr/christophe/ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

#define __MAC_OS_X
#ifdef __MAC_OS_X
#else
#include <mqueue.h>
#endif

#define EVER ;;

int main(int argc, char *argv[]) {
#ifndef __MAC_OS_X
	mqd_t mq;
#endif
	struct timeval heure;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s nom_file_message\n", argv[0]);
		exit(EXIT_FAILURE);
	}
#ifndef __MAC_OS_X
	mq = mq_open(argv[1], O_WRONLY | O_CREAT, 0600, NULL);
	if (mq == (mqd_t) - 1) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}
#endif

	for (EVER) {
		gettimeofday(&heure, NULL);
#ifndef __MAC_OS_X
		mq_send(mq, (char *) &heure, sizeof(heure), 1);
#endif
	}

	exit(EXIT_SUCCESS); /* unreachable code */
}
