#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>

#include "semun.h"

static void usageError(const char *prog, const char *msg)
{
    if(NULL == prog)
    {
        fprintf(stderr, "NULL pointer\n");
        return;
    }

    if(NULL != msg)
    {
        fprintf(stderr, "%s\n", msg);
    }

    fprintf(stderr, "Usage :\n"
                    "        %s init-value\n"
                    "        %s semid operation\n", prog, prog);

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int semid, nsems;
    union semun arg;
    
    if((argc != 2) || (0 == strcmp(argv[1], "--help")))
    {
        usageError(argv[0], NULL);
    }

    /* Create nsems semaphore */
    nsems = atoi(argv[1]);
    semid = semget(IPC_PRIVATE, nsems, S_IRUSR | S_IWUSR);
    if(-1 == semid)
    {
        fprintf(stderr, "Invoke semget() failed, errno(%d), strerror(%s)\n",
                errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Semaphore ID = %d\n", semid);

    exit(EXIT_SUCCESS);
}
