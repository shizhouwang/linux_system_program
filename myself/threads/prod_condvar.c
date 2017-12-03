#include "../lib/tools.h"
#include <pthread.h>
#include <time.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int avail = 0;

static void *threadFunc(void *arg)
{
    int num;
    int ret, i;

    if(arg == NULL)
        cmdLineErr("NULL Pointer");

    num = atoi((char *)arg);
    debug("Index:%d, need to produce %d units.\n", num, num);

    
    debug("Index:%d, going to produce units.\n", num);
    for(i = 0; i < num; i++) {
        sleep(1);

        ret = pthread_mutex_lock(&mtx);
        if(ret != 0)
            errExit("pthread_mutex_lock()");

        avail ++;

        ret = pthread_mutex_unlock(&mtx);
        if(ret != 0)
            errExit("pthread_mutex_unlock()");

        debug("Index:%d, produce units end, avail:%d, going to wake up main thread.\n", num, avail);
        ret = pthread_cond_signal(&cond);
        if(ret != 0)
            errExit("pthread_cond_signal()");
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int totRequired = 0;    /* Total number of units that all thread will produce */
    int numConsumed = 0;    /* Total units so far consumed */
    int done = 0;
    time_t t;
    pthread_t tid;
    int i, ret;

    t = time(NULL);
    
    for(i = 1; i < argc; i++) {
        totRequired += atoi(argv[i]);
    }
    
    debug("main: the total required units are %d, going to create %d threads.\n", totRequired, argc - 1);
    for(i = 1; i < argc; i++) {
        ret = pthread_create(&tid, NULL, threadFunc, (void *)argv[i]);
        if(ret != 0)
            errExit("pthread_create()");
    }

    debug("main: enter polling loop.\n", argc - 1);
    for(;;) {
        ret = pthread_mutex_lock(&mtx);
        if(ret != 0)
            errExit("pthread_mutex_lock()");
        
        while(avail == 0) {
            debug("main: have to wait the wake up signal.\n");
            ret = pthread_cond_wait(&cond, &mtx);
            if(ret != 0)
                errExit("pthread_mutex_wait()");
        }

        debug("main: recieved the wake up signal, available units are %d, consumed units are %d.\n", avail, numConsumed);
        while(avail > 0) {
            numConsumed ++;
            avail --;
            
            done = (numConsumed >= totRequired) ? 1 : 0;
        }
           
        debug("main: after this loop, available units are %d, consumed units are %d.\n", avail, numConsumed);
        ret = pthread_mutex_unlock(&mtx);
        if(ret != 0)
            errExit("pthread_mutex_unlock()");

        if(done)
            break;
    }
    
    exit(EXIT_SUCCESS);
}
