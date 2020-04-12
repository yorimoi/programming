#include "printg.h"

#include <stdio.h>

int fprintg(int fd, const char* fmt, ...) {
    printf("%s\n", fmt);

    return 0;
}

