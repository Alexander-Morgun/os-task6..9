#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

#define n_workers 2
#define n_iter 4000000000

volatile int counters[n_workers];

void* do_stuff(void *param)
{
    int my_num = ((int *)param)[0];
    for (unsigned i = 0; i < n_iter; ++i) {
        counters[my_num] += 1;
    }
    free(param);
    pthread_exit(NULL);
    return NULL;   
}

int main()
{
    pthread_t tid[n_workers];
    time_t beg_t = time(NULL);
    for (int i = 0; i < n_workers; ++i) {
        int *arg = calloc(1, sizeof(int));
        arg[0] = i;
        pthread_create(&tid[i], NULL, do_stuff, arg);
    }
    for (int i = 0; i < n_workers; ++i) {
        pthread_join(tid[i], NULL);
    }
    printf("%d seconds\n", (int)(time(NULL) - beg_t));
    return 0;
}
