#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "semun.h"

void usageError(const char *prog, const char *msg)
{
    if(NULL == prog)
    {
        fprintf(stderr, "NULL pointer");
        exit(EXIT_FAILURE);
    }
    if(NULL != msg)
    {
        fprintf(stderr, "%s\n", msg);
    }

    fprintf(stderr, "Usage :\n"
                    "       %s [semid] [val] ...\n", prog);
    exit(EXIT_FAILURE);

}

void errExit(const char *msg)
{
    if(NULL == msg)
    {
        fprintf(stderr, "NULL pointer");
        exit(EXIT_FAILURE);
    }
    
    fprintf(stderr, "Invoke %s failed, errno(%d), strerror(%s)\n",
                    msg,
                    errno,
                    strerror(errno));

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    union semun arg;
    struct semid_ds ds;
    int semid;
    int index;

    if((argc < 3) || (0 == strcmp(argv[1], "--help")))
    {
        usageError(argv[0], NULL); 
    }

    semid = atoi(argv[1]);
    arg.buf = &ds;
    if(-1 == semctl(semid, 0, IPC_STAT, arg))
    {
        errExit("semctl");   
    }

    /* Compare input number of init value with sem_nsems */
    if(ds.sem_nsems != argc - 2)
    {
        fprintf(stderr, "Input param error, sem_nsems=%ld, input num=%d", arg.buf->sem_nsems, argc - 2);
        exit(EXIT_FAILURE);
    }
    
    arg.array = calloc(ds.sem_nsems, sizeof(arg.array[0]));
    if(NULL == arg.array)
    {
        errExit("calloc");
    }

    for(index = 0; index < ds.sem_nsems; index++)
    {
       arg.array[index] = atoi(argv[index + 2]);
    }

    if(-1 == semctl(semid, 0, SETALL, arg))
    {
        errExit("semctl");
    }
    
    printf("Exec SETALL on semid(%d) successfully\n", semid);

    exit(EXIT_SUCCESS); 
}
