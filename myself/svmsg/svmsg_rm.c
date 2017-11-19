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

    fprintf(stderr, "Usage: %s [msqid ...]\n", progName);

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int index = 0;

    if((argc > 1) && (0 == strcmp(argv[1], "--help")))
    {
        usageError(argv[0], NULL);
    }

    for(index = 1; index < argc; index++)
    {
        if(-1 == msgctl(atoi(argv[index]), IPC_RMID, NULL))
        {
            fprintf(stderr, "Invoke msgctl() fail, errno(%d), strerror(%s)\n",
                            errno, strerror(errno));

            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
