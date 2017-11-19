#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

typedef enum { FALSE, TRUE } Boolean;

static void errExit(const char *msg)
{
    if(NULL == msg)
    {   
        fprintf(stderr, "NULL pointer");
        exit(EXIT_FAILURE);
    }   

    fprintf(stderr, "Invoke %s failed, errno(%d), strerror(%s)\n",
            msg,
            errno,
            strerror(errno));

    exit(EXIT_FAILURE);
}

static void cmdLineErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);     /* Flush any pending stdout */

    fprintf(stderr, "Command-line usage error: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);
    
    fflush(stderr);     /* In case stderr is not line-buffered */
    exit(EXIT_FAILURE);
}

static void usageErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);     /* Flush any pending stdout */

    fprintf(stderr, "Usage: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);
    
    fflush(stderr);     /* In case stderr is not line-buffered */
    exit(EXIT_FAILURE);
}

static int isDigit(const unsigned char *str)
{
    char *endptr = NULL;

    if(NULL == str)
    {   
        fprintf(stderr, "NULL pointer");
        exit(EXIT_FAILURE);
    }   
    
    strtol(str, &endptr, 10);
    if((*str != '\0') && (*endptr == '\0'))
    {
        return 1;
    }

    return 0;
}
#endif  //TOOLS_H
