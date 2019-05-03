/* For Windows */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

enum FIELD_SIZE {
    F_WIDTH   = 10 + 2 + 2 + 2,  // main_field + wall + l_margin + r_margin
    F_HEIGHT  = 20 + 1 + 4 + 2,  // main_field + wall + u_margin + d_margin
};

enum MINO_TYPES {
    MT_I,
    MT_O,
    MT_S,
    MT_Z,
    MT_J,
    MT_L,
    MT_T,
    MT_MAX,
};

enum MINO_ANGLES {
    MA_0,
    MA_90,
    MA_180,
    MA_270,
    MA_MAX,
};

enum MINO_SIZE {
    M_WIDTH  = 4,
    M_HEIGHT = 4,
};

int mino_types[MT_MAX][MA_MAX][M_HEIGHT][M_WIDTH] = {
    // MT_I
    {
        // MA_0
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0},
        },
        // MA_90
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
        },
        // MA_180
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
        },
        // MA_270
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
        },
    },
    // MT_O
    {
        // MA_0
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
        },
        // MA_90
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
        },
        // MA_180
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
        },
        // MA_270
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
        },
    },
    // MT_S
    {
        // MA_0
        {
            {0,0,0,0},
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
        },
        // MA_90
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
        },
        // MA_180
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,1,1,0},
            {1,1,0,0},
        },
        // MA_270
        {
            {0,0,0,0},
            {1,0,0,0},
            {1,1,0,0},
            {0,1,0,0},
        },
    },
    // MT_Z
    {
        // MA_0
        {
            {0,0,0,0},
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
        },
        // MA_90
        {
            {0,0,0,0},
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
        },
        // MA_180
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,0,0},
            {0,1,1,0},
        },
        // MA_270
        {
            {0,0,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {1,0,0,0},
        },
    },
    // MT_J
    {
        // MA_0
        {
            {0,0,0,0},
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0},
        },
        // MA_90
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,1,0,0},
        },
        // MA_180
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,0},
            {0,0,1,0},
        },
        // MA_270
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {1,1,0,0},
        },
    },
    // MT_L
    {
        // MA_0
        {
            {0,0,0,0},
            {0,0,1,0},
            {1,1,1,0},
            {0,0,0,0},
        },
        // MA_90
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,1,0},
        },
        // MA_180
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,0},
            {1,0,0,0},
        },
        // MA_270
        {
            {0,0,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
        },
    },
    // MT_T
    {
        // MA_0
        {
            {0,0,0,0},
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
        },
        // MA_90
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,1,0,0},
        },
        // MA_180
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,0},
            {0,1,0,0},
        },
        // MA_270
        {
            {0,0,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {0,1,0,0},
        },
    },
};

int field[F_HEIGHT][F_WIDTH];
int f_buf[F_HEIGHT][F_WIDTH];

int mino_x = 3 + 3,
    mino_y,
    mino_type,
    mino_angle;

int mino_stacks[2][7] = {{0,1,2,3,4,5,6}, {0,1,2,3,4,5,6}},
    mino_nexts[7];

int score;

int hold = -1;

void draw() {
    memcpy(f_buf, field, sizeof field);
    for(int i=0; i<M_HEIGHT; i++)
        for(int j=0; j<M_WIDTH; j++)
            f_buf[i+mino_y][j+mino_x]
            |= mino_types[mino_type][mino_angle][i][j];

    //system("cls");
    printf("\033[1;%dHSCORE: %d", (F_WIDTH-4)*2+2, score);
    printf("\033[3;%dHHOLD: ", (F_WIDTH-4)*2+2);
        if(hold != -1) {
            for(int i=1; i<M_HEIGHT-1; i++) {
                printf("\033[%d;%dH", 4+i, (F_WIDTH-4)*2+3);
                for(int j=0; j<M_WIDTH; j++)
                    printf(mino_types[hold][MA_0][i][j]?"■":"  ");
            }
        }
    printf("\033[8;%dHNEXT: ", (F_WIDTH-4)*2+2);
        for(int k=0; k<5; k++) {
            for(int i=1; i<M_HEIGHT-1; i++) {
                printf("\033[%d;%dH", 9+i+k*3, (F_WIDTH-4)*2+3);
                for(int j=0; j<M_WIDTH; j++) {
                    printf(mino_types[mino_nexts[k]][MA_0][i][j]?"■":"  ");
                }
            }
        }

    printf("\033[H");
    for(int y=2; y<4; y++) {
        printf("・");
        for(int x=3; x<F_WIDTH-3; x++)
            printf(f_buf[y][x]?"■":"・");
        printf("・\n");
    }
    for(int y=4; y<F_HEIGHT-2; y++) {
        for(int x=2; x<F_WIDTH-2; x++)
            printf(f_buf[y][x]?"■":"・");
        printf("\n");
    }
}

