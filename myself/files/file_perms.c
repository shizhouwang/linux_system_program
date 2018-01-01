#include "../lib/tools.h"
#include "file_perms.h"
#include "stdio.h"
#include <sys/stat.h>

#define STR_SIZE sizeof("rwxrwxrwx")

char *filePermStr(mode_t perm, int flags)
{
    static char str[STR_SIZE];
    snprintf(str, sizeof(str), "%c%c%c%c%c%c%c%c%c",
                (perm & S_IRUSR) ? 'r' : '-', (perm & S_IWUSR) ? 'w' : '-',
                (perm & S_IXUSR) ?
                    (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's' : 'x') : 
                    (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S' : '-'),

                (perm & S_IRGRP) ? 'r' : '-', (perm & S_IWGRP) ? 'w' : '-',
                (perm & S_IXGRP) ?
                    (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's' : 'x') : 
                    (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S' : '-'),

                (perm & S_IROTH) ? 'r' : '-', (perm & S_IWOTH) ? 'w' : '-',
                (perm & S_IXOTH) ?
                    (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 't' : 'x') : 
                    (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T' : '-'));

    return str;
}
