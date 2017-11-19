#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>    /* IPC_NOWAIT */
#include <unistd.h>     /* getopt() */


#define MAX_MTEXT 1024

struct mbuf
{
    long mtype;             /* Message type */
    char mtext[MAX_MTEXT];  /* Message body */
};


static void usageError(const char *progName, const char *msg)
{
    if(NULL != msg)
    {
        fprintf(stderr, "%s\n", msg);
    }

    fprintf(stderr, "Usage: %s [-n] msgid mtype [mtext]\n", progName);
    fprintf(stderr, "   -n          Use IPC_NOWAIT flag\n");

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int flag = 0, opt = 0, msqid = 0;
    int strLen = 0;
    struct mbuf mbuf;

    memset(&mbuf, 0, sizeof(mbuf));

    /* Parse command-line options and arguments */
    while(-1 != (opt = getopt(argc, argv, "n")))
    {
         if('n' == opt)
         {
            flag |= IPC_NOWAIT;
         }
         else
         {
            usageError(argv[0], "Bad option!");
         }
    }

    if((argc < optind + 2) || (argc > optind + 3))
    {
        usageError(argv[0], "Wrong number of arguments!");
    }

    msqid = atoi(argv[optind]);
    mbuf.mtype = atoi(argv[optind + 1]);
    
    if(argc == optind + 3)
    {
        strLen = strlen(argv[optind + 2]);
        if(strLen < sizeof(mbuf.mtext))
        {
            memcpy(mbuf.mtext, argv[optind + 2], strLen);
        }
        else
        {
            fprintf(stderr, "mtext too long (max %ld characters)!\n", sizeof(mbuf.mtext));
        }
    
    }
    else
    {
        strLen = 0;
    }
    
    if(-1 == msgsnd(msqid, (const void *)&mbuf, strLen + 1, flag))  /* strLen + 1: add end '\0' */
    {
        fprintf(stderr, "Invoke msgsend() fail, errno(%d), strerror(%s)\n",
                            errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
