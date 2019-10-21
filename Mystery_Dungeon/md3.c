#include <stdio.h>
#include <time.h>
#include <termios.h>

#define FIELD_WIDTH 32
#define FIELD_HEIGHT 32
#define AREA_MAX 16
#define AREA_SIZE_MIN 8

typedef struct {
    int x, y, w, h, i;
} AREA;
AREA areas[AREA_MAX];

int area_cnt = 1;

int getch(void) {
    char c; struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(0, TCSANOW, &t);
    c = getchar();
    tcsetattr(0, TCSADRAIN, &t);

    return c;
}

unsigned short rand(int seed) {
    static int rand;
    if (seed) {
        rand = seed;
    }
    rand = rand * 1234567 + 1221;
    rand ^= (rand >> (rand % 16)) & 1;

    return (unsigned short) rand;
}

void draw() {
    int buf[FIELD_HEIGHT][FIELD_WIDTH];

    for (int z = 0; z < area_cnt; ++z) {
        for (int y = areas[z].y; y < areas[z].y + areas[z].h; ++y) {
            for (int x = areas[z].x; x < areas[z].x + areas[z].w; ++x) {
                buf[y][x] = z;
            }
        }
    }

    printf("\033[H");
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            printf("\033[38;5;%dm%2d\033[0m", buf[y][x]+1, buf[y][x]);
        }
        printf("\n");
    }
}

void split_area(int idx) {
    int new_idx = area_cnt;
    int old_w = areas[idx].w,
        old_h = areas[idx].h;

    if (new_idx >= AREA_MAX) {
        return;
    }

    if (rand(0) % 2) {
        areas[new_idx].x = areas[idx].x + areas[idx].w / 2;
        areas[new_idx].y = areas[idx].y;
        areas[new_idx].w = areas[idx].w / 2;
        areas[new_idx].h = areas[idx].h;
        areas[new_idx].i = new_idx;
        areas[idx].w /= 2;
    } else {
        areas[new_idx].x = areas[idx].x;
        areas[new_idx].y = areas[idx].y + areas[idx].h / 2;
        areas[new_idx].w = areas[idx].w;
        areas[new_idx].h = areas[idx].h / 2;
        areas[new_idx].i = new_idx;
        areas[idx].h /= 2;
    }

    if (areas[idx].w < AREA_SIZE_MIN || areas[idx].h < AREA_SIZE_MIN) {
        areas[idx].w = old_w;
        areas[idx].h = old_h;
        return;
    }

    ++area_cnt;

    draw();
    getch();

    split_area(idx);
    split_area(new_idx);
}

int main(void) {

    rand(time(NULL));
    areas[0].w = FIELD_WIDTH;
    areas[0].h = FIELD_HEIGHT;

    printf("\033[2J");
    draw();
    getch();
    split_area(0);

    return 0;
}

