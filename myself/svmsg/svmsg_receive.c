#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define _GNU_SOURCE     /* For MSG_EXCEPT decleration */
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>     /* getopt() */
#include <sys/msg.h>    /* IPC_NOWAIT */


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

    fprintf(stderr, "Usage: %s [options] msqid [max-bytes]\n", progName);
    fprintf(stderr, "Permitted options are:\n");
    fprintf(stderr, "   -n          Use IPC_NOWAIT flag\n");
    fprintf(stderr, "   -e          Use MSG_NOERROR flag\n");
#ifdef MSG_EXCEPT
    fprintf(stderr, "   -x          Use MSG_EXCEPT flag\n");
#endif
    fprintf(stderr, "   -t type     Select message of given type\n");

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int flags = 0, opt = 0, msqid = 0;
    int strLen = 0, maxLen = 0;
    long type = 0, msgLen = 0;
    struct mbuf mbuf;

    memset(&mbuf, 0, sizeof(mbuf));

    /* Parse command-line options and arguments */
    while(-1 != (opt = getopt(argc, argv, "next:")))
    {
        switch(opt)
        {
            case 'n':   flags |= IPC_NOWAIT;    break;
            case 'e':   flags |= MSG_NOERROR;   break;
#ifdef MSG_EXCEPT
            case 'x':   flags |= MSG_EXCEPT;    break;
#endif
            case 't':   type = atoi(optarg);    break;

            default:
                usageError(argv[0], NULL);
        }

    }

    if((argc < optind + 1) || (argc > optind + 2))
    {
        usageError(argv[0], "Wrong number of arguments!");
    }
    
    msqid = atoi(argv[optind]);
    
    if(argc == optind + 2)
    {
        maxLen = atoi(argv[optind + 1]);
        maxLen = (maxLen < MAX_MTEXT) ? maxLen : MAX_MTEXT;
    }
    else
    {
        maxLen = MAX_MTEXT;    
    }

    /* Get message and display on stdout */
    msgLen = msgrcv(msqid, (void *)&mbuf, maxLen, type, flags);
    if(-1 == msgLen)
    {
        fprintf(stderr, "Invoke msgrcv() fail, errno(%d), strerror(%s)\n",
                            errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Received: type=%ld; length=%ld; body=%s\n",
                        mbuf.mtype, msgLen, mbuf.mtext);

    exit(EXIT_SUCCESS);
}
