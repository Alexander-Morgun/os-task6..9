#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

#define n_workers 300
#define n_iter 1000000

void* do_stuff(void *param)
{
    int my_num = ((int *)param)[0];
    srand(my_num);
    int *blocks[n_iter];
    unsigned n_blocks = 0;
    for (unsigned i = 0; i < n_iter; ++i) {
        switch (rand() % 3) {
            case 0:
                if (!(blocks[n_blocks] = calloc(1, 256 * (1 + rand() % 4)))) {
                    printf("Error!\n");
                    exit(1);                
                }
                ++n_blocks;
                break;
            case 1:
                if (n_blocks) {
                    unsigned to_del = rand() % n_blocks;
                    free(blocks[to_del]);
                    --n_blocks;
                    blocks[to_del] = blocks[n_blocks];
                }
                break;
            case 2:
                if (n_blocks) {
                    blocks[rand() % n_blocks][rand() % 64] = rand();
                }
        }
    }
    for (unsigned i = 0; i < n_blocks; ++i) {
        free(blocks[i]);
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
