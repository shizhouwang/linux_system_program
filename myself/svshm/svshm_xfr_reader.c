#include "../svsem/semun.h"
#include "svshm_xfr.h"


int main(int argc, char *argv[])
{
    int semid, shmid, bytes, xfrs;
    struct shmseg *shmp;

    semid = semget(SEM_KEY, 0, 0);
    if(semid == -1)
        errExit("semget");
    
    shmid = shmget(SHM_KEY, 0, 0);
    if(shmid == -1)
        errExit("shmget");

    shmp = shmat(shmid, NULL, SHM_RDONLY);
    if(shmp == (void *)-1)
       errExit("shmat");

    for(bytes = 0, xfrs =0; ;xfrs++) {
        if(reserveSem(semid, READ_SEM) == -1)
            errExit("reserveSem");
        else
            printf("[svshm_xfr_reader.c] >> reserveSem(semid, READ_SEM)\n");

        if(shmp->cnt == 0)
            break;

        bytes += shmp->cnt;

        if(write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt)
            errExit("write");
        else
            printf("[svshm_xfr_reader.c] >> write %d bytes.\n", shmp->cnt);

        if(releaseSem(semid, WRITE_SEM) == -1)  /* Give write a turn */
            errExit("reserveSem");
        else
            printf("[svshm_xfr_reader.c] >> releaseSem(semid, WRITE_SEM)\n");
    }

    if(shmdt(shmp) == -1)
        errExit("shmdt");

    if(releaseSem(semid, WRITE_SEM) == -1)  /* Give write a turn */
        errExit("releaseSem");

    printf("Received %d bytes (%d xfrs)\n", bytes, xfrs);

    exit(EXIT_SUCCESS);
}
