
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "semun.h"

static void usageError(const char *progName, const char *msg)
{
    if(NULL == progName)
    {
        fprintf(stderr, "NULL pointer\n");
        exit(EXIT_FAILURE);
    }
    
    if(NULL != msg)
    {
        fprintf(stderr, "%s\n", msg);
    }
    
    fprintf(stderr, "Usage : %s [semid]\n", progName);

    exit(EXIT_FAILURE);
}

static void errExit(const char *funcName)
{
    if(NULL == funcName)
    {
        fprintf(stderr, "NULL pointer");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Invoke %s failed, errno(%d), stderror(%s)\n",
                    funcName, errno, strerror(errno));
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int semid;
    union semun arg;
    struct semid_ds ds, dummy;
    int i, j;

    if((argc != 2) || (0 == strcmp(argv[1], "--help")))
    {
        usageError(argv[0], NULL); 
    }
    
    semid = atoi(argv[1]);
    arg.buf = &ds;
    
    if(-1 == semctl(semid, 0, IPC_STAT, arg))
    {
        errExit("semctl(IPC_STAT)");
    }
    
    printf("Last semaphore change time: %s\n", ctime(&ds.sem_ctime));
    printf("Last semaphore operation time: %s\n", ctime(&ds.sem_otime));

    arg.array = calloc(ds.sem_nsems, sizeof(arg.array[0]));
    if(NULL == arg.array)
    {
        errExit("calloc");
    }
    
    if(-1 == semctl(semid, 0, GETALL, arg))
    {
        errExit("semctl(IPC_STAT)");
    }

    printf("Sem # Value SETPID SETNCNT SETZCNT\n");
    for(i = 0; i < ds.sem_nsems; i++)
    {
        printf("%3d  %5d  %5d  %5d  %5d\n",
                i,
                arg.array[i],
                semctl(semid, i, GETPID, dummy),
                semctl(semid, i, GETNCNT, dummy),
                semctl(semid, i, GETZCNT, dummy));
    }
    exit(EXIT_SUCCESS);
}
