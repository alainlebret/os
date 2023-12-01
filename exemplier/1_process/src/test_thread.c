#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int main(void)
{
    printf("%d / %ld\n",getpid(), pthread_self());
    return EXIT_SUCCESS;
}

