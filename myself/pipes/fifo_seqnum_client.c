#include "fifo_seqnum.h"
#include <sys/stat.h>
#include <fcntl.h>

static char clientFifo[CLIENT_FIFO_NAME_LEN];

static void removeFifo(void)
{
    unlink(clientFifo);
}

int main(int argc, char *argv[])
{
    int serverFd, clientFd;
    struct request req;
    struct response resp;

    if(argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [request-len]\n", argv[0]);

    umask(0);

    snprintf(clientFifo, sizeof(clientFifo), CLIENT_FIFO_TEMPLATE, (long)getpid());
    if(mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
        errExit("mkfifo()");
    
    if(atexit(removeFifo) !=0)
        errExit("atexit()");

    req.pid = (long)getpid();
    req.seqLen = (argc > 1) ? strlen(argv[1]) : 1;

    clientFd = open(clientFifo, O_WRONLY);
    if(clientFd == -1)
        errExit("open()");

    if(write(clientFd, &req, sizeof(req)) != sizeof(req))
        errExit("write()");

    serverFd = open(SERVER_FIFO, O_RDONLY);
    if(serverFd == -1)
        errExit("open()");

    if(read(serverFd, &resp, sizeof(resp)) != sizeof(resp))
        errExit("resp()");

    printf("read resonse from server\n");

    exit(EXIT_SUCCESS);
}
