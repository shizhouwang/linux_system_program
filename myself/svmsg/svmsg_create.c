#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void usageError(const char *progName, const char *msg)
{
    if(NULL != msg)
    {
        fprintf(stderr, "%s\n", msg);
    }

    fprintf(stderr, "Usage: %s [-cx] {-f pathname | -k key | -p}"
                            "[octal-perms]\n", progName);

    fprintf(stderr, "   -c          Use IPC_CREAT flag\n");
    fprintf(stderr, "   -x          Use IPC_EXCL flag\n");
    fprintf(stderr, "   -f pathname Generate key using ftok()\n");
    fprintf(stderr, "   -k key      Use 'key' as key\n");
    fprintf(stderr, "   -p          Use IPC_PRIVATE key\n");

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    key_t key;
    int msqid, opt, flags;
    unsigned int perms;
    int numFlag = 0;

    while(-1 != (opt = getopt(argc, argv, "cxf:k:p")))
    {
        switch(opt)
        {
            case 'c':
                flags |= IPC_CREAT;
                /* Test */
                printf("argc(%d), optind(%d)\n", argc, optind);
                break;

            case 'x':
                flags |= IPC_EXCL;
                /* Test */
                printf("argc(%d), optind(%d)\n", argc, optind);
                break;

            case 'f':
                if(-1 == (key = ftok(optarg, 1)))
                {
                    fprintf(stderr, "ftok() error, errno(%d), strerror(%s)\n",
                                    errno, strerror(errno));
                }
                numFlag++;
                /* Test */
                printf("argc(%d), optind(%d)\n", argc, optind);
                break;

            case 'k':
                key = atoi(optarg);
                numFlag++;
                /* Test */
                printf("argc(%d), optind(%d)\n", argc, optind);
                break;

            case 'p':
                key = IPC_PRIVATE;
                numFlag++;
                /* Test */
                printf("argc(%d), optind(%d)\n", argc, optind);
                break;

            default:
                usageError(argv[0], "Bad option\n");
        }
    }
    
    if(1 != numFlag)
    {
        fprintf(stderr, "Exactly only one of -f/-k/-p should be supplied!\n");
        usageError(argv[0], NULL);
    }

    perms = (optind == argc) ? (S_IRUSR | S_IWUSR) : atoi(argv[optind]);

    msqid =  msgget(key, flags | perms);
    if(-1 == msqid)
    {
        fprintf(stderr, "msgget() fail, errno(%d), strerror(%s)\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("msqid:%d\n", msqid);
    exit(EXIT_SUCCESS);
}
