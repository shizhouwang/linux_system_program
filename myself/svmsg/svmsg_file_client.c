#include "svmsg_file.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <sys/stat.h>
#include <fcntl.h>

static int clientId;

static void usageErr(const char *prog, const char *msg)
{
    if(NULL != msg)
    {
        fprintf(stderr, "%s\n", msg);
    }
    fprintf(stderr, "Usage: %s [pathname]\n", prog);

    exit(EXIT_FAILURE);
}

void removeQueue(void)
{
    if(-1 == msgctl(clientId, IPC_RMID, NULL))
    {
        fprintf(stderr, "Invoke msgctl() fail, errno(%d), strerror(%s)\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    struct requestMsg req;
    struct responseMsg resp;
    ssize_t msgLen = 0, recvBytes = 0;
    int serverId = 0;
    int numMsgs  = 0;

    if((argc != 2) || (argv[1] == "--help"))
    {
        usageErr(argv[0], NULL);
    }
    
    if(strlen(argv[1]) > sizeof(req.pathname) - 1)
    {
        fprintf(stderr, "pathname too long (max: %ld bytes)\n",
                        (long) sizeof(req.pathname) - 1);
    }

    /* Get server's queue identifier; create queue for response */
    if(-1 == (serverId = msgget(SERVER_KEY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)))
    {
        fprintf(stderr, "Invoke msgget() fail, errno(%d), strerror(%s)\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    /* Debug print */
    printf("POS1\n");

    if(-1 == (clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP)))
    {
        fprintf(stderr, "Invoke msgget() fail, errno(%d), strerror(%s)\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
 
    /* Debug print */
    printf("POS2\n");

    if(0 != atexit(removeQueue))
    {
        fprintf(stderr, "Invoke atexit() fail\n");
        exit(EXIT_FAILURE);
    }

    /* Send message asking for file named in argv[1] */
    req.mtype = 1;
    req.clientId = clientId;
    strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
    req.pathname[sizeof(req.pathname) - 1] = '\0';  /* Ensure string is terminated */

    if(-1 == msgsnd(serverId, &req, REQ_MSG_SIZE, 0))
    {
        fprintf(stderr, "Invoke msgsnd() fail, errno(%d), strerror(%s)\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Get first response, which may be failure notification */
    msgLen =  msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
    if(-1 == msgLen)
    {
        fprintf(stderr, "Invoke msgrcv() fail, errno(%d), strerror(%s)\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(RESP_MT_FAILURE == resp.mtype)
    {
        printf("%s\n", resp.data);
        removeQueue();
        exit(EXIT_FAILURE);
    }

    /* File was opened successfully by server; process messgaes
     * (including the one already received) containing file data
     */
    recvBytes = msgLen;
    for(numMsgs = 1; RESP_MT_DATA == resp.mtype; numMsgs++)
    {
        msgLen =  msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
        if(-1 == msgLen)
        {
            fprintf(stderr, "Invoke msgrcv() fail, errno(%d), strerror(%s)\n",
                            errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        recvBytes += msgLen;
    }

    printf("Received %ld bytes (%d messages)\n", (long)recvBytes, numMsgs);

    exit(EXIT_SUCCESS);
}
