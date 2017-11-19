#include <sys/mman.h>

/* For open() */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../lib/tools.h"


int main(int argc, char *argv[])
{
    int fd;
    struct stat sb;
    void *addr;

    if((argc != 2) || (strcmp("--help", argv[1]) == 0))
        usageErr("%s file\n", argv[0]);

    fd = open(argv[1], O_RDONLY);
    if(fd == -1)
        errExit("open()");

    /* Obtain the size of file and use it to specify the size
     * of mapping and the size of buf to be written */
    if(fstat(fd, &sb) == -1)
        errExit("fstat()");
    
    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(addr == MAP_FAILED)
        errExit("mmap()");

    if(write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size)
        errExit("write()");

    exit(EXIT_SUCCESS);
}
