#define _BSD_SOURCE
#include "../lib/tools.h"

#include <sys/mman.h>
#include <sys/mman.h>

#define LEN (1024 * 1024)
#define CMD_FMT "cat /proc/%ld/maps | grep zero"
#define CMD_SIZE (sizeof(CMD_FMT) + 20)

int main(int argc, char *argv[])
{
    void *addr;
    char cmd[CMD_SIZE];

    addr  = mmap(NULL, LEN, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(addr == MAP_FAILED)
        errExit("mmap()");
    
    /* Debug */
    printf("Size of (CMD_FMT):%ld.\n", sizeof(CMD_FMT));

    printf("Before invoke mprotect().\n");
    snprintf(cmd, sizeof(cmd), CMD_FMT, (long int)getpid());
    system(cmd);

    if(mprotect(addr, LEN, PROT_READ | PROT_WRITE) == -1)
        errExit("mprotect");
    
    printf("After invoke mprotect().\n");
    system(cmd);

    exit(EXIT_SUCCESS);
}
