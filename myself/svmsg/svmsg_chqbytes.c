#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <errno.h>
#include <string.h>

static void usageError(const char *progName, const char *msg)
{
    if(NULL != msg)
    {
        fprintf(stderr, "%s\n", msg);
    }

    fprintf(stderr, "Usage: %s [msqid] [msg_qbytes]\n", progName);

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    struct msqid_ds ds = {0};
    int msqid = 0;

    if((3 != argc) && (0 == strcmp(argv[1], "--help")))
    {
        usageError(argv[0], NULL);
    }
    
    msqid = atoi(argv[1]);

    /* Retrieve copy of msqid_ds structure from kernel */
    if(-1 == msgctl(msqid, IPC_STAT, &ds))
    {
        fprintf(stderr, "Invoke msgctl() fail, errno(%d), strerror(%s)\n",
                errno, strerror(errno));

        exit(EXIT_FAILURE);
    }

    ds.msg_qbytes = atol(argv[2]);

    /* Update associated data structure in kernel */
    if(-1 == msgctl(msqid, IPC_SET, &ds))
    {
        fprintf(stderr, "Invoke msgctl() fail, errno(%d), strerror(%s)\n",
                errno, strerror(errno));

        exit(EXIT_FAILURE);
    }

    /* Retrieve the data structure again, make sure IPC_SET is working */
    memset(&ds, 0, sizeof(ds));
    if(-1 == msgctl(msqid, IPC_STAT, &ds))
    {
        fprintf(stderr, "Invoke msgctl() fail, errno(%d), strerror(%s)\n",
                errno, strerror(errno));

        exit(EXIT_FAILURE);
    }

    printf("After IPC_SET, msg_qbytes=%ld\n", (long)ds.msg_qbytes);

    exit(EXIT_SUCCESS);
}
