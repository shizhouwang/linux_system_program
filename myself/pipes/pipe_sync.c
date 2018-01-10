#include "tools.h"
#include "curr_time.h"

int main(int argc, char *argv[])
{
    int pfd[2];
    int i;
    char dummy;

    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sleep-time [sleep-time ...]", argv[0]);

    setbuf(stdout, NULL);
    
    printf("%s parent started\n", currTime("%T"));

    if(pipe(pfd) == -1)
        errExit("pipe()");

    for(i = 1; i < argc; i++) {
        switch(fork()) {
            case -1 : 
                errExit("fork()");

            case 0 : /* Child */
                if(close(pfd[0]) == -1)
                    errExit("close()");
                
                /* Child do some work here */

                sleep(atoi(argv[i]));

                printf("%s Child %d (PID=%ld) closing pipe\n",
                        currTime("%T"), i, (long)getpid());

                if(close(pfd[1]) == -1)
                    errExit("close()");
                
                _exit(EXIT_SUCCESS);

            default : /* Parent */
                break;
        }
    }

    /* Close the write port so we can read only from the children */
    if(close(pfd[1]) == -1)
        errExit("close()");

    if(read(pfd[0], &dummy, 1) != 0)
        errExit("read()");
    
    printf("%s Parent ready to go\n", currTime("%T"));

    exit(EXIT_SUCCESS);
}

