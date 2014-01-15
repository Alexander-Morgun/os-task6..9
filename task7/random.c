#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#define n_memory 300000000
#define n_iter 1000000000

int32_t memory[n_memory];

int main()
{
    srand(139);
    time_t beg_t = time(NULL);
    for (unsigned i = 0; i < n_iter; ++i) {
        memory[(rand() * 1ull * rand()) % n_memory] = rand();
    }
    printf("%d seconds\n", (int)(time(NULL) - beg_t));
    return 0;
}
