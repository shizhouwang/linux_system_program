#define _BSD_SOURCE /* For major() and minor() */
#include "../lib/tools.h"
#include <sys/stat.h>
#include <time.h>  /* For ctime() */

static void displayStatInfo(const struct stat *st)
{
    if(st == NULL)
        cmdLineErr("Pointer is NULL.\n");

    printf("File type:      \n");
    switch(st->st_mode & S_IFMT) {
        case S_IFSOCK : printf("socket\n");         break;
        case S_IFLNK : printf("symbolic link\n");   break;
        case S_IFREG : printf("regular file\n");    break;
        case S_IFBLK : printf("block device\n");    break;
        case S_IFDIR : printf("directory\n");       break;
        case S_IFCHR : printf("character device\n"); break;
        case S_IFIFO : printf("FIFO\n");             break;
        default : printf("unknown file type\n");     break;
    }

    printf("Device containing i-node : major=%ld,   minor=%ld\n",
                (long)major(st->st_dev), (long)minor(st->st_dev));

    printf("I-node number : %ld\n", (long)st->st_ino);

    printf("Mode : %lo(octal)\n",
            (unsigned long)st->st_mode);

    if(st->st_mode & (S_ISUID | S_ISGID | S_ISVTX)) {
        printf("    special bits set:   %s%s%s\n",
                (st->st_mode & S_ISUID) ? "set-UID" : "",
                (st->st_mode & S_ISGID) ? "set-GID" : "",
                (st->st_mode & S_ISVTX) ? "sticky"  : "");
    }

    printf("Numbers of hard links : %ld\n", st->st_nlink);

    printf("Ownership : UID=%ld GID=%ld\n",
            (long)st->st_uid, (long)st->st_gid);
    
    if(S_ISCHR(st->st_mode) || S_ISBLK(st->st_mode)) {
       printf("Device number(st_rdev) : major=%ld, minor=%ld\n",
               (long)major(st->st_rdev), (long)minor(st->st_rdev));
    }
    printf("File size : %ld bytes\n", (long)st->st_size);
    printf("Optional I/O blocksize : %ld blks\n", (long)st->st_blksize);
    printf("512B blocks allocated : %ld\n", (long)st->st_blocks);

    printf("Last file access :   %s\n", ctime(&st->st_atime));
    printf("Last file modification : %s\n", ctime(&st->st_mtime));
    printf("Last file change : %s\n", ctime(&st->st_ctime));

}

int main(int argc, char *argv[])
{
    struct stat st;
    Boolean statLink;
    int fname;

    if(argc > 2 && strcmp(argv[1], "-l") == 0)
        statLink = 1;

    fname = (statLink == 1) ? 2 : 1;

    if(argc < fname || strcmp(argv[1], "--help") == 0)
        usageErr("%s [options] filepath\n"
                    "Available opions are :\n"
                    "   -l -- Sse lstat instead of stat\n", argv[0]);

    if(statLink) {
        if(lstat(argv[fname], &st) == -1)
            errExit("lstat()");
    } else {
        if(stat(argv[fname], &st) == -1)
            errExit("stat()");
    }
    
    displayStatInfo(&st);

    exit(EXIT_SUCCESS);
}
