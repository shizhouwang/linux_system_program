#include "../lib/tools.h"
#include <sys/wait.h>

#define BUF_SIZE 10

int main(int argc, char *argv[])
{
    int pfd[2];
    char buf[BUF_SIZE];
    int num;

    if(argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s string\n", argv[0]);

    if(pipe(pfd) == -1)
        errExit("pipe()");

    switch(fork())
    {
        case -1 : 
            errExit("fork()");

        case 0 : /* Child */
            if(close(pfd[1]) == -1)
                errExit("close()");
            
            for(;;) {
                num = read(pfd[0], buf, sizeof(buf));
                if(num == -1)
                    errExit("read()");
                else if(num == 0)
                    break;
                else
                    if(write(STDOUT_FILENO, buf, num) != num)
                        errExit("write()");
            }

            if(write(STDOUT_FILENO, "\n", 1) != 1)
                errExit("write()");
            
            if(close(pfd[0]) == -1)
                errExit("close");

            _exit(EXIT_SUCCESS);
        
        default : /* Parent */
            if(close(pfd[0]) == -1)
                errExit("close()");

            if(write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
                errExit("write()");

            if(close(pfd[1]) == -1)
                errExit("close()");
            
            wait(NULL);

            exit(EXIT_SUCCESS);
    }
}
