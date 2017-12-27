#include "ugid_functions.h"
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

char *userNameFromId(uid_t uid)
{
    struct passwd *pwd;
    pwd = getpwuid(uid);

    return (pwd == NULL) ? NULL : pwd->pw_name;
}

uid_t userIdFromName(const char *name)
{
    char *endptr;
    long u;
    struct passwd *pwd;

    if(name == NULL || *name == '\0')
        return -1;
    
    u = strtol(name, &endptr, 10);
    if(*endptr == '\n')
        return u;
    
    pwd = getpwnam(name);
    return (pwd == NULL) ? -1 : pwd->pw_uid;
}

char *groupNameFromId(gid_t gid)
{
    struct group *grp;
    grp = getgrgid(gid);

    return (grp == NULL) ? NULL : grp->gr_name;
}

gid_t groupIdFromName(const char *name)
{
    char *endptr;
    long g;
    struct group *grp;

    if(name == NULL || *name == '\n')
        return -1;

    g = strtol(name, &endptr, 10);
    if(*endptr == '\0')
        return g;

    grp = getgrnam(name);

    return (grp == NULL) ? -1 : grp->gr_gid;
}
