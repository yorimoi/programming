/* vim:set foldmethod=marker: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>

typedef int bool;
#define true  1
#define false 0

#define FIELD_WIDTH    8
#define FIELD_HEIGHT   14

#define PUYO_START_X   3
#define PUYO_START_Y   1

#define PUYO_COLOR_MAX 4

enum {
  CELL_NONE,
  CELL_WALL,
  CELL_PUYO_0,
  CELL_PUYO_1,
  CELL_PUYO_2,
  CELL_PUYO_3,
  CELL_MAX
};

enum {
  PUYO_ANGLE_0,
  PUYO_ANGLE_90,
  PUYO_ANGLE_180,
  PUYO_ANGLE_270,
  PUYO_ANGLE_MAX,
};

int puyo_sub_pos[][2] =
{
  { 0,-1}, // PUYO_ANGLE_0
  {-1, 0}, // PUYO_ANGLE_90
  { 0, 1}, // PUYO_ANGLE_180
  { 1, 0}, // PUYO_ANGLE_270
};

int cells[FIELD_HEIGHT][FIELD_WIDTH];
int buffer[FIELD_HEIGHT][FIELD_WIDTH];
int checked[FIELD_HEIGHT][FIELD_WIDTH];

char cell_AA[][12+1] =
{
  "\033[37m .", // CELL_NONE
  "\033[37m #", // CELL_WALL
  "\033[31m O", // CELL_PUYO_0
  "\033[32m O", // CELL_PUYO_1
  "\033[33m O", // CELL_PUYO_2
  "\033[34m O", // CELL_PUYO_3
};

int puyo_x = PUYO_START_X;
int puyo_y = PUYO_START_Y;
int puyo_color;
int puyo_color_sub;
int puyo_angle;

bool lock = false;

int getch() //{{{
{
  char c;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  c = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return c;
} //}}}
int kbhit() //{{{
{
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
} //}}}

void display()
{
  printf("\033[H");
  memcpy(buffer, cells, sizeof buffer);

  if(!lock) {
    int sub_x = puyo_x + puyo_sub_pos[puyo_angle][0];
    int sub_y = puyo_y + puyo_sub_pos[puyo_angle][1];
    buffer[puyo_y][puyo_x] = CELL_PUYO_0 + puyo_color;
    buffer[sub_y][sub_x]   = CELL_PUYO_0 + puyo_color_sub;
  }

  for(int y=1; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++) {
      printf("%s", cell_AA[buffer[y][x]]);
    }
    printf("\n");
  }
}

bool collision_puyo_to_field(int _puyo_x, int _puyo_y, int _puyo_angle)
{
  if(cells[_puyo_y][_puyo_x] != CELL_NONE)
    return true;

  int sub_x = _puyo_x + puyo_sub_pos[_puyo_angle][0];
  int sub_y = _puyo_y + puyo_sub_pos[_puyo_angle][1];

  if(cells[sub_y][sub_x] != CELL_NONE)
    return true;

  return false;
}

int get_connected_puyo_cnt(int _x, int _y, int _cell, int _cnt)
{
  if(checked[_y][_x] || (cells[_y][_x] != _cell))
    return _cnt;

  _cnt++;
  checked[_y][_x] = true;

  for(int i=0; i<PUYO_ANGLE_MAX; i++) {
    int x = _x + puyo_sub_pos[i][0];
    int y = _y + puyo_sub_pos[i][1];
    _cnt = get_connected_puyo_cnt(x, y, _cell, _cnt);
  }

  return _cnt;
}

void erase_puyo(int _x, int _y, int _cell)
{
  if(cells[_y][_x] != _cell)
    return;

  cells[_y][_x] = CELL_NONE;

  for(int i=0; i<PUYO_ANGLE_MAX; i++) {
    int x = _x + puyo_sub_pos[i][0];
    int y = _y + puyo_sub_pos[i][1];
    erase_puyo(x, y, _cell);
  }
}

int main()
{
  bool quit = false;
  srand((unsigned int)time(NULL));
  time_t t = 0;

  for(int y=0; y<FIELD_HEIGHT; y++)
    cells[y][0] = cells[y][FIELD_WIDTH-1] = CELL_WALL;
  for(int x=0; x<FIELD_WIDTH; x++)
    cells[FIELD_HEIGHT-1][x] = CELL_WALL;

  puyo_color     = rand() % PUYO_COLOR_MAX;
  puyo_color_sub = rand() % PUYO_COLOR_MAX;

  printf("\033[2J\033[H\033[?25l");

  while(!quit) {
    if(t < time(NULL)) {
      t = time(NULL);
      if(!lock) {
        if(!collision_puyo_to_field(puyo_x, puyo_y+1, puyo_angle)) {
          puyo_y++;
        } else {
          int sub_x = puyo_x + puyo_sub_pos[puyo_angle][0];
          int sub_y = puyo_y + puyo_sub_pos[puyo_angle][1];

          cells[puyo_y][puyo_x] = CELL_PUYO_0 + puyo_color;
          cells[sub_y][sub_x]   = CELL_PUYO_0 + puyo_color_sub;

          puyo_x     = PUYO_START_X;
          puyo_y     = PUYO_START_Y;
          puyo_angle = PUYO_ANGLE_0;
          puyo_color     = rand() % PUYO_COLOR_MAX;
          puyo_color_sub = rand() % PUYO_COLOR_MAX;
          lock = true;
        }
      }

      if(lock) {
        lock = false;
        for(int y=FIELD_HEIGHT-3; y>=0; y--)
          for(int x=1; x<FIELD_WIDTH-1; x++)
            if((cells[y][x] != CELL_NONE) && (cells[y+1][x] == CELL_NONE)) {
              cells[y+1][x] = cells[y][x];
              cells[y][x]   = CELL_NONE;
              lock = true;
            }
        if(!lock) {
          memset(checked, 0, sizeof checked);
          for(int y=0; y<FIELD_HEIGHT-2; y++)
            for(int x=1; x<FIELD_WIDTH-2; x++) {
              if(cells[y][x] != CELL_NONE)
                if(get_connected_puyo_cnt(x, y, cells[y][x], 0) >= 4) {
                  erase_puyo(x, y, cells[y][x]);
                  lock = true;
                }
            }
        }
      }

      display();
    }

    if(kbhit()) {
      if(lock) {
        getch();
        continue;
      }
      int x = puyo_x;
      int y = puyo_y;
      int a = puyo_angle;
      switch(getch()) {
        case 'h': x--; break;
        case 'j': y++; break;
        case 'l': x++; break;
        case ' ': a = (a + 1) % PUYO_ANGLE_MAX; break;
        case 'q': quit = true; break;
        default: continue;
      }
      if(!collision_puyo_to_field(x, y, a)) {
        puyo_x     = x;
        puyo_y     = y;
        puyo_angle = a;
      }
      display();
    }
  }

  printf("\033[?25h");

  return 0;
}
