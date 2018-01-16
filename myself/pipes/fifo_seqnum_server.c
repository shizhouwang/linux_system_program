#include <sys/stat.h>
#include <fcntl.h>
#include "fifo_seqnum.h"

int main(int argc, char *argv[])
{
    char clientFifoName[CLIENT_FIFO_NAME_LEN];
    struct request req;
    struct response resp;
    int fifoFd, clientFifoFd, seqNum;

    umask(0);

    if(mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
        errExit("mkfifo()");

    /* Open the read point and wait client to write */
    fifoFd = open(SERVER_FIFO, O_RDONLY);
    if(fifoFd == -1)
        errExit("open()");

    /* Open the write point so we will not see EOF */
    if(open(SERVER_FIFO, O_WRONLY) == -1)
        errExit("open()");
    
    for(;;) {
        if(read(fifoFd, &req, sizeof(req)) != sizeof(req)) {
            fprintf(stderr, "read %s err\n", SERVER_FIFO);
            continue;
        }
        
        printf("recv client(%ld) request\n", (long)req.pid);

        snprintf(clientFifoName, sizeof(clientFifoName), CLIENT_FIFO_TEMPLATE, (long)req.pid);

        clientFifoFd = open(clientFifoName, O_WRONLY);
        if(clientFifoFd == -1){
            fprintf(stderr, "open client pipe to write fail (pipe name:%s), ignore it\n", clientFifoName);
            continue;
        }
        
        resp.seqNum = seqNum;
        if(write(clientFifoFd, &resp, sizeof(resp)) != sizeof(resp))
            errExit("write()");
    }

    exit(EXIT_SUCCESS);
}
