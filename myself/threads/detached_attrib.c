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
    pthread_attr_t attr;

    int s;
    void *retval;
 
    debug("POS1\n");

    s = pthread_attr_init(&attr);   /* Assigns default values */
    if(s != 0)
        errExit("pthread_attr_init");

    debug("POS2\n");
    s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(s != 0)
        errExit("pthread_attr_setdetachstate");

    debug("POS3\n");
    s = pthread_create(&tid, &attr, threadFunc, "Hello World");
    if(s != 0)
        errExit("pthread_create");

    debug("POS4\n");
    s = pthread_attr_destroy(&attr);
    if(s != 0)
        errExit("pthread_attr_destroy");

    /* The main process will not wait the thread exit if we set the thread detached */
    sleep(5);

    exit(EXIT_SUCCESS);
}
