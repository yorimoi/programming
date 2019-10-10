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
#endif

#define H 4
#define W 4

typedef enum {
    NUM_NONE, NUM_2,    NUM_4,     NUM_8,     NUM_16,    NUM_32,
    NUM_64,   NUM_128,  NUM_256,   NUM_512,   NUM_1024,  NUM_2048,
    NUM_4096, NUM_8192, NUM_16384, NUM_32768, NUM_65536, NUM_MAX,
}Num;

typedef enum {
    VEC_LEFT,  // h
    VEC_DOWN,  // j
    VEC_UP,    // k
    VEC_RIGHT, // l
}Vec;

Num field[H][W];
char *nums[NUM_MAX] = {
    "     ", "  2  ", "  4  ", "  8  ", "  16 ", "  32 ",
    "  64 ", " 128 ", " 256 ", " 512 ", " 1024", " 2048",
    " 4096", " 8192", "16384", "32768", "65536",
};

char moved;
char clear;

void draw() {
    printf("\033[H");
    for (int x = 0; x < W; ++x) { printf("+-----"); } printf("+\n");
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) { printf("|     "); } printf("|\n");
        for (int x = 0; x < W; ++x) { printf("|%s", nums[field[y][x]]); }
        printf("|\n");
        for (int x = 0; x < W; ++x) { printf("|     "); } printf("|\n");
        for (int x = 0; x < W; ++x) { printf("+-----"); } printf("+\n");
    }
}

/// return: 0bxxxxyyyy
unsigned char get_random_null_cell() {
    int x, y;
    while (1) {
        x = rng() % W;
        y = rng() % H;
        if (field[y][x] == NUM_NONE) {
            return (x << 4) + y;
        }
    }
}

char exist_null_cell() {
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            if (field[y][x] == NUM_NONE)
                return 1;
        }
    }
    return 0;
}

char is_null_cell(int x, int y) {
    return field[y][x] == NUM_NONE;
}

void move(Vec v) {
    switch (v) {
        case VEC_LEFT:
            for (int y = 0; y < H; ++y) {
                for (int x = 1; x < W; ++x) {
                    int x2 = x;
                    while (field[y][x2] != NUM_NONE && is_null_cell(x2-1, y)) {
                        moved = 1;
                        field[y][x2-1] = field[y][x2];
                        field[y][x2] = NUM_NONE;
                        --x2;
                        if (x2 < 1) break;
                    }
                    if (x2 && field[y][x2] != NUM_NONE && field[y][x2-1] == field[y][x2]) {
                        moved = 1;
                        ++field[y][x2-1];
                        if (field[y][x2-1] == NUM_MAX-1) {
                            clear = 1;
                        }
                        field[y][x2] = NUM_NONE;
                    }
                }
            }
            break;

        case VEC_DOWN:
            for (int x = 0; x < W; ++x) {
                for (int y = H-2; y >= 0; --y) {
                    int y2 = y;
                    while (field[y2][x] != NUM_NONE && is_null_cell(x, y2+1)) {
                        moved = 1;
                        field[y2+1][x] = field[y2][x];
                        field[y2][x] = NUM_NONE;
                        ++y2;
                        if (y2 > H-2) break;
                    }
                    if (y2 != H-1 && field[y2][x] != NUM_NONE && field[y2+1][x] == field[y2][x]) {
                        moved = 1;
                        ++field[y2+1][x];
                        if (field[y2+1][x] == NUM_MAX-1) {
                            clear = 1;
                        }
                        field[y2][x] = NUM_NONE;
                    }
                }
            }
            break;

        case VEC_UP:
            for (int x = 0; x < W; ++x) {
                for (int y = 1; y < H; ++y) {
                    int y2 = y;
                    while (field[y2][x] != NUM_NONE && is_null_cell(x, y2-1)) {
                        moved = 1;
                        field[y2-1][x] = field[y2][x];
                        field[y2][x] = NUM_NONE;
                        --y2;
                        if (y2 < 1) break;
                    }
                    if (y2 && field[y2][x] != NUM_NONE && field[y2-1][x] == field[y2][x]) {
                        moved = 1;
                        ++field[y2-1][x];
                        if (field[y2-1][x] == NUM_MAX-1) {
                            clear = 1;
                        }
                        field[y2][x] = NUM_NONE;
                    }
                }
            }
            break;

        case VEC_RIGHT:
            for (int y = 0; y < H; ++y) {
                for (int x = W-2; x >= 0; --x) {
                    int x2 = x;
                    while (field[y][x2] != NUM_NONE && is_null_cell(x2+1, y)) {
                        moved = 1;
                        field[y][x2+1] = field[y][x2];
                        field[y][x2] = NUM_NONE;
                        ++x2;
                        if (x2 > W-2) break;
                    }
                    if (x2 != W-1 && field[y][x2] != NUM_NONE && field[y][x2+1] == field[y][x2]) {
                        moved = 1;
                        ++field[y][x2+1];
                        if (field[y][x2+1] == NUM_MAX-1) {
                            clear = 1;
                        }
                        field[y][x2] = NUM_NONE;
                    }
                }
            }
            break;
    }
}

char can_move() {
    if (exist_null_cell()) return 1;

    for (int y = 0; y < H; ++y) {
        for (int x = 1; x < W; ++x) {
            if (field[y][x] != NUM_NONE && field[y][x-1] == field[y][x])
                return 1;
        }
        for (int x = W-2; x >= 0; --x) {
            if (field[y][x] != NUM_NONE && field[y][x+1] == field[y][x])
                return 1;
        }
    }

    for (int x = 0; x < W; ++x) {
        for (int y = H-2; y >= 0; --y) {
            if (field[y][x] != NUM_NONE && field[y+1][x] == field[y][x])
                return 1;
        }
        for (int y = 1; y < H; ++y) {
            if (field[y][x] != NUM_NONE && field[y-1][x] == field[y][x])
                return 1;
        }
    }

    return 0;
}

void gameloop() {
    while (1) {
        draw();

        switch (getch()) {
            case 'q': return;
            case 'h': move(VEC_LEFT);  break;
            case 'j': move(VEC_DOWN);  break;
            case 'k': move(VEC_UP);    break;
            case 'l': move(VEC_RIGHT); break;
            default: continue;
        }

        if (clear) {
            draw();
            printf("%*sCLEAR!!!\n", 9, "");
            return;
        }

        if (!moved) {
            if (can_move()) {
                continue;
            } else {
                printf("%*sGAMEOVER\n", 9, "");
                return;
            }
        }

        if (exist_null_cell()) {
            int xy = get_random_null_cell();
            field[xy&15][xy>>4] = NUM_2;
            moved = 0;
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

    // Init field
    unsigned char xy = get_random_null_cell();
    field[xy&15][xy>>4] = NUM_2;
    xy = get_random_null_cell();
    field[xy&15][xy>>4] = NUM_2;

    printf("\033[2J\033[H");
    gameloop();

    return 0;
}
