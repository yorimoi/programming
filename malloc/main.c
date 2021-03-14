#include <assert.h>
#include <stdio.h>
#include "krmalloc.h"

int main(void) {

    int *ary;
    int length = -1;

    printf("length: ");
    fflush(stdin);
    scanf("%d", &length);
    if (length < 0) {
        fprintf(stderr, "invalid length\n");
        return 1;
    }


    // malloc
    ary = (int *)malloc(sizeof(int) * length);
    for (int i = 0; i < length; ++i) {
        ary[i] = i;
    }
    for (int i = 0; i < length; ++i) {
        printf("[%d]", ary[i]);
    }
    printf("\n");
    free(ary);


    // calloc
    ary = (int *)calloc(length, sizeof(int));
    for (int i = 0; i < length; ++i) {
        printf("[%d]", ary[i]);
    }
    printf("\n");
    free(ary);


    // double free
    ary = (int *)malloc(42);
    assert(ary != NULL);
    free(ary);
    assert(ary == NULL);
    free(ary);

    return 0;
}

