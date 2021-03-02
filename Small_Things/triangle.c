#include <stdio.h>
#include <stdlib.h>

#define EXPECT_VALID_VALUE(value)               \
    if (value < 0) {                            \
        fprintf(stderr,"invalid " #value "\n"); \
        exit(1);                                \
    }
#define INSERT_BLANK(count) printf("%*s", count, "");

int main(void) {

    // example
    printf("           #\n");
    printf("          ###\n");
    printf("         #####\n");
    printf("        #######\n");
    printf("       #       #\n");
    printf("      ###     ###\n");
    printf("     #####   #####\n");
    printf("    ####### #######\n");
    printf("   #       #       #\n");
    printf("  ###     ###     ###\n");
    printf(" #####   #####   #####\n");
    printf("####### ####### #######\n");

    int height = -1;
    int stages = -1;

    printf("Input height: ");
    scanf("%d", &height);
    EXPECT_VALID_VALUE(height);

    printf("Input stages: ");
    scanf("%d", &stages);
    EXPECT_VALID_VALUE(stages);

    for (int stage = 0; stage < stages; ++stage) {
        for (int row = 1; row <= height; ++row) {
            INSERT_BLANK(height*(stages-stage-1));
            for (int col_of_stage = 0; col_of_stage <= stage; ++col_of_stage) {
                INSERT_BLANK(height-row);
                for (int col = 0; col <= (row-1)*2; ++col) {
                    printf("#");
                }
                INSERT_BLANK(height-row+1);
            }
            printf("\n");
        }
    }

    return 0;
}

