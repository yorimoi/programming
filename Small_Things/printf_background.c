#include <stdio.h>
#include <unistd.h>

int main() {

    daemon(0, 0);

    printf("Hello, World!\n");

    for(;;);

    return 0;
}
