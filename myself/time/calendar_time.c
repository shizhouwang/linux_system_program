#include "../lib/tools.h"

#include <time.h>
#include <sys/time.h>

#define SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 *60)

int main(int argc, char *argv[])
{
    time_t t;
    struct timeval tv;
    struct tm *pgm, *plocal;
    struct tm gm, local;

    t = time(NULL);
    printf("Seconds since the Epoch (1 Jan 1970): %ld"
            " about (%6.3f) years\n", (long)t, t / SECONDS_IN_TROPICAL_YEAR);

    if(gettimeofday(&tv, NULL) == -1)
        errExit("gettimeofday()");
    printf("Function gettimeofday() returned %ld secs, %ld microsecs.\n", (long)tv.tv_sec, (long)tv.tv_usec);

    pgm = gmtime(&t);
    if(pgm == NULL)
        errExit("gmtime()");

    gm = *pgm;  /* Save local copy, since *pgm may be modifed by acstime() or gmtime() */
    printf("Broken down by gmtime:\n");
    printf("year=%d, month=%d, day=%d, hour=%d, min=%d, sec=%d\n"
           "day of the week=%d, day of the year=%d, dst=%d.\n",
            gm.tm_year, gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec,
            gm.tm_wday, gm.tm_yday, gm.tm_isdst);
    
    plocal = localtime(&t);
    if(plocal == NULL)
        errExit("localtime()");

    local = *plocal;
    printf("Broken down by localtime:\n");
    printf("year=%d, month=%d, day=%d, hour=%d, min=%d, sec=%d\n"
           "day of the week=%d, day of the year=%d, dst=%d.\n",
            local.tm_year, local.tm_mon, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec,
            local.tm_wday, local.tm_yday, local.tm_isdst);
    
    printf("ctime() of time() value:%s", ctime(&t));
    printf("asctime() of localtime() value:%s", asctime(&local));

    printf("mktime() of gmtime() value:%ld.\n", (long)mktime(&gm));
    printf("mktime() of localtime() value:%ld.\n", (long)mktime(&local));
    
    exit(EXIT_SUCCESS);
}
