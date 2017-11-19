#ifndef __SVMSG_FILE_H__
#define __SVMSG_FILE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stddef.h> /* For offsetof() */

#define PATH_MAX   1024
#define SERVER_KEY 0x1aaaaaa1

struct requestMsg {
    long mtype;
    int clientId;
    char pathname[PATH_MAX];
};

#define REQ_MSG_SIZE (offsetof(struct requestMsg, pathname) - \
                      offsetof(struct requestMsg, clientId) + PATH_MAX)
#define RESP_MSG_SIZE 8192

struct responseMsg {
    long mtype;
    char data[RESP_MSG_SIZE];
};

/* Types for response messages sent from server to client */
#define RESP_MT_FAILURE 1   /* File couldn't be opened */
#define RESP_MT_DATA    2   /* Message contains file data */
#define RESP_MT_END     3   /* File data complete */

#endif  /* __SVMSG_FILE_H__ */
