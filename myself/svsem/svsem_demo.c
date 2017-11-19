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
    int semid;

    if(((argc < 2) || (argc > 3)) || (0 == strcmp(argv[1], "--help")))
    {
        usageError(argv[0], NULL);
    }

    if(2 == argc) /* Create and initialize semaphore */
    {
        union semun arg;
        semid = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
        if(-1 == semid)
        {
            fprintf(stderr, "Invoke semget() failed, errno(%d), strerror(%s)\n",
                             errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        arg.val = atoi(argv[1]);
        if(-1 == semctl(semid, 0, SETVAL, arg))
        {
            fprintf(stderr, "Invoke semctl() failed, errno(%d), strerror(%s)\n",
                             errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        printf("Semaphore ID = %d\n", semid);
    }
    else /* Perform an operation on first semaphore */
    {
        struct sembuf sop;  /* Structure defining operation */

        semid = atoi(argv[1]);

        sop.sem_num = 0;            /* Specifies firet semaphore in set */
        sop.sem_op = atoi(argv[2]); /* Add, subtract, or wait for 0 */
        sop.sem_flg = 0;            /* No special options for operation */
        
        printf("%ld: about to semop\n", (long) getpid());
        if(-1 == semop(semid, &sop, 1))
        {
            fprintf(stderr, "Invoke semctl() failed, errno(%d), strerror(%s)\n",
                             errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        printf("%ld: semop completed\n", (long) getpid());
    }

    exit(EXIT_SUCCESS);
}
