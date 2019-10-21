/*
 * Windows: gcc
 * POSIX:   gcc
 */

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
            exit(1);
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

// ifdef no setumeiji ni enum he hennkou
//#define WIDTH  10 + 2
//#define HEIGHT 20 + 1
enum {
    WIDTH  = 10 + 2, // Field width
    HEIGHT = 20 + 4, // Field height
    MT_MAX = 7,      // Mino Type Max
};

// Mino Type
enum {
    MT_NONE,
    MT_WALL,
    MT_I,
    MT_O,
    MT_S,
    MT_Z,
    MT_J,
    MT_L,
    MT_T,
};

// global hennsuu ha 0 de syokika sareru
int field[HEIGHT][WIDTH];
int field_buf[HEIGHT][WIDTH];

int minos[MT_MAX][4][4] = {
    // I
    {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 2, 2, 2, 2 },
        { 0, 0, 0, 0 },
    },
    // O
    {
        { 0, 0, 0, 0 },
        { 0, 3, 3, 0 },
        { 0, 3, 3, 0 },
        { 0, 0, 0, 0 },
    },
    // S
    {
        { 0, 0, 0, 0 },
        { 0, 4, 4, 0 },
        { 4, 4, 0, 0 },
        { 0, 0, 0, 0 },
    },
    // Z
    {
        { 0, 0, 0, 0 },
        { 5, 5, 0, 0 },
        { 0, 5, 5, 0 },
        { 0, 0, 0, 0 },
    },
    // J
    {
        { 0, 0, 0, 0 },
        { 6, 0, 0, 0 },
        { 6, 6, 6, 0 },
        { 0, 0, 0, 0 },
    },
    // L
    {
        { 0, 0, 0, 0 },
        { 0, 0, 7, 0 },
        { 7, 7, 7, 0 },
        { 0, 0, 0, 0 },
    },
    // T
    {
        { 0, 0, 0, 0 },
        { 0, 8, 0, 0 },
        { 8, 8, 8, 0 },
        { 0, 0, 0, 0 },
    },
};

char *block_colors[] = {
    "",
    "",
    "\033[41m",
    "\033[42m",
    "\033[43m",
    "\033[44m",
    "\033[45m",
    "\033[46m",
    "\033[47m",
};

int mino_x, mino_y, mino_shape[4][4];

void display(void) {
    memcpy(field_buf, field, sizeof(field));
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (mino_shape[y][x]) {
                field_buf[y+mino_y][x+mino_x] = mino_shape[y][x];
            }
        }
    }

    printf("\033[H");
    for (int y = 3; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (field_buf[y][x] == MT_NONE) {
                printf(" .");
            } else {
                printf("%s[]\033[0m", block_colors[field_buf[y][x]]);
            }
        }
        printf("\n");
    }
}

int is_move(int mx, int my, int ms[][4]) {
    int ret = 1;
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (field[mino_y+y+my][mino_x+x+mx] && ms[y][x]) {
                ret = 0;
            }
        }
    }
    return ret;
}

void rotation(void) {
    int ms_buf[4][4];
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            ms_buf[y][x] = mino_shape[4 - 1 - x][y];
        }
    }
    if (is_move( 0, 0, ms_buf)) {
        memcpy(mino_shape, ms_buf, sizeof(mino_shape));
    }
}

void init(void) {
    mino_x = 4;
    mino_y = 0;
    memcpy(mino_shape, minos[rng() % MT_MAX], sizeof(mino_shape));
}

void gameloop(void) {
    time_t t = 0;
    while (1) {
        if (t != time(NULL)) {
            t = time(NULL);
            if (is_move( 0, 1, mino_shape)) {
                ++mino_y;
            } else {
                // Fix to field
                memcpy(field, field_buf, sizeof(field));

                // Delete line
                for (int y = HEIGHT - 2; y > 3; --y) {
                    int can_delete = 1;
                    for (int x = 1; x < WIDTH - 1; ++x) {
                        if(field[y][x] == MT_NONE) {
                            can_delete = 0;
                            break;
                        }
                    }
                    if (can_delete) {
                        for (int z = y; z > 3; --z) {
                            memcpy(field[y], field[y - 1], sizeof(field[y]));
                        }
                        //for(int y2=y; y2>3; y2--)
                        //    for(int x2=1; x2<WIDTH-1; x2++)
                        //        field[y2][x2] = field[y2-1][x2];
                    }
                }
        //for(int y=0; y<HEIGHT-1; y++) {
        //  int line_erase = 1;
        //  for(int x=1; x<WIDTH-1; x++)
        //    if(field[y][x] == MT_NONE)
        //      line_erase = 0;
        //  if(line_erase) {
        //    for(int y2=y; y2>0; y2--)
        //      for(int x2=1; x2<WIDTH-1; x2++)
        //        field[y2][x2] = field[y2-1][x2];
        //  }
        //}

                init();
            }

            display();
        }

        if (kbhit()) {
            switch (getch()) {
                case 'q': return;
                case 'h': if (is_move(-1,  0, mino_shape)) --mino_x; break;
                case 'j': if (is_move( 0,  1, mino_shape)) ++mino_y; break;
                case 'l': if (is_move( 1,  0, mino_shape)) ++mino_x; break;
                case ' ': rotation(); break;
                case 'k': while (is_move( 0, 1, mino_shape)) ++mino_y; t = 0;
            }
            display();
        }
    }
}

int main(void) {

// Random
#ifdef _WIN32
// long int rng();
#else
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
        fprintf(stderr, "error: timespec_get()\n");
        return 1;
    }
    srandom(ts.tv_nsec ^ ts.tv_sec);
#endif


    for (int y = 0; y < HEIGHT; ++y) {
        field[y][0] = field[y][WIDTH - 1] = MT_WALL;
    }
    for (int x = 0; x < WIDTH; ++x) {
        field[HEIGHT - 1][x] = MT_WALL;
    }

    init();

    printf("\033[2J\033[H");

    gameloop();

    return 0;
}
