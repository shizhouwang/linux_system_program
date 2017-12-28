#include "../lib/ugid_functions.h"


int main(int argc, char *argv[])
{
    uid_t uid;
    gid_t gid;
    Boolean errEnd;
    int i;

    if(argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("Usage :\n %s uid gid [file ...]\n"
                 "          owner or group can be '-', "
                 "meaning leave unchanged\n", argv[0]);

    if(strcmp(argv[1], "-") == 0) {
        uid = -1;  
    } else {
        uid = userIdFromName(argv[1]);
        if(uid == -1)
            cmdLineErr("No such user (%s)\n", argv[1]);
    }

    if(strcmp(argv[2], "-") == 0) {
        gid = -1;  
    } else {
        gid = groupIdFromName(argv[2]);
        if(gid == -1)
            cmdLineErr("No such group (%s)\n", argv[2]);
    }
    
    errEnd = FALSE;
    for(i = 3; i < argc; i++) {
        if(chown(argv[i], uid, gid) == -1) {
            fprintf(stderr, "chown: %s fail\n", argv[i]);
            errEnd = TRUE;
        }
    }

    exit(errEnd ? EXIT_FAILURE : EXIT_SUCCESS);
}
