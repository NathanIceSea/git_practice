#define __USE_MINGW_ANSI_STDIO 1
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "make_random_array.h"
#include "linked_list.h"

// 基数
#define RADIX 10000

// 创建基数排序桶
List* MakeRadixBuckets(size_t radix) {
    List* li = (List*) malloc(sizeof(List) * radix);
    for (size_t i = 0; i < radix; i++) {
        li[i] = MakeEmpty();
    }
    return li;
}
// 释放基数排序桶
void FreeRadixBuckets(List* buckets, size_t size) {
    for (size_t i = 0; i < size; i++) {
        List li = buckets[i];
        DeleteList(li);
    }
    free(buckets);
}

void bench(size_t length) {
    // 创建和打印待排序数组
    unsigned int *array = (unsigned int*) MakeRandArr(length, true);
//    unsigned int array[10] = {64, 8, 216, 512, 27, 729, 0, 1, 343, 125};
//    length = 10;
    printf("--------------------%010zd--------------------\n", length);
//    for (size_t i = 0; i < length; i++) {
//        printf("%u ", array[i]);
//    }
//    puts("");
//    puts("--------------------------------------------------");

    // 初始化基数排序桶并进行第一轮排序
    List *lastBuckets = MakeRadixBuckets(RADIX);
    for (size_t i = 0; i < length; i++) {
        List bucket = lastBuckets[array[i] % RADIX];
        Insert(bucket, array[i], bucket);
    }

    // 注意插入位置、桶内查找顺序、桶间扫描顺序 与 结果顺序间的联系
    // 示范：上端插入，从左往右从下向上扫描 -> 下小上大，左小右大；最终结果同序
    bool go_on = true;
//    for (size_t div = RADIX; go_on; div *= RADIX) {
//        go_on = false;
//        List *tempBuckets = MakeRadixBuckets(RADIX);
//        for (int i = 0; i < RADIX; i++) {
//            List lastBucket = lastBuckets[i];
//            for (PtrToNode ptr = lastBucket->next; ptr; ptr = ptr->next) {
//                unsigned int thisNum = ptr->elem;
//                if (thisNum >= div)
//                    go_on = true;
//                List bucket = tempBuckets[(thisNum / div) % RADIX];
//                Insert(Last(bucket), thisNum, bucket);
//            }
//        }
//        FreeRadixBuckets(lastBuckets, RADIX);
//        lastBuckets = tempBuckets;
//    }

//    for (int i = 0; i < RADIX; i++) {
//        List bucket = lastBuckets[i];
//        for (PtrToNode ptr = bucket->next; ptr; ptr = ptr->next) {
//            unsigned int thisNum = ptr->elem;
//            printf("%u ", thisNum);
//        }
//    }
//    puts("");
//    puts("--------------------------------------------------");
    FreeRadixBuckets(lastBuckets, RADIX);


    lastBuckets = MakeRadixBuckets(RADIX);
    for (size_t i = 0; i < length; i++) {
        List bucket = lastBuckets[array[i] % RADIX];
        Insert(bucket, array[i], bucket);
    }
    // 示范2：上端插入，从右（大）往左（下）从下（大）向上（小）扫描；结果同序
    // 如果：下端插入，从右（大）往左（下）从上（大）向下（小）扫描；结果同样同序（只是桶内顺序变为上大下小）
    // 这里由于单链表不支持逆向迭代故不演示
    clock_t begin = clock();
    go_on = true;
    for (size_t div = RADIX; go_on; div *= RADIX) {
        go_on = false;
        List *tempBuckets = MakeRadixBuckets(RADIX);
        for (int i = RADIX - 1; i >= 0; i--) {
            List lastBucket = lastBuckets[i];
            for (PtrToNode ptr = lastBucket->next; ptr; ptr = ptr->next) {
                unsigned int thisNum = ptr->elem;
                if (thisNum >= div)
                    go_on = true;
                List bucket = tempBuckets[(thisNum / div) % RADIX];
                Insert(Last(bucket), thisNum, bucket);
            }
        }
        FreeRadixBuckets(lastBuckets, RADIX);
        lastBuckets = tempBuckets;
    }
    clock_t end = clock();

    for (int i = RADIX - 1; i >=0 ; i--) {
        List bucket = lastBuckets[i];
        for (PtrToNode ptr = bucket->next; ptr; ptr = ptr->next) {
            unsigned int thisNum = ptr->elem;
            printf("%u ", thisNum);
        }
    }
    puts("");
    printf("--------------------%010f--------------------\n", (double)(end - begin) / CLOCKS_PER_SEC);

    FreeRadixBuckets(lastBuckets, RADIX);
}

int main() {
    bench(50);
    // bench(500);
    // bench(5000);
    // bench(50000);
    return 0;
}