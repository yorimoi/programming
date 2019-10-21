#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 25

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

enum {
    MINO_ANGLE_0,
    MINO_ANGLE_90,
    MINO_ANGLE_180,
    MINO_ANGLE_270,
    MINO_ANGLE_MAX,
};

int field[FIELD_HEIGHT][FIELD_WIDTH];
int field_buf[FIELD_HEIGHT][FIELD_WIDTH];

const int minos[MINO_TYPE_MAX][4][4] = {
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
    },
    {
        {0,0,0,0},
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
    },
    {
        {0,0,0,0},
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
    },
    {
        {0,0,0,0},
        {1,1,1,0},
        {0,0,1,0},
        {0,0,0,0},
    },
    {
        {0,0,0,0},
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
    },
    {
        {0,0,0,0},
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
    },
};

struct Data {
    int mino_x;
    int mino_y;
    int mino_angle;
    int mino_type;
};

int getch() {
  char ch;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  ch = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return ch;
}

int kbhit() {
  int ch, of;
  struct termios ot, nt;
  tcgetattr(0, &ot);
  nt = ot;
  nt.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &nt);
  of = fcntl(0, F_GETFL, 0);
  fcntl(0, F_SETFL, of | O_NONBLOCK);
  ch = getchar();
  tcsetattr(0, TCSANOW, &ot);
  fcntl(0, F_SETFL, of);
  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}

void reset(struct Data *d) {
    d->mino_x = 4;
    d->mino_angle = MINO_ANGLE_0;
    d->mino_type = MINO_TYPE_I;
    switch(d->mino_type) {
        case MINO_TYPE_I: d->mino_y = 2; break;
    }
}

void quit() {
    printf("\033[?25h");
    exit(0);
}

void draw(struct Data *d) {
    printf("\033[H");
    memcpy(field_buf, field, sizeof field);

    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            if(minos[d->mino_type][i][j])
                field_buf[i+d->mino_y][j+d->mino_x] = 1;

    for(int y=0; y<FIELD_HEIGHT; y++) {
        for(int x=0; x<FIELD_WIDTH; x++) {
            if(field_buf[y][x])
                printf(" #");
            else
                printf(" .");
        }
        printf("\n");
    }
}

// 秋田
int cd(int x, int y, int type, int angle) {
    rotate(type, angle);
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
}

int main() {

    srand((unsigned int)time(NULL));

    for(int y=0; y<FIELD_HEIGHT; y++)
        field[y][0] = field[y][FIELD_WIDTH-1] = 1;
    for(int x=1; x<FIELD_WIDTH-1; x++)
        field[FIELD_HEIGHT-1][x] = 1;

    struct Data data;
    reset(&data);

    time_t t = 0;

    printf("\033[2J\033[H\033[?25l");

    while(1) {
        if(t != time(NULL)) {
            t = time(NULL);

            draw(&data);
            data.mino_y++;
        }

        if(kbhit()) {
            switch(getch()) {
                case 'h': data.mino_x--; break;
                case 'j': data.mino_y++; break;
                case 'k': data.mino_y--; break;
                case 'l': data.mino_x++; break;
                case 'q': quit();
            }
            draw(&data);
        }
    }

    return 0;
}
