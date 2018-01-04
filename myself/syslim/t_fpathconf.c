#include "../lib/tools.h"


void static fpathconfPrint(const char *msg, int fd, int name)
{
    int long lim;
    
    if(msg == NULL)
        cmdLineErr("Error param\n");

    errno = 0;
    lim = fpathconf(fd, name);
    if(lim != -1) {
        printf("%s : %ld\n", msg, lim);
    } else {
        if(errno == 0)
            printf("%s (indeterminate)\n", msg);
        else
            errExit("fpathconf");
    }
}

int main(int argc, char *argv[])
{
    fpathconfPrint("_PC_NAME_MAX", STDIN_FILENO, _PC_NAME_MAX);
    fpathconfPrint("_PC_PATH_MAX", STDIN_FILENO, _PC_PATH_MAX);
    fpathconfPrint("_PC_PIPE_BUF", STDIN_FILENO, _PC_PIPE_BUF);

    exit(EXIT_SUCCESS);
}
