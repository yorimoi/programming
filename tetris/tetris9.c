#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

enum {
    FIELD_WIDTH   = 10 + 2,
    FIELD_HEIGHT  = 20 + 4,
    MINO_TYPE_MAX =  7,
};

int field[FIELD_HEIGHT * FIELD_WIDTH];
int field_buf[FIELD_HEIGHT * FIELD_WIDTH];
int mino_x, mino_y, mino_shape;

void draw(void) {
    memcpy(field_buf, field, sizeof(field));
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            field_buf[(mino_y + y) * FIELD_WIDTH + mino_x + x]
                |= mino_shape >> (y * 4 + x) & 1;
        }
    }

    printf("\033[H\n");
    for (int y = 3; y < FIELD_HEIGHT; ++y) {
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            printf(field_buf[y * FIELD_WIDTH + x] ? "[]" : " .");
        }
        printf("\n");
    }
}

/* Collision Detection */
int cd(int mx, int my, int ms) {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (field[(mino_y + my + y) * FIELD_WIDTH + mino_x + mx + x]
                    & (ms >> (y * 4 + x) & 1)) {
                return 1;
            }
        }
    }
    return 0;
}

/* Rotation */
void rot() {
    int ms_buf;
    for (int i = 4 - 1; i >= 0; --i) {
        for (int j = 0; j < 4; ++j) {
            ms_buf |= (mino_shape >> (i + 4 * j) & 1) << (i * 4 + (4 - 1 - j));
        }
    }
    if (!cd( 0, 0, ms_buf)) {
        mino_shape = ms_buf;
    }
}

void init(void) {
    static const int minos[] = { 240, 1632, 1728, 3168, 3712, 2272, 1248 };
    mino_x = 4;
    mino_y = 1;
    mino_shape = minos[rand() % MINO_TYPE_MAX];
}

void gameloop(void) {
    time_t t = 0;
    while (1) {
        if (t != time(NULL)) {
            t = time(NULL);
            if (!cd( 0, +1, mino_shape)) {
                ++mino_y;
            } else {
                /* Fix to field */
                memcpy(field, field_buf, sizeof(field));

                /* Delete line */
                for (int y = FIELD_HEIGHT - 2; y > 0; --y) {
                    int can_delete = 1;
                    for (int x = 1; x < FIELD_WIDTH - 1; ++x) {
                        can_delete &= field[y * FIELD_WIDTH + x];
                    }
                    if (can_delete) {
                        for (int y2 = y++; y2 > 0; --y2) {
                            for (int x2 = 0; x2 < FIELD_WIDTH - 1; ++x2) {
                                field[y2 * FIELD_WIDTH + x2]
                                    = field[(y2 - 1) * FIELD_WIDTH + x2];
                            }
                        }
                    }
                }

                /* Check Gameover */
                for (int x = 1; x < FIELD_WIDTH - 1; ++x) {
                    if (field[2 * FIELD_WIDTH + x]) {
                        printf("\n%*sGAME OVER\n", 8, "");
                        return;
                    }
                }

                t = 0;
                init();
            }
            draw();
        }

        if (kbhit()) {
            switch (getch()) {
                case 'q': return;
                case 'h': if (!cd(-1,  0, mino_shape)) --mino_x; break;
                case 'j': if (!cd( 0, +1, mino_shape)) ++mino_y; break;
                case 'l': if (!cd(+1,  0, mino_shape)) ++mino_x; break;
                case ' ': rot(); break;
                case 'k': while (!cd( 0, +1, mino_shape)) ++mino_y; t = 0;
            }
            draw();
        }
    }
}

int main(void) {
    srand((unsigned int) time(NULL));
    for (int y = 0; y < FIELD_HEIGHT * FIELD_WIDTH; y += FIELD_WIDTH) {
        field[y] = field[y + (FIELD_WIDTH - 1)] = 1;
    }
    for (int x = 0; x < FIELD_WIDTH; ++x) {
        field[FIELD_WIDTH * (FIELD_HEIGHT - 1) + x] = 1;
    }
    init();
    printf("\033[2J");

    gameloop();

    return 0;
}
