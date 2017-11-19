#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../lib/tools.h"

#define MEM_SIZE 10

int main(int argc, char *argv[])
{
    int fd;
    char *addr;

    if((argc < 2) || (strcmp("--help", argv[1]) == 0))
        usageErr("%s file [new-value]\n", argv[0]);

    fd = open(argv[1], O_RDWR);
    if(fd == -1)
        errExit("open()");
    
    addr = (char *)mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if((void *)addr == MAP_FAILED)
        errExit("mmap()");

    if(close(fd) == -1)
        errExit("close()");
    fd = -1;

    printf("Current string=%.*s\n", MEM_SIZE, addr);

    if(argc > 2) {
        if(strlen(argv[2]) >= MEM_SIZE)
            cmdLineErr("'new-value' too large\n");

        memset((void *)addr, 0, MEM_SIZE);
        strncpy(addr, argv[2], MEM_SIZE - 1);

        if(msync((void *)addr, MEM_SIZE, MS_SYNC) == -1)
            errExit("msync()");

        printf("Copied \"%s\" to shared memory\n", argv[2]);

    }

    exit(EXIT_SUCCESS);
}
