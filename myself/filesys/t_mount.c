#include "../lib/tools.h"
#include <sys/mount.h>

static void usageError(const char *progName, const char *msg)
{
    if(msg != NULL)
        fprintf(stderr, "%s\n", msg);

    fprintf(stderr, "Usage: %s [option] source target\n\n", progName);
    fprintf(stderr, "Available options:\n");
#define fpe(str) fprintf(stderr, "     "str)   /* Shorter! */
    fpe("-t fstype [e.g., 'ext2' or 'reiserfs']\n");
    fpe("-o date    [file system-dependent options,\n");
    fpe("           e.g., 'bsdgroup' for ext2]\n");
    fpe("-f mountflags can include any of :\n");
#define fpe2(str) fprintf(stderr, "        "str)
    fpe2("b - MS_BIND   create a bind mount\n");
    fpe2("d - MS_DIRSYNC synchronous directory updates\n");

    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    char *ftype;
    unsigned long flags;
    void *data;
    int i, opt;

    if(argc < 3)
        usageError(argv[0], NULL);

    while((opt = getopt(argc, argv, "t:o:f:")) != -1) {
        switch(opt) {
            case 'o':
                data = optarg;
                break;

            case 't':
                ftype = optarg;
                break;

            case 'f':
                for(i = 0; i < strlen(optarg); i++) {
                    switch(optarg[i]) {
                        case 'b': flags |= MS_BIND; break;
                        case 'd': flags |= MS_DIRSYNC; break;
                        case 'l': flags |= MS_MANDLOCK; break;
                        case 'm': flags |= MS_MOVE; break;
                        case 'A': flags |= MS_NOATIME; break;
                        case 'V': flags |= MS_NODEV; break;
                        case 'D': flags |= MS_NODIRATIME; break;
                        case 'E': flags |= MS_NOEXEC; break;
                        case 'S': flags |= MS_NOSUID; break;
                        case 'r': flags |= MS_RDONLY; break;
                        case 'c': flags |= MS_REC; break;
                        case 'R': flags |= MS_REMOUNT; break;
                        case 's': flags |= MS_SYNCHRONOUS; break;
                        default: usageError(argv[0], NULL);
                    }
                }
                break;

            default: usageError(argv[0], NULL);
        }
    }
    
    if(argc != optind + 2)
        usageError(argv[0], NULL);
    
    if(mount(argv[optind], argv[optind + 1], ftype , flags, data) == -1)
        errExit("mount");


    exit(EXIT_SUCCESS);
}
