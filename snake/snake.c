#include <stdio.h>

enum {
    FIELD_WIDTH  = 16,
    FIELD_HEIGHT = 16,
    SNAKE_LENGHT_START = 3,
    SNAKE_LENGHT_MAX = 256,
};

typedef struct {
    int 
} Snake;
Snake snake[SNAKE_LENGHT_MAX];

void display() {
    // 枠の表示
    for(int j=-1; j<=FIELD_WIDTH; ++j)
        printf("■");
    printf("\n");
    for(int i=0; i<FIELD_HEIGHT; ++i) {
        for(int j=-1; j<=FIELD_WIDTH; ++j)
            printf(j<0||FIELD_WIDTH==j?"■":"  ");
        printf("\n");
    }
    for(int j=-1; j<=FIELD_WIDTH; ++j)
        printf("■");

    // 餌の表示

    // 蛇の表示
    printf("■");
}

int main(void) {

    display();

    return 0;
}
