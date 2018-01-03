#define _GNU_SOURCE
#include "../lib/tools.h"
#include <sys/utsname.h>

int main(int argc, char *argv[])
{
    struct utsname uts;
    
    if(uname(&uts) == -1)
        errExit("uname()");

    printf("System name :       %s\n"
           "Node name :         %s\n"
           "System release :    %s\n"
           "System version :    %s\n"
           "Hardware :          %s\n",
           uts.sysname, uts.nodename,
           uts.release, uts.version,
           uts.machine);
#ifdef _GNU_SOURCE
    printf("Domain name :       %s\n", uts.domainname);
#endif

    exit(EXIT_SUCCESS);
}
