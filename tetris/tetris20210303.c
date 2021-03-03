#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>

#define FIELD_WIDTH  (10+2)
#define FIELD_HEIGHT (20+1)
#define MINO_SIZE        4
#define INIT_POS_X       4
#define INIT_POS_Y       0

typedef enum {
    MinoI,
    MinoO,
    MinoJ,
    MinoL,
    MinoS,
    MinoZ,
    MinoT,
    MinoMax,
} MinoKind;

static int pow_of_2(int n) {
    static int ary[] = { 1, 2, 4, 8, 16, 32, 64, 128, };
    return ary[n];
}

static char getch() {
    char c;
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(0, TCSANOW, &t);
    c = getchar();
    tcsetattr(0, TCSADRAIN, &t);
    return c;
}

static _Bool kbhit() {
    int c, of;
    struct termios ot, nt;
    tcgetattr(0, &ot);
    nt = ot;
    nt.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(0, TCSANOW, &nt);
    of = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, of | O_NONBLOCK);
    c = getchar();
    tcsetattr(0, TCSANOW, &ot);
    fcntl(0, F_SETFL, of);
    if (c != EOF) {
        ungetc(c, stdin);
        return true;
    }
    return false;
}

static void draw(int field[][FIELD_WIDTH], int mino_shape[], int pos_x, int pos_y) {
    int field_buf[FIELD_HEIGHT][FIELD_WIDTH];
    memcpy(field_buf, field, sizeof(int) * FIELD_HEIGHT * FIELD_WIDTH);

    printf("\x1b[H");
    for (int h = 0; h < MINO_SIZE; ++h) {
        for (int w = 0; w < MINO_SIZE; ++w) {
            field_buf[pos_y+h][pos_x+w] |= mino_shape[h] & pow_of_2(MINO_SIZE-1-w);
        }
    }
    for (int h = 0; h < FIELD_HEIGHT; ++h) {
        for (int w = 0; w < FIELD_WIDTH; ++w) {
            printf(field_buf[h][w] ? "[]" : " .");
        }
        printf("\n");
    }
}

static _Bool is_hit(int field[][FIELD_WIDTH], int mino_shape[], int pos_x, int pos_y) {
    for (int h = 0; h < MINO_SIZE; ++h) {
        for (int w = 0; w < MINO_SIZE; ++w) {
            if (field[pos_y+h][pos_x+w] && mino_shape[h] & pow_of_2(MINO_SIZE-1-w)) {
                return true;
            }
        }
    }
    return false;
}

static void rotate(int *mino_shape) {
    int mino_buf[MINO_SIZE] = {0};
    for (int i = 0; i < MINO_SIZE; ++i) {
        for (int j = 0; j < MINO_SIZE; ++j) {
            mino_buf[i] |= (mino_shape[j] & pow_of_2(MINO_SIZE-1-i)) ? pow_of_2(j) : 0;
        }
    }
    memcpy(mino_shape, mino_buf, sizeof(mino_buf));
}

static void fix_filed(int field[][FIELD_WIDTH], int *mino_shape, int pos_x, int pos_y) {
    for (int h = 0; h < MINO_SIZE; ++h) {
        for (int w = 0; w < MINO_SIZE; ++w) {
            field[pos_y+h][pos_x+w] |= mino_shape[h] & pow_of_2(MINO_SIZE-w-1);
        }
    }
}

static void delete_lines(int field[][FIELD_WIDTH]) {
    for (int h = FIELD_HEIGHT-2; 0 <= h; --h) {
        _Bool deletable = true;
        for (int w = 1; w < FIELD_WIDTH-2; ++w) {
            if (field[h][w] == 0) {
                deletable = false;
                break;
            }
        }
        if (deletable) {
            for (int h2 = h++; 0 < h2; --h2) {
                memcpy(field[h2], field[h2-1], sizeof(field[0]));
            }
        }
    }
}

static void init_mino(int *mino_shape, int *pos_x, int *pos_y) {
    static const int minos[][4] = {
        { 0x0, 0x0, 0xF, 0x0, },  // I
        { 0x0, 0x6, 0x6, 0x0, },  // O
        { 0x0, 0x8, 0xE, 0x0, },  // J
        { 0x0, 0x2, 0xE, 0x0, },  // L
        { 0x0, 0x6, 0xC, 0x0, },  // S
        { 0x0, 0xC, 0x6, 0x0, },  // Z
        { 0x0, 0x4, 0xE, 0x0, },  // T
    };

    memcpy(mino_shape, minos[rand() % MinoMax], sizeof(minos[MinoI]));
    *pos_x = INIT_POS_X;
    *pos_y = INIT_POS_Y;
}

static _Bool is_gameover(int field[][FIELD_WIDTH], int *mino_shape) {
    for (int h = 0; h < MINO_SIZE; ++h) {
        for (int w = 0; w < MINO_SIZE; ++w) {
            if (field[INIT_POS_Y+h][INIT_POS_X+w] && (mino_shape[h] & pow_of_2(MINO_SIZE-1-w))) {
                return true;
            }
        }
    }
    return false;
}

int main(void) {
    int field[FIELD_HEIGHT][FIELD_WIDTH] = {0};
    int current_mino_shape[MINO_SIZE];
    int pos_x, pos_y;
    time_t now = time(NULL);

    srand(time(NULL));
    init_mino(current_mino_shape, &pos_x, &pos_y);

    for (int h = 0; h < FIELD_HEIGHT; ++h) {
        field[h][0] = field[h][FIELD_WIDTH-1] = 1;
    }
    for (int w = 0; w < FIELD_WIDTH; ++w) {
        field[FIELD_HEIGHT-1][w] = 1;
    }

    printf("\x1b[2J");
    draw(field, current_mino_shape, pos_x, pos_y);

    while (1) {
        if (now != time(NULL)) {
            now = time(NULL);
            if (!is_hit(field, current_mino_shape, pos_x, pos_y+1)) {
                ++pos_y;
            } else {
                fix_filed(field, current_mino_shape, pos_x, pos_y);
                delete_lines(field);
                init_mino(current_mino_shape, &pos_x, &pos_y);
                if (is_gameover(field, current_mino_shape)) {
                    printf("%*s", FIELD_WIDTH-5, "");
                    printf("GAME OVER\n");
                    break;
                }
            }
            draw(field, current_mino_shape, pos_x, pos_y);
        }

        if (kbhit()) {
            int pos_x_tmp = pos_x;
            int pos_y_tmp = pos_y;
            int mino_shape_tmp[MINO_SIZE];
            memcpy(mino_shape_tmp, current_mino_shape, sizeof(mino_shape_tmp));
            switch (getch()) {
                case 'h': --pos_x_tmp; break;
                case 'j': ++pos_y_tmp; break;
                case 'k': while (!is_hit(field, mino_shape_tmp, pos_x_tmp, pos_y_tmp+1)) {
                              ++pos_y_tmp;
                          };
                          break;
                case 'l': ++pos_x_tmp; break;
                case ' ': rotate(mino_shape_tmp); break;
                case 'q': return 0;
            }
            if (!is_hit(field, mino_shape_tmp, pos_x_tmp, pos_y_tmp)) {
                pos_x = pos_x_tmp;
                pos_y = pos_y_tmp;
                memcpy(current_mino_shape, mino_shape_tmp, sizeof(current_mino_shape));
                draw(field, current_mino_shape, pos_x, pos_y);
            }
        }
    }

    return 0;
}

