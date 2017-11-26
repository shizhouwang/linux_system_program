/* For mincore() and MAP_ANONYMOUS */
#define _BSD_SOURCE
#include "../lib/tools.h"
#include <sys/mman.h>

static void displayMincore(char *addr, size_t length)
{
    long int pageSize, i;
    size_t numPages;
    unsigned char *vec;

    if( addr == NULL || length < 1)
        cmdLineErr("Input param error.");

    pageSize = sysconf(_SC_PAGESIZE);
    if(pageSize == -1)
        errExit("sysconf()");

    numPages = (length + pageSize - 1) / pageSize;
    vec = (unsigned char *)malloc(numPages);
    if(vec == NULL)
        errExit("malloc()");

    if(mincore(addr, length, vec) == -1)
        errExit("mincore()");
    
    for(i = 0; i < numPages; i++) {
        if(i % 64 == 0)
            printf("%s%10p: ", (i == 0) ? "" : "\n", addr + i * pageSize);

        printf("%c", (vec[i] & 1) ? '*' : '.');
    }
    printf("\n");

    free(vec);
}

int main(int argc, char *argv[])
{
    void *addr;
    size_t len, lockLen, i;
    long int pageSize, stepSize;

    if(argc != 4 || strcmp("--help", argv[1]) == 0)
        usageErr("%s num-pages lock-page-step lock-page-len\n", argv[0]);

    /*
     * After test, we can use _SC_PAGESIZE or _SC_PAGE_SIZE
     */
    pageSize = sysconf(_SC_PAGESIZE);
    if(pageSize == -1)
        errExit("sysconf");
    
    /* Debug print */
    debug("pageSize=%ld\n", pageSize);

    len = atoi(argv[1]) * pageSize;
    stepSize = atoi(argv[2]) * pageSize;
    lockLen = atoi(argv[3]) * pageSize;

    addr = mmap(NULL, len, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(addr == MAP_FAILED)
        errExit("mmap");

    printf("Allocate %ld (%#lx) bytes starting at %p.\n", (long)len, (unsigned long)len, addr);

    printf("Before mlock().\n");
    displayMincore(addr, len);

    for(i = 0; i + lockLen <= len; i += stepSize) {
        if(mlock(addr + i, lockLen) == -1)
            errExit("mlock()");
    }

    printf("After mlock().\n");
    displayMincore(addr, len);
    
    exit(EXIT_SUCCESS);
}
