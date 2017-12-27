#include "../lib/tools.h"


int main(int argc, char *argv[])
{
    printf("uid=%ld, gid=%ld, euid=%ld, egid=%ld.\n", 
            (long)getuid(), (long)getgid(), (long)geteuid(), (long)getegid());
    exit(EXIT_SUCCESS);
}
