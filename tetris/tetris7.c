#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <conio.h>

#define FIELD_WIDTH   12
#define FIELD_HEIGHT  24

int cells[FIELD_HEIGHT][FIELD_WIDTH];
int cells_buf[FIELD_HEIGHT][FIELD_WIDTH];

enum {
    MINO_TYPE_I,
    MINO_TYPE_O,
    MINO_TYPE_S,
    MINO_TYPE_Z,
    MINO_TYPE_L,
    MINO_TYPE_J,
    MINO_TYPE_T,
    MINO_TYPE_MAX
};

int minos[MINO_TYPE_MAX][4] = {
    /*   8421
     * 0 0000
     * 1 0000
     * 2 0000
     * 3 0000 */

    /* I */
    {  0,  0, 15,  0 },
    /* O */
    {  0,  6,  6,  0 },
    /* S */
    {  0,  6, 12,  0 },
    /* Z */
    {  0, 12,  6,  0 },
    /* L */
    {  0,  2, 14,  0 },
    /* J */
    {  0, 14,  2,  0 },
    /* T */
    {  0,  4, 14,  0 },
};

int current_mino[4];

int mino_type, mino_x, mino_y;

void display(void) {
    memcpy(cells_buf, cells, sizeof(cells_buf));
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            cells_buf[mino_y+y][mino_x+x] |= (current_mino[y] >> x) & 1;
        }
    }

    printf("\033[H");
    for (int y = 1; y < 3; ++y) {
        printf("  ");
        for (int x = 1; x < FIELD_WIDTH - 1; ++x) {
            printf(cells_buf[y][x] ? "[]" : "  ");
        }
        printf("\n");
    }
    for (int y = 3; y < FIELD_HEIGHT; ++y) {
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            printf(cells_buf[y][x] ? "[]" : " .");
        }
        printf("\n");
    }
}

/* Create a new mino */
void init(void) {
    mino_x = 4;
    mino_y = 0;
    mino_type = rand() % MINO_TYPE_MAX;
    memcpy(current_mino, minos[mino_type], sizeof(current_mino));
}

/* Collision Detection */
int cd(int _mino_x, int _mino_y, int *mino_shape) {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (cells[_mino_y+y][_mino_x+x] & ((mino_shape[y] >> x) & 1)) {
                return 1;
            }
        }
    }
    return 0;
}

/* Right Rotation */
void rotation(void) {
    int mino_buf[4] = {0};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            // mino_buf[4 - 1 - j] |= ((current_mino[i] >> j) & 1) << i; // Left
            mino_buf[j] |= ((current_mino[4 - 1 - i] >> j) & 1) << i;
        }
    }
    if (!cd(mino_x, mino_y, mino_buf)) {
        memcpy(current_mino, mino_buf, sizeof(current_mino));
    }
}

void gameloop(void) {
    time_t t = time(NULL);

    while (1) {
        if (!kbhit()) {
            if (t == time(NULL)) {
                continue;
            }
            t = time(NULL);
            if (!cd(mino_x, mino_y + 1, current_mino)) {
                ++mino_y;
            } else {
                /* Fix to field */
                for (int y = 0; y < 4; ++y) {
                    for (int x = 0; x < 4; ++x) {
                        cells[mino_y+y][mino_x+x] |= (current_mino[y] >> x) & 1;
                    }
                }
                /* Delete line */
                for (int y = FIELD_HEIGHT - 2; y > 0; --y) {
                    int can_delete = 1;
                    for (int x = 1; x < FIELD_WIDTH - 2; ++x) {
                        if (!cells[y][x]) {
                            can_delete = 0;
                            break;
                        }
                    }
                    if (can_delete) {
                        for (int z = y++; z > 0; --z) {
                            memcpy(cells[z], cells[z-1], sizeof(cells[z]));
                        }
                    }
                }
                /* Check GameOver */
                for (int x = 1; x < FIELD_WIDTH - 2; ++x) {
                    if (cells[2][x]) {
                        printf("%*sGAME OVER\n", 8, "");
                        return;
                    }
                }

                init();
            }
            display();
        } else {
            int x_tmp = mino_x,
                y_tmp = mino_y;
            switch (getch()) {
                case 'q': return;     break;
                case 'h': --x_tmp;    break;
                case 'j': ++y_tmp;    break;
                case 'l': ++x_tmp;    break;
                case ' ': rotation(); break;
                case 'k':
                    while (!cd(mino_x, ++mino_y, current_mino));
                    --mino_y;
                    t = 0;
                    continue;
            }
            if (!cd(x_tmp, y_tmp, current_mino)) {
                mino_x = x_tmp;
                mino_y = y_tmp;
            }
            display();
        }
    }
}

int main(void) {

    srand((unsigned int) time(NULL));
    for (int x = 0; x < FIELD_WIDTH; ++x) {
        cells[FIELD_HEIGHT-1][x] = 1;
    }
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        cells[y][0] = cells[y][FIELD_WIDTH-1] = 1;
    }
    printf("\033[2J\033[H");
    init();

    display();

    gameloop();

    return 0;
}

