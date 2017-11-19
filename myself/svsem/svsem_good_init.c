#include "tools.h"
#include "semun.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <sys/stat.h>
#define PERMS (S_IRWXU | S_IRUSR)
#define KEY 100

int main(int argc, char *argv[])
{
    int semid;
    union semun arg;

    semid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | PERMS);
    if(semid != -1)
    {
        struct sembuf buf;

        arg.val = 0;
        if(-1 == semctl(semid, 0, SETVAL, arg))
        {
            errExit("semctl()");
        }
        
        sleep(15);
        /* Perform a "no-op" semaphore operation - changes sem_otime
         * so other processes can see we're initialized the set
         */
        buf.sem_num = 0;
        buf.sem_op = 0;
        buf.sem_flg = 0;

        if(-1 == semop(semid, &buf, 1))
        {
            errExit("semop()");
        }
    }
    else
    {
        struct semid_ds ds;
        int i;
        const int MAX_TRIES = 10;

        semid = semget(KEY, 1, PERMS);
        if(-1 == semid)
        {
            errExit("semget()");
        }

        arg.buf = &ds;
        for(i = 0; i < 10; i++)
        {
            if(-1 == semctl(semid, 0, IPC_STAT, arg))
            {
                errExit("semctl");
            }
            
            if(0 != ds.sem_otime)
            {
                break;
            }
            sleep(1);
        }

        if(0 == ds.sem_otime)
        {
            fprintf(stderr, "Existing semaphore not initialized\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Debug print */
    fprintf(stdout, "Existing semaphore initialized\n");

    exit(EXIT_SUCCESS);
}
