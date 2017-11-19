#include "../svsem/semun.h"
#include "svshm_xfr.h"

int main(int argc, char *argv[])
{
    int semid, shmid, xfrs, bytes;
    struct shmseg *shmp;
    union semun dummy;

    semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
    if(semid == -1)
        errExit("semget()");

    if(initSemAvailible(semid, WRITE_SEM) == -1)
        errExit("initSemAvailible()");
    if(initSemInUse(semid, READ_SEM) == -1)
        errExit("initSemInUse()");
    
    shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);
    if(shmid == -1)
        errExit("shmget()");

    shmp = shmat(shmid, NULL, 0);
    if(shmp == (void *)-1)
        errExit("shmat()");

    for(xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->cnt) {
        if(reserveSem(semid, WRITE_SEM) == -1)  /* Give writer a turn */
            errExit("reserveSem()");
        else
            printf("[svshm_xfr_writer.c] >> reserveSem(semid, WRITE_SEM)\n");

        shmp->cnt = read(STDIN_FILENO, shmp->buf, sizeof(shmp->buf));
        if(shmp->cnt == -1)
            errExit("read");
        else
            printf("[svshm_xfr_writer.c] >> read %d bytes\n", shmp->cnt);

        if(releaseSem(semid, READ_SEM) == -1)    /* Give reader a turn */
            errExit("releaseSem()");
        else
            printf("[svshm_xfr_writer.c] >> releaseSem(semid, READ_SEM)\n");
            
        /* We think it's END-OF-FILE when read 0 byte */
        if(shmp->cnt == 0)
            break;
    }

    printf("[svshm_xfr_writer.c] >> goto out of the loop\n");
    
    if(reserveSem(semid, WRITE_SEM) == -1)
        errExit("reserveSem()");
    else
        printf("[svshm_xfr_writer.c] >> reserveSem(semid, WRITE_SEM)\n");

    if(semctl(semid, 0, IPC_RMID, dummy))
        errExit("semctl()");
    
    if(shmdt(shmp) == -1)
        errExit("shmdt()");
    if(shmctl(shmid, IPC_RMID, 0))
        errExit("shmctl()");

    fprintf(stdout, "Send %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
