#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "fifo_seqnum.h"

int main(int argc, char *argv[])
{
    char clientFifo[CLIENT_FIFO_NAME_LEN];
    struct request req;
    struct response resp;
    int serverFd, clientFd;
    int seqNum = 0;

    umask(0);

    if(mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
        errExit("mkfifo()");

    /* Open the read descriptor and wait client to write */
    serverFd = open(SERVER_FIFO, O_RDONLY);
    if(serverFd == -1)
        errExit("open()");

    /* Open an extra write descriptor, so that we never see EOF */
    if(open(SERVER_FIFO, O_WRONLY) == -1)
        errExit("open()");
    
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        errExit("signal()");

    for(;;) {
        if(read(serverFd, &req, sizeof(req)) != sizeof(req)) {
            fprintf(stderr, "read %s err\n", SERVER_FIFO);
            continue;
        }
        
        printf("recv client(%ld) request\n", (long)req.pid);

        snprintf(clientFifo, sizeof(clientFifo), CLIENT_FIFO_TEMPLATE, (long)req.pid);

        clientFd = open(clientFifo, O_WRONLY);
        if(clientFd == -1){
            fprintf(stderr, "open client pipe to write fail (pipe name:%s), ignore it\n", clientFifo);
            continue;
        }
        
        resp.seqNum = seqNum;
        if(write(clientFd, &resp, sizeof(resp)) != sizeof(resp))
            errExit("write()");
        if(close(clientFd) == -1)
            errExit("close(()");

        seqNum += req.seqLen;
    }

    exit(EXIT_SUCCESS);
}
