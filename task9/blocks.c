#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

#define n_workers 8
#define matrix_size 2048
#define n_blocks 128
#define block_size matrix_size/n_blocks

static long long A[n_blocks][n_blocks][block_size][block_size],
                 B[n_blocks][n_blocks][block_size][block_size],
                 C[n_blocks][n_blocks][block_size][block_size];
static unsigned lines_per_thread = matrix_size / n_workers;

long long* get_element(long long matr[n_blocks][n_blocks][block_size][block_size], unsigned y, unsigned x)
{
    return &matr[y / n_blocks][x / n_blocks][y % n_blocks][x % n_blocks];
}

void* do_stuff(void *param)
{
    int my_num = ((int *)param)[0];
    unsigned beg = lines_per_thread * my_num;
    for (unsigned i = 0; i < lines_per_thread; ++i) {
        for (unsigned j = 0; j < matrix_size; ++j) {
            long long rez = 0;
            for (unsigned k = 0; k < matrix_size; ++k) {
                rez += *get_element(A, beg + i, k) * *get_element(B, k, j);
            }
            *get_element(C, beg + i, j) = rez;
        }
    }
    free(param);
    pthread_exit(NULL);
    return NULL;   
}

int main()
{
    srand(139);
    for (unsigned i = 0; i < matrix_size; ++i) {
        for (unsigned j = 0; j < matrix_size; ++j) {
            *get_element(A, i, j) = rand();
            *get_element(B, i, j) = rand();
        }
    }
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
