#ifndef SVSHM_XFR_H
#define SVSHM_XFR_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../svsem/binary_sems.h"
#include "../svsem/tools.h"


#define SHM_KEY 0x1234  /* Key for shared memory segment */
#define SEM_KEY 0x5678  /* Key for semophore set */

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
                        /* Permissions for our IPC objects */

#define WRITE_SEM 0 
#define READ_SEM 1

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

struct shmseg {         /* Defines structure of shared menory segment */
    int cnt;            /* Number of bytes used in 'buf' */
    char buf[BUF_SIZE]; /* Data being transferd */
};

#endif
