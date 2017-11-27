#include "../lib/tools.h"
#include <pthread.h>

static void * threadFunc(void *arg)
{
    char *s;

    if(arg != NULL) {
        s = (char *)arg;
        printf("arg to thread=%s.\n", s);
        return (void *)strlen(s);
    }
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    int s;
    void *retval;
    
    printf("In main() function\n");
    s = pthread_create(&tid, NULL, threadFunc, "Hello World");
    if(s != 0)
        errExit("pthread_create");

    s = pthread_join(tid, &retval);
    if(s != 0)
        errExit("pthread_join");

    printf("retval=%ld\n", (long)retval);

    exit(EXIT_SUCCESS);
}
