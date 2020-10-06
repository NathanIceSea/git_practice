#ifndef _MAKE_RANDOM_ARRAY_H_
#define _MAKE_RANDOM_ARRAY_H_ 1

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int seed;
bool init = false;
int* MakeRandArr(size_t length, bool same) {
    // 初始化种子
    if (!init || !same) {
        srand(time(NULL));
        seed = rand();
        init = true;
    }
    srand(seed);

    int *arr = (int *) malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
        arr[i] = rand() - 10000;
    return arr;
}

#endif