#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void) {
    const char *plane = "Alice and Bob", *p;
    unsigned char *key,   *k;
    unsigned char *crypt, *c;
    int len = strlen(plane);

    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
        fprintf(stderr, "error: timespec_get()\n");
        return 1;
    }
    srandom(ts.tv_nsec ^ ts.tv_sec);

    k = key = (unsigned char *) malloc(len);
    c = crypt = (unsigned char *) malloc(len);

// Create key
    for (int _i = len; _i; --_i) {
        *k++ = random() % 256;
    }

// Encode
    p = plane;
    k = key;
    for (int _i = len; _i; --_i) {
        *c++ = *p++ ^ *k++;
    }

// Display
    printf("[PLANE TEXT]\n%s\n", plane);
    p = plane;
    for (int _i = len; _i; --_i, ++p) {
        for (int j = 0; j < 8; ++j) {
            printf("%d", *p >> j & 1);
        }
    }
    printf("\n[KEY]\n");
    k = key;
    for (int _i = len; _i; --_i, ++k) {
        for (int j = 0; j < 8; ++j) {
            printf("%d", *k >> j & 1);
        }
    }
    printf("\n[CRYPT TEXT]\n");
    c = crypt;
    for (int _i = len; _i; --_i, ++c) {
        for (int j = 0; j < 8; ++j) {
            printf("%d", *c >> j & 1);
        }
    }
    printf("\n");

// Decode
    printf("[DECODE]\n");
    c = crypt;
    k = key;
    for (int _i = len; _i; --_i) {
        printf("%c", *c++ ^ *k++);
    }
    printf("\n");

    return 0;
}
