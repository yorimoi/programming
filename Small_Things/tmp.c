#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DIGITS 16

int ctoi(char c) {
    return c - 48;
}

int main(void){

    char n[8];
    char credit[128][16+1];

    fgets(n, sizeof(n), stdin);

    for(int i=0; i<atoi(n); i++) {
        fgets(credit[i], sizeof(credit[i]), stdin);
        scanf("%*c");
    }

    for(int i=0; i<atoi(n); i++) {
        int even, odd;
        even = odd = 0;
        for(int j=DIGITS-1; j>0; j--) {
            /* even */
            if(j%2) {
                int tmp = ctoi(credit[i][j-1]) * 2;
                if(tmp >= 10)
                    even += tmp/10 + tmp%10;
                else
                    even += tmp;
            /* odd */
            } else {
                odd += ctoi(credit[i][j-1]);
            }
        }
        printf("%d\n", (10 - ((even + odd) % 10)) % 10);
    }

    return 0;
}
