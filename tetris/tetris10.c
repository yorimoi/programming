#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Windows 32-bit and 64-bit
#ifdef _WIN32
#include <Windows.h>
#include <wincrypt.h>
#include <conio.h>

unsigned long int rng() {
    static HCRYPTPROV prov;
    if (CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, 0)) {
        unsigned long int li = 0;
        if (CryptGenRandom(prov, sizeof(li), (BYTE *) &li)) {
            return li;
        } else {
            fprintf(stderr, "error: CryptGenRandom()\n");
            return rand();
        }
    }
}

// Linux or MacOS
#else
#include <fcntl.h>
#include <termios.h>

#define rng() random()

char getch() {
    char c;
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(0, TCSANOW, &t);
    c = getchar();
    tcsetattr(0, TCSADRAIN, &t);
    return c;
}

int kbhit() {
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
      return 1;
    }
    return 0;
}
#endif

#define H 23
#define W 12

typedef struct {
    int x, y;
    int shape[4][4];
} Mino;

void draw(int field[H][W], Mino *mino) {
    int field_buf[H][W] = {0};
    memcpy(field_buf, field, sizeof(field_buf));

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            field_buf[mino->y+y][mino->x+x] |= mino->shape[y][x];
        }
    }

    printf("\033[H\n");
    for (int y = 2; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            printf(field_buf[y][x] ? "[]" : " .");
        }
        printf("\n");
    }
}

void mino_init(Mino *mino) {
    static const int minos[7] = {
        /* 1234
           5678 */
        15,  // I
        102, // O
        108, // S
        198, // Z
        142, // J
        46,  // L
        78,  // T
    };

    mino->x = 4;
    mino->y = 0;
    memset(mino->shape, 0, sizeof(mino->shape));
    for (int y = 0, r = rand()%7; y < 2; ++y) {
        for (int x = 0; x < 4; ++x) {
            mino->shape[y+1][x] = minos[r] >> (7-(y*4+x)) & 1;
        }
    }
}

int can_move(int field[H][W], int mx, int my, int ms[4][4]) {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (field[my+y][mx+x] & ms[y][x]) {
                return 0;
            }
        }
    }
    return 1;
}

void rotation(int ms[4][4]) {
    int ms_buf[4][4];

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            ms_buf[y][4-1-x] = ms[x][y];
        }
    }
    memcpy(ms, ms_buf, sizeof(ms_buf));
}

void gameloop(int field[H][W], Mino *mino) {
    time_t t = time(NULL);
    while (1) {
        if (t != time(NULL)) {
            t = time(NULL);
            if (can_move(field, mino->x, mino->y+1, mino->shape)) {
                mino->y++;
                draw(field, mino);
            } else {
                // Fix to field
                for (int y = 0; y < 4; ++y) {
                    for (int x = 0; x < 4; ++x) {
                        field[mino->y+y][mino->x+x] |= mino->shape[y][x];
                    }
                }
                // Delete line
                for (int y = 2; y < H-1; ++y) {
                    int can_delete = 1;
                    for (int x = 1; x < W-1; ++x) {
                        can_delete &= field[y][x];
                    }
                    if (can_delete) {
                        for (int y2 = y--; y2 > 0; --y2) {
                            memcpy(field[y2], field[y2-1], sizeof(field[y]));
                        }
                    }
                }
                // Check ameover
                for (int x = 1; x < W-1; ++x) {
                    if (field[1][x]) {
                        printf("%*sGAMEOVER\n", 8, "");
                        return;
                    }
                }
                // Init mino
                t = time(NULL);
                mino_init(mino);
            }
        }

        if (kbhit()) {
            int tmp_x = mino->x,
                tmp_y = mino->y,
                tmp_s[4][4];
            memcpy(tmp_s, mino->shape, sizeof(tmp_s));
            switch (getch()) {
                case 'q': return;
                case 'h': tmp_x--; break;
                case 'j': tmp_y++; break;
                case 'l': tmp_x++; break;
                case ' ': rotation(tmp_s); break;
                case 'k': while (can_move(field, tmp_x, tmp_y+1, tmp_s)) {
                              ++tmp_y;
                          }
                          t = 0;
            }
            if (can_move(field, tmp_x, tmp_y, tmp_s)) {
                mino->x = tmp_x;
                mino->y = tmp_y;
                memcpy(mino->shape, tmp_s, sizeof(tmp_s));
                draw(field, mino);
            }
#ifdef _WIN32
            Sleep(10);
#endif
        }
    }
}

int main(void) {

// Random
#ifdef _WIN32
#else
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
        fprintf(stderr, "error: timespec_get()\n");
        return 1;
    }
    srandom(ts.tv_nsec ^ ts.tv_sec);
#endif

    int field[H][W] = {0};
    Mino mino;

    mino_init(&mino);

    for (int y = 0; y < H; ++y) {
        field[y][0] = field[y][W-1] = 1;
    }
    for (int x = 0; x < W; ++x) {
        field[H-1][x] = 1;
    }

    printf("\033[2J\033[H");
    gameloop(field, &mino);

    return 0;
}
