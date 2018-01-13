#include "../lib/tools.h"
#include <ctype.h>


#define POPEN_FMT "/bin/ls -d %s 2>/dev/null"
#define PAT_SIZE 50
#define PCMD_BUF_SIZE (sizeof(POPEN_FMT) + PAT_SIZE)
#define PATH_MAX 1024

int main(int argc, char *argv[])
{
    char pat[PAT_SIZE];
    char pcmd[PCMD_BUF_SIZE];
    char filepath[PATH_MAX];
    FILE *fd;
    int len, i, fileCnt, status;
    Boolean badPatten = FALSE; /* Is it a bad patten character */

    for (;;) {
        printf("patten:");
        fflush(stdout);

        if (fgets(pat, sizeof(pat), stdin) == NULL)
            errExit("fgets()");
        
        len = strlen(pat);
        if (len < 1)
            continue;

        if (pat[len - 1] == '\n')
            pat[len - 1] = '\0';

        for (i = 0, badPatten = FALSE; i < len && !badPatten; i++) {
            if(!isalnum((unsigned char)pat[i]) && 
                    strchr("_*?[^-].", (unsigned char)pat[i]) == NULL)
                badPatten = TRUE;
        }
        
        if (badPatten) {
            printf("Bad pattern character:%c\n", pat[i - 1]);
            continue;
        }
        
        snprintf(pcmd, sizeof(pcmd), POPEN_FMT, pat);
        fd = popen(pcmd, "r");
        if (fd == NULL)
            errExit("open()");
        
        fileCnt = 0;
        while (fgets(filepath, sizeof(filepath), fd)) {
            printf("%s\n", filepath);
            fileCnt++;
        }

        status = pclose(fd);
        printf("    %d matching file%s\n", fileCnt, (fileCnt != 1) ? "s" : "");
        printf("    pclose status = %d\n", status);
    }

    exit(EXIT_SUCCESS);
}
