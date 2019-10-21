#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

enum FIELD_SIZE {
    F_WIDTH  = 6 + 1 + 1,
    F_HEIGHT = 12 + 1 + 2,
};

enum PUYO {
    PY_NONE,
    PY_WALL,
    PY_RED,
    PY_GREEN,
    PY_YELLOW,
    PY_BLUE,
    PY_PURPLE,
    PY_DISAPPEAR,
    PY_MAX,
    PY_TYPE_MAX = 4,
    PY_COLOR_MAX = 5,

    PY_START_X = 3,
    PY_START_Y = 1,
};

enum ANGLE {
    AG_0,
    AG_90,
    AG_180,
    AG_270,
    AG_MAX,
};

enum SLEEP {
    SL_FALL = 75,
    SL_POP  = 500,
};

int puyo_sub_pos[][2] = {
    { 0,-1},  // AG_0
    { 1, 0},  // AG_90
    { 0, 1},  // AG_180
    {-1, 0},  // AG_270
};

char puyo_colors[PY_MAX][5+3+1] = {
    "\033[0m  ",  // PY_NONE
    "\033[0m■",   // PY_WALL
    "\033[31m●",  // PY_RED
    "\033[32m●",  // PY_GREEN
    "\033[33m●",  // PY_YELLOW
    "\033[34m●",  // PY_BLUE
    "\033[35m●",  // PY_PURPLE
    "\033[36m※", // PY_DISAPPEAR
};

int field[F_HEIGHT][F_WIDTH],
    f_buf[F_HEIGHT][F_WIDTH],
    checked[F_HEIGHT][F_WIDTH];

int puyo_x = PY_START_X,
    puyo_y = PY_START_Y,
    puyo_color[2],
    puyo_nexts[2][2],
    puyo_angle;

int *puyo_types;

int chain,
    dc,    // difference color
    score;

time_t t;

// 連鎖数ボーナス (1連鎖~)
int chain_bonus[20] = {
    0, 8, 16, 32, 64, 96, 128, 160, 192, 224,
    256, 288, 320, 352, 384, 416, 448, 480, 512
};

// 連結数ボーナス (4~11以上)
int link_bonus[8] = {
    0, 2, 3, 4, 5, 6, 7, 10
};

// 色数ボーナス
int color_bonus[5] = {
    0, 3, 6, 12, 24
};

void draw() {
    memcpy(f_buf, field, sizeof f_buf);
    int sub_x = puyo_x + puyo_sub_pos[puyo_angle][0],
        sub_y = puyo_y + puyo_sub_pos[puyo_angle][1];
    f_buf[puyo_y][puyo_x] = PY_RED + puyo_color[0];
    f_buf[sub_y][sub_x]   = PY_RED + puyo_color[1];

    // Next puyo
    printf("\033[1;%dH%s", F_WIDTH*2+2, puyo_colors[PY_RED+puyo_nexts[0][1]]);
    printf("\033[2;%dH%s", F_WIDTH*2+2, puyo_colors[PY_RED+puyo_nexts[0][0]]);
    printf("\033[3;%dH%s", F_WIDTH*2+4, puyo_colors[PY_RED+puyo_nexts[1][1]]);
    printf("\033[4;%dH%s", F_WIDTH*2+4, puyo_colors[PY_RED+puyo_nexts[1][0]]);

    if(chain > 0)
        printf("\033[7;%dH\033[0m%d連鎖!", F_WIDTH*2+3, chain);
    else
        printf("\033[7;%dH\033[0m       ", F_WIDTH*2+3);

    printf("\033[H");
    for(int y=2; y<F_HEIGHT; y++) {
        for(int x=0; x<F_WIDTH; x++) {
            if(y == 2 && x == 3 && f_buf[y][x] < 2)
                printf("\033[31m×");
            else
                printf("%s", puyo_colors[f_buf[y][x]]);
        }
        printf("\n");
    }

    printf("SCORE: %08d", score);
}

int cd(int _x, int _y, int _a) {
    if(field[_y][_x] != PY_NONE)
        return 1;

    int sub_x = _x + puyo_sub_pos[_a][0],
        sub_y = _y + puyo_sub_pos[_a][1];
    if(field[sub_y][sub_x] != PY_NONE)
        return 1;

    return 0;
}

int get_connected_cnt(int _x, int _y, int _color, int _cnt) {
    if((checked[_y][_x])
    || (field[_y][_x] != _color))
        return _cnt;

    _cnt++;
    checked[_y][_x] = 1;

    for(int i=0; i<AG_MAX; i++) {
        int x = _x + puyo_sub_pos[i][0],
            y = _y + puyo_sub_pos[i][1];
        _cnt = get_connected_cnt(x, y, _color, _cnt);
    }
}

int _pow(int x, int y) {
    if(y==0) return 1;
    int z = x;
    for(int i=1; i<y; i++)
        z *= x;
    return z;
}

void erase_disappear() {
    for(int y=0; y<F_HEIGHT-1; y++)
        for(int x=1; x<F_WIDTH-1; x++)
            if(field[y][x] == PY_DISAPPEAR)
                field[y][x] = PY_NONE;
}

void erase_puyos(int _x, int _y, int _color) {
    if(field[_y][_x] != _color)
        return;

    field[_y][_x] = PY_DISAPPEAR;

    for(int i=0; i<AG_MAX; i++) {
        int x = _x + puyo_sub_pos[i][0],
            y = _y + puyo_sub_pos[i][1];
        erase_puyos(x, y, _color);
    }

    dc |= _pow(2, _color);
}

