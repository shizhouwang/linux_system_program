#ifndef SEMUN_H
#define SEMUN_H

#include <sys/types.h>
#include <sys/sem.h>

/* Used in calls to semctl() */
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
#if defined(__linux__)
    struct seminfo *__buf;
#endif
};


#endif
