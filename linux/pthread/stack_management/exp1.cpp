#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#define NTHREADS 4
#define MEGEXTRA 1000000

pthread_attr_t attr[NTHREADS];
int n_size[NTHREADS];

void *dowork(void *threadid)
{
    size_t mystacksize;

    long tid = (long)threadid;
    double A[n_size[tid]][n_size[tid]];
    printf("Thread %ld, A address: %p\n", tid, A);
    pthread_attr_getstacksize (&attr[tid], &mystacksize);
    printf("Thread %ld: stack size = %li bytes\n", tid, mystacksize);

    void* stack = NULL;
    printf("Thread %ld, stack address: %p\n", tid, &stack);
    size_t stacksize;

    // 这个函数，stack指向的到底是个啥？
    int rc = pthread_attr_getstack(&attr[tid], &stack, &stacksize);
    printf("Thread %ld: stack size = %li bytes, stackaddr=%p\n", tid, stacksize, stack);

    for (int i=0; i<n_size[tid]; i++) {
        for (int j = 0; j<n_size[tid]; j++) {
            A[i][j] = ((i*j)/3.452) + (n_size[tid]-i);
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NTHREADS];
    int rc;

    int page_size = getpagesize();
    printf("page size: %d\n", page_size);

    for (int i = 0; i < NTHREADS; ++i) {
        size_t stacksize;
        pthread_attr_init(&attr[i]);
        //n_size[i] = 100 * i * i;
        n_size[i] = 100;
        pthread_attr_getstacksize(&attr[i], &stacksize);
        printf("Default stack size = %li\n", stacksize);
    }

    for (int i = 0; i < NTHREADS; ++i) {
        // 设置的栈大小，必须是页大小的整数倍
        size_t stacksize = ((sizeof(double)*n_size[i]*n_size[i]+MEGEXTRA) / 4096+1) * 4096;
        printf("Amount of stack needed per thread = %li\n",stacksize);
        rc = pthread_attr_setstacksize(&attr[i], stacksize);
        if (rc != 0) {
            printf("pthread_attr_setstacksize failed\n");
            exit(-1);
        }
        printf("Creating threads with stack size = %li bytes\n",stacksize);
    }

    for(int i = 0; i < NTHREADS; i++){
        printf("Created %d threads.\n", i);
        rc = pthread_create(&threads[i], &attr[i], dowork, (void *)((long)i));
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    pthread_exit(NULL);
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}
