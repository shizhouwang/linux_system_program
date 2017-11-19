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
    struct msginfo info = {0};
    int msqid = 0, maxind = 0, index = 0;
    
    maxind = msgctl(0, MSG_INFO, &info);
    if(-1 == maxind)
    {
        fprintf(stderr, "Invoke msgctl() fail, errno(%d), strerror(%s)\n",
                errno, strerror(errno));

        exit(EXIT_FAILURE);
    }

    printf("The  index  of the  highest  used entry is %d\n"
           "---------------------------------------------\n", maxind);

    printf("key       msqid     owner     perms     used-bytes     messages\n");
    
    for(index = 0; index <= maxind; index++)
    {
        msqid = msgctl(index, MSG_STAT, &ds);
        if(-1 == msqid)
        {
            continue;
        }
        printf("%-10lx%-10ld%-10ld%-10x%-15ld%-10ld\n",
                (unsigned long)ds.msg_perm.__key,
                (unsigned long)msqid,
                (unsigned long)ds.msg_perm.uid,
                (unsigned int)ds.msg_perm.mode,
                (unsigned long)ds.__msg_cbytes,
                (unsigned long)ds.msg_qnum);
    }

    exit(EXIT_SUCCESS);
}
