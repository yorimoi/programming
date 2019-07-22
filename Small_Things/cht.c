/* CHange Title for linux

   Usage
     cht title
*/

#include <stdio.h>

int main(int argc, char **argv) {

    if(argc < 2) {
        printf("Invalid argument\n");
        return 1;
    }

    printf("\033]1;%s\007", argv[1]);

    return 0;
}
