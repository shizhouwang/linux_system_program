#include "../lib/tools.h"
#include <sys/mman.h>

/* For open() */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* For wait() */
#include <sys/wait.h>

#ifdef USE_MAP_ANON
#define _BSD_SOURCE
#endif

int main(int argc, char *argv[])
{
    int *addr;
#ifdef USE_MAP_ANON
    addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
                MAP_SHARED | MAP_ANONYMOUS,  -1, 0);
    if(addr == MAP_FAILED)
        errExit("mmap()");
#else
    int fd;

    fd = open("/dev/zero", O_RDWR);
    if (fd == -1)
        errExit("open()");

    addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED)
        errExit("mmap()");

    if(close(fd) == -1)
        errExit("close()");
#endif
    *addr = 1;
    switch(fork()) {
        case -1 :
            errExit("fork()");
            break;

        case 0 : /* Child */
            (*addr)++;
            printf("[main()] >> In child, param=%d.\n", *addr);

            if(munmap(addr, sizeof(int)) == -1)
                errExit("munmap()");
            exit(EXIT_FAILURE);
            
            break;
        default : /* Parent */
            if(wait(NULL) == -1)
                errExit("wait()");
            
            printf("[main()] >> In parent, param=%d.\n", *addr);

            if(munmap(addr, sizeof(int)) == -1)
                errExit("munmap()");
            exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
}
