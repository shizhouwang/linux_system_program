#include "svmsg_file.h"

#include <signal.h> /* For signal processing */
#include <unistd.h> /* For fork() */

#include <sys/types.h>
#include <sys/wait.h>

#include <sys/stat.h> /* For read() */
#include <fcntl.h>
/* SIGCHID handler */
void grimReaper(int signal)
{
    int savedErrno;

    savedErrno = errno; /* waitpid() might change 'errno' */
    while(waitpid(-1, NULL, WNOHANG) > 0)
    {
        continue;
    }

    errno = savedErrno;
}

void serveRequest(struct requestMsg *req)
{
    int fd = 0;
    struct responseMsg resp;
    ssize_t numRead = 0;

    memset(&resp, 0, sizeof(resp));

    if(-1 == (fd = open(req->pathname, O_RDONLY)))
    {
        fprintf(stderr, "Invoke open() fail, errno(%d), strerror(%s)\n",
                        errno, strerror(errno));

        /* Notify client */
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "Couldn't open %s", req->pathname);
        msgsnd(req->clientId, &resp, strlen(resp.data)+1, 0);
        exit(EXIT_FAILURE);
    }

    /* Transmit file contents in message with type RESP_MT_DATA.We don't
     * diagnose read() and msgsnd() errors since we can't notify client.
     */
    resp.mtype = RESP_MT_DATA;
    while((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0)
    {
        if(-1 == msgsnd(req->clientId, &resp, numRead, 0))
        {
            break;
        }
    }
    
    /* Send a message of type RESP_MT_END to signify end-of-file */
    resp.mtype = RESP_MT_END;
    msgsnd(req->clientId, &resp, 0, 0);
}

int main(int argc, char *argv[])
{
    int serverId = 0, msgLen = 0;
    struct sigaction act;
    struct requestMsg req;
    pid_t pid;

    /* Create server message queue */
    serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL | 
                                    S_IRUSR | S_IWGRP | S_IRGRP | S_IWGRP);
    //serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL | 0x0666);
    if(-1 == serverId)
    {
        fprintf(stderr, "Invoke msgget() fail, errno(%d), strerror(%s)\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Handle SIGCHLD */
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    act.sa_handler = grimReaper;
    if(-1 == sigaction(SIGCHLD, &act, NULL))
    {
        fprintf(stderr, "Invoke sigaction() fail, errno(%d), strerror(%s)\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        msgLen = msgrcv(serverId, (void *)&req, REQ_MSG_SIZE, 1, 0);
        if(-1 == msgLen)
        {
            if(errno == EINTR)  /* Interrupted by SIGCHID handler? */
            {
                continue;       /* ...then restart msgrcv() */
            }
            else                /* Some other error, terminate loop */
            {
                fprintf(stderr, "Invoke msgrcv() fail, errno(%d), strerror(%s)\n",
                        errno, strerror(errno));
                break;
            }
        }

        /* Create child process */
        pid = fork();
        if(-1 == pid)
        {
            fprintf(stderr, "Invoke fork() fail, errno(%d), strerror(%s)\n",
                    errno, strerror(errno));
            break;
        }
        
        if(0 == pid)
        {
            serveRequest(&req);
            _exit(EXIT_SUCCESS);
        }

        /* Parent loops to receive next client request */
    }

    /* If msgrcv() or fork() fails, remove server MQ and exit */
    if(-1 == msgctl(serverId, IPC_RMID, NULL))
    {
        fprintf(stderr, "Invoke msgctl() fail, errno(%d), strerror(%s)\n",
                errno, strerror(errno));
    }

    exit(EXIT_FAILURE);
}