int score_cal(int _chain, int _link, int _dc) {
    int dc_cnt = 0;
    for(int i=0; i<PY_COLOR_MAX; i++)
        if(_dc>>i & 1) dc_cnt++;
    return _link * (10 * (chain_bonus[_chain-1] + link_bonus[_link-4] + color_bonus[dc_cnt]));
}

void quit() {
    printf("\033[?25h");
    exit(0);
}

void gameover() {
    char bq[][3+1] = { "ば","た","ん","き","ゅ","ー" };
    for(int i=0; i<6; i++) {
        printf("\033[%d;%dH\033[36m%s", 4+i/3, F_WIDTH-2+i%3*2, bq[i]);
        Sleep(SL_FALL);
    }
    printf("\033[%dH", F_HEIGHT);
    quit();
}

int is_choking() {
    if(field[2][3] != PY_NONE)
        return 1;
    return 0;
}

int is_perfect() {
    for(int x=1; x<F_WIDTH-1; x++)
        if(field[F_HEIGHT-2][x] != PY_NONE)
            return 0;
    return 1;
}

void draw_perfect() {
    //char perfect[] = "Perfect!!";
    char perfect[] = "ALLCLEAR";
    for(int i=0; i<sizeof perfect; i++) {
        printf("\033[8;%dH\033[0m%c", F_WIDTH*2+3+i, perfect[i]);
        Sleep(20);
    }
    Sleep(300);
    printf("\033[8;%dH         ", F_WIDTH*2+3);
}


void key_operation() {
    int x = puyo_x,
        y = puyo_y,
        a = puyo_angle,
        j = 0;

    switch(getch()) {
        case 'h': x--; break;
        case 'j': y++; j=1; break;
        //case 'k': y--; break;
        case 'l': x++; break;
        case 'f': a = (a + 1) % AG_MAX; break;
        case 'd': a = (AG_MAX + a - 1) % AG_MAX; break;
        case 'q': quit(); break;
    }

    if(!cd(x, y, a)) {
        puyo_x     = x;
        puyo_y     = y;
        puyo_angle = a;
    }
    else if(j)
        t = 0;

    draw();
}

void puyo_init(int *_colors) {
    _colors[0] = puyo_types[rand() % PY_TYPE_MAX];
    _colors[1] = puyo_types[rand() % PY_TYPE_MAX];
}

void puyo_next() {
    memcpy(puyo_color, puyo_nexts[0], sizeof puyo_color);
    memcpy(puyo_nexts[0], puyo_nexts[1], sizeof puyo_color);
    puyo_init(puyo_nexts[1]);
}

void field_init() {
    for(int y=0; y<F_HEIGHT; y++)
        field[y][0] = field[y][F_WIDTH-1] = PY_WALL;
    for(int x=1; x<F_WIDTH-1; x++)
        field[F_HEIGHT-1][x] = PY_WALL;
    puyo_init(puyo_nexts[0]);
    puyo_init(puyo_nexts[1]);
    puyo_next();
    printf("\033[2J\033[H\033[?25l");
}

int *types_init(int *types, int size) {
    int typemax[PY_COLOR_MAX];
    types = malloc(sizeof(int *) * size);
    for(int i=0; i<PY_COLOR_MAX; i++)
        typemax[i] = i;
    for(int i=0; i<PY_COLOR_MAX; i++) {
        int j = rand() % PY_COLOR_MAX;
        int t = typemax[i];
        typemax[i] = typemax[j];
        typemax[j] = t;
    }
    for(int i=0; i<size; i++)
        types[i] = typemax[i];

    return types;
}

void game_loop() {
    int lock = 0;

    while(1) {
        if(t != time(NULL)) {
            t = time(NULL);

            if(!lock) {
                if(!cd(puyo_x, puyo_y+1, puyo_angle))
                    puyo_y++;
                else {
                    memcpy(field, f_buf, sizeof field);
                    puyo_x     = PY_START_X;
                    puyo_y     = PY_START_Y;
                    puyo_angle = AG_0;
                    puyo_next();
                    t = 0;
                    lock = 1;
                    chain = 0;
                    dc = 0;
                }
            }
            draw();
        }

        if(lock) {
            do {
                Sleep(SL_FALL);
                lock = 0;
                for(int y=F_HEIGHT-3; y>=0; y--) {
                    for(int x=1; x<F_WIDTH-1; x++)
                        if((field[y][x] != PY_NONE)
                        && (field[y+1][x] == PY_NONE)) {
                            field[y+1][x] = field[y][x];
                            field[y][x]   = PY_NONE;
                            lock = 1;
                        }
                }
                draw();
            } while(lock);
            memset(checked, 0, sizeof checked);
            int cc; // connected count
            for(int y=0; y<F_HEIGHT-1; y++)
                for(int x=1; x<F_WIDTH-1; x++) {
                    if(field[y][x] != PY_NONE)
                        if((cc = get_connected_cnt(x, y, field[y][x], 0)) >= 4) {
                            erase_puyos(x, y, field[y][x]);
                            score += score_cal(chain, cc, dc);
                            lock = 1;
                        }
                }
            if(lock) {
                chain++;
                draw();
                Sleep(SL_POP);
                erase_disappear();
            }
            if(!lock) {
                if(is_choking())
                    gameover();
                if(is_perfect())
                    draw_perfect();
            }
        }

        if(kbhit()) {
            if(lock) getch();
            else key_operation();
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));
    puyo_types = types_init(puyo_types, PY_TYPE_MAX);
    field_init();
    game_loop();

    return 0;
}