/* Collision Detection */
int cd(int _x, int _y, int _a) {
    for(int i=0; i<M_HEIGHT; i++)
        for(int j=0; j<M_WIDTH; j++)
            if(field[_y+i][_x+j] & mino_types[mino_type][_a][i][j])
                return 1;
    return 0;
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a      = *b;
    *b      = tmp;
}

void init_minos(int *nexts) {
    for(int i=7-1; i>0; i--) {
        int mix = rand() % i;
        int tmp = *(nexts+i);
        *(nexts+i) = *(nexts+mix);
        *(nexts+mix) = tmp;
    }
}

void mino_reset() {
    static int next_cnt = -1;
    if(7 <= ++next_cnt) {
        next_cnt = 0;
        memcpy(mino_stacks[0], mino_stacks[1], sizeof mino_stacks[0]);
        init_minos(mino_stacks[1]);
    }
    mino_type  = mino_stacks[0][next_cnt];
    mino_angle = 0;
    mino_x     = 3 + 3;
    mino_y     = mino_type==MT_I ? 2 : 1;
    for(int i=1, j=0; i<=7; i++)
        mino_nexts[i-1]
        = next_cnt+i < 7 ? mino_stacks[0][next_cnt+i] : mino_stacks[1][j++];
}

void quit() {
    printf("\033[?25h");
    exit(0);
}

void gameover() {
    printf(" Game Over!\n");
    quit();
}

int main() {
    srand((unsigned int)time(NULL));

    memset(field, 1, sizeof field);
    for(int y=0; y<F_HEIGHT-3; y++)
        for(int x=3; x<F_WIDTH-3; x++)
            field[y][x] = 0;

    init_minos(mino_stacks[0]);
    init_minos(mino_stacks[1]);
    mino_reset();
    time_t t = 0;
    int can_hold = 1;
    printf("\033[2J\033[H\033[?25l");

    while(1) {
        if(t != time(NULL)) {
            t = time(NULL);

            draw();

            if(!cd(mino_x, mino_y+1, mino_angle))
                mino_y++;
            else {
                t = 0;
                can_hold = 1;
                memcpy(field, f_buf, sizeof f_buf);

                int cnt = 0;
                for(int i=0; i<M_HEIGHT; i++)
                    for(int j=0; j<M_WIDTH; j++)
                        if((field[i][j+mino_x]
                        & mino_types[mino_type][mino_angle][i][j])
                        && (mino_y+i<M_HEIGHT))
                            cnt++;
                if(cnt == 4)
                    gameover();

                int line = 0;
                for(int i=4; i<F_HEIGHT-3; i++) {
                    int erase = 1;
                    for(int j=1; j<F_WIDTH-1; j++)
                        if(!field[i][j]) erase = 0;
                    if(erase) {
                        for(int k=i; k>=4; k--)
                            memcpy(field[k], field[k-1], sizeof field[k]);
                        line++;
                    }
                }
                switch(line) {
                    case 1: score += 40;   break;
                    case 2: score += 100;  break;
                    case 3: score += 300;  break;
                    case 4: score += 1200; break;
                }

                mino_reset();

                for(int i=0; i<M_HEIGHT; i++)
                    for(int j=0; j<M_WIDTH; j++)
                        if(field[i+mino_y][j+mino_x]
                        & mino_types[mino_type][mino_angle][i][j])
                            gameover();
            }
        }

        if(kbhit()) {
            int _x = mino_x,
                _y = mino_y,
                _a = mino_angle;
            switch(getch()) {
                case 'h': _x--; break;
                case 'j': _y++; break;
                case 'k': while(!cd(_x, _y+1, _a)) _y++; t = 0; break;
                case 'l': _x++; break;
                case 'f': _a = (_a+1) % MA_MAX; break;
                case 'd': _a = (_a-1+MA_MAX) % MA_MAX; break;
                case ' ':
                    if(hold == -1) hold = mino_type, mino_reset();
                    if(can_hold) can_hold = 0, swap(&hold, &mino_type);
                    break;
                case 'q': quit();
            }
            if(!cd(_x, _y, _a)) {
                mino_x     = _x;
                mino_y     = _y;
                mino_angle = _a;
            }
            draw();
        }
    }

    return 0;
}
