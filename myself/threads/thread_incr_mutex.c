#include "../lib/tools.h"
#include <pthread.h>

static long glob = 0;
static pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;

static void *threadFunc(void *arg)
{
    long loops;
    long loc,i;

    if(arg == NULL)
        cmdLineErr("Error param.\n");
    
    loops = *(long *)arg;
    for(i = 0; i < loops; i++) {
        if(pthread_mutex_lock(&mtx) != 0)
            errExit("pthread_mutex_lock()");

        loc = glob;
        loc++;
        glob = loc;

        if(pthread_mutex_unlock(&mtx) != 0)
            errExit("pthread_mutex_unlock()");
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int res;
    long loops;
    
    loops = (argc > 1) ? atol(argv[1]) : 1000000;

    res = pthread_create(&t1, NULL, threadFunc, &loops);
    if(res != 0)
        errExit("pthread_create()");
    res = pthread_create(&t2, NULL, threadFunc, &loops);
    if(res != 0)
        errExit("pthread_create()");

    res = pthread_join(t1, NULL);
    if(res != 0)
        errExit("pthread_join()");
    res = pthread_join(t2, NULL);
    if(res != 0)
        errExit("pthread_join()");

    printf("golb=%ld.\n", glob);

    exit(EXIT_SUCCESS);
}
