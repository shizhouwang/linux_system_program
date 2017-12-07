#include "../lib/tools.h"
#include "curr_time.h"
#include <time.h>

#define BUF_SIZE 1024

char *currTime(const char *fmt)
{
    time_t t;
    struct tm *local;
    size_t ret;
    static char buf[BUF_SIZE];

    memset(buf, 0, BUF_SIZE);

    t = time(NULL);
    local = localtime(&t);
    if(local == NULL)
        errExit("localtime()");

    ret = strftime(buf, sizeof(buf), (fmt != NULL) ? fmt : "%c", local);

    return (ret != 0) ? buf : NULL;
}
