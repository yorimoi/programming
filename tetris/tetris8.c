#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 24

enum {
    MINO_TYPE_I,
    MINO_TYPE_O,
    MINO_TYPE_S,
    MINO_TYPE_Z,
    MINO_TYPE_L,
    MINO_TYPE_J,
    MINO_TYPE_T,
    MINO_TYPE_MAX,
};

int field[FIELD_HEIGHT * FIELD_WIDTH];
int field_buf[FIELD_HEIGHT * FIELD_WIDTH];

/* I 0b0000000011110000 */
/* O 0b0000011001100000 */
/* S 0b0000011011000000 */
/* Z 0b0000110001100000 */
/* L 0b0000111010000000 */
/* J 0b0000100011100000 */
/* T 0b0000010011100000 */
int minos[MINO_TYPE_MAX] = { 240, 1632, 1728, 3168, 3712, 2272, 1248 };

int mino_x, mino_y, mino_shape;

void draw(void) {
    memcpy(field_buf, field, sizeof(field));
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            field_buf[(mino_y+y)*FIELD_WIDTH+mino_x+x]
                |= mino_shape >> (y*4+x) & 1;
        }
    }

    printf("\033[H");
    for (int y = 0; y < 3; ++y) {
        printf("\n  ");
        for (int x = 1; x < FIELD_WIDTH-2; ++x) {
            printf(field_buf[y*FIELD_WIDTH+x] ? "■" : "  ");
        }
    }
    for (int yx = 3 * FIELD_WIDTH; yx < FIELD_HEIGHT * FIELD_WIDTH; ++yx) {
        if (!(yx % FIELD_WIDTH)) {
            printf("\n");
        }
        printf(field_buf[yx] ? "■" : " .");
    }
    printf("\n");
}

/* Collision Detection */
int cd(int mx, int my, int ms) {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (field[(my+y)*FIELD_WIDTH+mx+x] & (ms >> (y*4+x) & 1)) {
                return 1;
            }
        }
    }
    return 0;
}

/* Rotation */
void rot() {
    int ms_buf;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ms_buf |= (mino_shape >> (4-1-i+4*j) & 1) << (4*i+j);
        }
    }
    if (cd(mino_x, mino_y, ms_buf)) {
        return;
    }
    mino_shape = ms_buf;
}

void init(void) {
    mino_x = 4;
    mino_y = 0;
    mino_shape = minos[rand() % MINO_TYPE_MAX];
}

void gameloop(void) {
    time_t t = time(NULL);

    while (1) {
        if (t != time(NULL)) {
            t = time(NULL);
            draw();
            if (!cd(mino_x, mino_y+1, mino_shape)) {
                ++mino_y;
            } else {
                /* Fix to field */
                for (int y = 0; y < 4; ++y) {
                    for (int x = 0; x < 4; ++x) {
                        field[(mino_y+y)*FIELD_WIDTH+mino_x+x]
                            |= mino_shape >> (y*4+x) & 1;
                    }
                }
                /* Delete line */
                for (int y = FIELD_HEIGHT-2; y > 0; --y) {
                    int can_delete = 1;
                    for (int x = 1; x < FIELD_WIDTH-1; ++x) {
                        if (!field[y*FIELD_WIDTH+x]) {
                            can_delete = 0;
                        }
                    }
                    if (can_delete) {
                        for (int y2 = y; y2 > 0; --y2) {
                            for (int x2 = 0; x2 < FIELD_WIDTH-1; ++x2) {
                                field[y2*FIELD_WIDTH+x2]
                                    = field[(y2-1)*FIELD_WIDTH+x2];
                            }
                        }
                        ++y;
                    }
                }
                /* Check Gameover */
                for (int x = 1; x < FIELD_WIDTH-1; ++x) {
                    if (field[2*FIELD_WIDTH+x]) {
                        printf("%*sGAME OVER\n", 8, "");
                        return;
                    }
                }

                init();
            }
        }

        if (kbhit()) {
            switch (getch()) {
                case 'q': return;
                case 'h': if(!cd(mino_x-1, mino_y, mino_shape)) --mino_x; break;
                case 'j': if(!cd(mino_x, mino_y+1, mino_shape)) ++mino_y; break;
                case 'l': if(!cd(mino_x+1, mino_y, mino_shape)) ++mino_x; break;
                case ' ': rot(); break;
                case 'k': while(!cd(mino_x, mino_y+1, mino_shape)) {
                              ++mino_y;
                          }
                          t = 0;
            }
            draw();
        }
    }
}

int main(void) {
    srand((unsigned int) time(NULL));
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        field[y*FIELD_WIDTH] = 1;
        field[y*FIELD_WIDTH+(FIELD_WIDTH-1)] = 1;
    }
    for (int x = 0; x < FIELD_WIDTH; ++x) {
        field[FIELD_WIDTH * (FIELD_HEIGHT-1) + x] = 1;
    }

    init();

    printf("\033[2J");
    draw();

    gameloop();

    return 0;
}
