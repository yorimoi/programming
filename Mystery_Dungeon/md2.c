#include <stdio.h>
#include <termios.h>

#define FIELD_WIDTH 64
#define FIELD_HEIGHT 64
#define AREA_MAX 64
#define AREA_SIZE_MIN 2

int getch(void);
void display_area(void);
void split_area(int idx);

typedef struct {
    int x, y, w, h;
} AREA;
AREA areas[AREA_MAX];

int area_cnt;

int main(void) {

    area_cnt   = 0;
    areas[0].x = 0;
    areas[0].y = 0;
    areas[0].w = FIELD_WIDTH;
    areas[0].h = FIELD_HEIGHT;

    ++area_cnt;

    printf("\033[2J\033[H\033[?25l");

    display_area();
    getch();

    split_area(0);
    display_area();

    getch();

    printf("\033[?25h");

    return 0;
}

int getch(void) {
    char c;
    struct termios t;

    tcgetattr(0, &t);
    t.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(0, TCSANOW, &t);
    c = getchar();
    tcsetattr(0, TCSADRAIN, &t);

    return c;
}

void display_area(void) {
    int buf[FIELD_HEIGHT][FIELD_WIDTH];

    for (int i = 0; i < area_cnt; ++i) {
        for (int y = areas[i].y; y < areas[i].y + areas[i].h; ++y) {
            for (int x = areas[i].x; x < areas[i].x + areas[i].w; ++x) {
                buf[y][x] = i;
            }
        }
    }

    printf("\033[H");
    for (int y = 0; y < FIELD_HEIGHT; ++y) {
        for (int x = 0; x < FIELD_WIDTH; ++x) {
            printf("\033[38;5;%dm%2d\033[0m", buf[y][x]+1, buf[y][x]>99?buf[y][x]/10:buf[y][x]);
        }
        printf("\n");
    }

}

void split_area(int idx) {
    int new_idx = area_cnt;
    int w = areas[idx].w;
    int h = areas[idx].h;

    if (area_cnt % 2) {
        areas[idx].w /= 2;
        areas[new_idx].x = areas[idx].x + areas[idx].w;
        areas[new_idx].y = areas[idx].y;
        areas[new_idx].w = w - areas[idx].w;
        areas[new_idx].h = areas[idx].h;
    } else {
        areas[idx].h /= 2;
        areas[new_idx].x = areas[idx].x;
        areas[new_idx].y = areas[idx].y + areas[idx].h;
        areas[new_idx].w = areas[idx].w;
        areas[new_idx].h = h - areas[idx].h;
    }

    if (areas[idx].w < AREA_SIZE_MIN || areas[idx].h < AREA_SIZE_MIN
            || areas[new_idx].w < AREA_SIZE_MIN || areas[new_idx].h < AREA_SIZE_MIN) {
        areas[idx].w = w;
        areas[idx].h = h;
        return;
    }

    ++area_cnt;

    display_area();
    getch();

    split_area(idx);
    split_area(new_idx);
}

