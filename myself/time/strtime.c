#include "../lib/tools.h"
#define _XOPEN_SOURCE
#include <time.h>

int main(int argc, char *argv[])
{
    char *pret;
    struct tm tm;
    char *ofmt;
    char buf[128] = {0};

    if(argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s input-time input-fmt [ output-fmt ]\n", argv[0]);
    
    memset(&tm, sizeof(struct tm), 0);
    if(strptime(argv[1], argv[2], &tm) == NULL)
        errExit("strptime()");
    
    tm.tm_isdst = -1;   /* The daylight saving time is not available */
    
    printf("Calander time : %ld\n", (long)mktime(&tm));

    ofmt = argc > 3 ? argv[3] : "%H:%M:%S %A, %d %B %Y %Z";
    if(strftime(buf, sizeof(buf), ofmt, &tm) == 0)
        cmdLineErr("strftime() : the output buf is too small.\n");

    printf("strftime() output : %s\n", buf);

    exit(EXIT_SUCCESS);
}
