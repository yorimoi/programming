/* ボンバーマンを小一時間で作ってみた */
/* https://youtu.be/NlY1t4xlfdo */

/* [1:34:00]まで */
/* 飽きてきた */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>

#define MAP_WIDTH    31
#define MAP_HEIGHT   13

#define SCREEN_WIDTH 16

#define MONSTER_MAX  9

#define BOMB_MAX     10
#define BOMB_CNT_MAX 10

enum {
  DIRECTION_NORTH,
  DIRECTION_WEST,
  DIRECTION_SOUTH,
  DIRECTION_EAST,
  DIRECTION_MAX
};

int directions[][2] =
{
  { 0,-1}, // DIRECTION_NORTH
  {-1, 0}, // DIRECTION_WEST
  { 0, 1}, // DIRECTION_SOUTH
  { 1, 0}, // DIRECTION_EAST
};

enum {
  CELL_TYPE_NONE,
  CELL_TYPE_HARD_BLOCK,
  CELL_TYPE_SOFT_BLOCK,
  CELL_TYPE_EXPLOSION,
  CELL_TYPE_EXIT,
  CELL_TYPE_MAX
};

int cells[MAP_HEIGHT][MAP_WIDTH];

char cell_AA[][3+1] =
{
  "  ", // CELL_TYPE_NONE
  "■", // CELL_TYPE_HARD_BLOCK
  "□", // CELL_TYPE_SOFT_BLOCK
  "※", // CELL_TYPE_EXPLOSION
  "扉", // CELL_TYPE_EXIT
};

typedef struct {
  int x, y;
  int direction;
  int is_dead;
}MONSTER;

MONSTER monsters[MONSTER_MAX];

typedef struct {
  int x, y;
  int cnt;
}BOMB;

BOMB bombs[BOMB_MAX];

int exit_x, exit_y;

int getch() /*{{{*/
{
  char ch;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  ch = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return ch;
} /*}}}*/

int get_monster(int _x, int _y)
{
  for(int i=0; i<MONSTER_MAX; i++)
    if(!monsters[i].is_dead && monsters[i].x==_x && monsters[i].y==_y)
      return i;
  return -1;
}

void set_free_pos(int *px, int *py)
{
  while(1) {
    int x = rand() % MAP_WIDTH;
    int y = rand() % MAP_HEIGHT;
    switch(cells[y][x]) {
      case CELL_TYPE_HARD_BLOCK:
      case CELL_TYPE_SOFT_BLOCK:
        break;
      default:
        if(get_monster(x, y) < 0) {
          *px = x;
          *py = y;
          return;
        }
        break;
    }
  }
}

int get_bomb(int _x, int _y)
{
  for(int i=0; i<BOMB_MAX; i++)
    if(bombs[i].cnt>0 && bombs[i].x==_x && bombs[i].y==_y)
      return i;
  return -1;
}

int check_can_move(int _x, int _y)
{
  switch(cells[_y][_x]) {
    case CELL_TYPE_HARD_BLOCK:
    case CELL_TYPE_SOFT_BLOCK:
      return 0;
    default:
      if((get_monster(_x, _y) < 0)
          && (get_bomb(_x, _y) < 0))
        return 1;
  }
  return 0;
}

void set_rand_soft_block_pos(int *px, int *py)
{
  while(1) {
    int x = rand() % MAP_WIDTH;
    int y = rand() % MAP_HEIGHT;
    if(cells[y][x] == CELL_TYPE_SOFT_BLOCK) {
      *px = x;
      *py = y;
      return;
    }
  }
}

int get_free_bomb()
{
  for(int i=0; i<BOMB_MAX; i++)
    if(bombs[i].cnt <= 0)
      return i;
  return -1;
}

void explotion(int _x, int _y)
{
  cells[_y][_x] = CELL_TYPE_EXPLOSION;
  for(int j=0; j<DIRECTION_MAX; j++) {
    int x = _x;
    int y = _y;
    for(int i=0; i<2; i++) {
      x += directions[j][0];
      y += directions[j][1];
      if(cells[y][x] == CELL_TYPE_HARD_BLOCK)
        break;
      else if(cells[y][x] == CELL_TYPE_SOFT_BLOCK) {
        cells[y][x] = CELL_TYPE_EXPLOSION;
        break;
      }
      else {
        int monster = get_monster(x, y);
        if(monster > 1) {
          monsters[monster].is_dead = 1;
        }

        int bomb = get_bomb(x, y);
        if(bomb >= 0) {
          bombs[bomb].cnt = 0;
          explotion(x, y);
        }

        cells[y][x] = CELL_TYPE_EXPLOSION;
      }
    }
  }
}

void quit()
{
  printf("\033[?25h");
  exit(0);
}

int main()
{
  srand((unsigned int)time(NULL));
  for(int y=0; y<MAP_HEIGHT; y+=MAP_HEIGHT-1)
    for(int x=0; x<MAP_WIDTH; x++)
      cells[y][x] = CELL_TYPE_HARD_BLOCK;
  for(int y=0; y<MAP_HEIGHT; y++)
    for(int x=0; x<MAP_WIDTH; x+=MAP_WIDTH-1)
      cells[y][x] = CELL_TYPE_HARD_BLOCK;
  for(int y=1; y<MAP_HEIGHT-1; y++)
    for(int x=1; x<MAP_WIDTH-1; x++) {
      if(x%2==0 && y%2==0)
        cells[y][x] = CELL_TYPE_HARD_BLOCK;
      else if(rand()%2)
        cells[y][x] = CELL_TYPE_SOFT_BLOCK;
    }
  cells[1][1] = cells[1][2] = cells[2][1] = CELL_TYPE_NONE;

  set_rand_soft_block_pos(&exit_x, &exit_y);

  monsters[0].x = monsters[0].y = 1;

  for(int i=1; i<MONSTER_MAX; i++)
    set_free_pos(&monsters[i].x, &monsters[i].y);

  printf("\033[2J\033[H\033[?25l");

  while(1) {
    printf("\033[H");
    int left = monsters[0].x - SCREEN_WIDTH/2;
    if(left < 0)
      left = 0;
    if(left > MAP_WIDTH - SCREEN_WIDTH)
      left = MAP_WIDTH - SCREEN_WIDTH;

    for(int y=0; y<MAP_HEIGHT; y++) {
      for(int x=left; x<left+SCREEN_WIDTH; x++) {
        int monster = get_monster(x, y);
        int bomb    = get_bomb(x, y);
        if(monster > 0)
          printf("敵");
        else if(monster == 0)
          printf("＠");
        else if(bomb >= 0)
          printf("%02d", bombs[bomb].cnt);
        else
          printf("%s", cell_AA[cells[y][x]]);
      }
      printf("\n");
    }

    for(int y=1; y<MAP_HEIGHT-1; y++)
      for(int x=1; x<MAP_WIDTH-1; x++)
        if(cells[y][x] == CELL_TYPE_EXPLOSION) {
          cells[y][x] = (x==exit_x && y==exit_y) ?
            CELL_TYPE_EXIT : CELL_TYPE_NONE;
        }

    int x = monsters[0].x;
    int y = monsters[0].y;
    switch(getch()) {
      case 'h': x--;    break;
      case 'j': y++;    break;
      case 'k': y--;    break;
      case 'l': x++;    break;
      case 'q': quit(); break;
      case 'b':
        {
          int bomb = get_free_bomb();
          if(bomb >= 0) {
            bombs[bomb].x   = monsters[0].x;
            bombs[bomb].y   = monsters[0].y;
            bombs[bomb].cnt = BOMB_CNT_MAX;
          }
        }
        break;
    }

    if(check_can_move(x, y)) {
      monsters[0].x = x;
      monsters[0].y = y;
    }

    for(int i=0; i<BOMB_MAX; i++) {
      if(bombs[i].cnt <= 0)
        continue;

      bombs[i].cnt--;
      if(bombs[i].cnt <= 0)
        explotion(bombs[i].x, bombs[i].y);
    }

    for(int i=1; i<MONSTER_MAX; i++) {
      if(monsters[i].is_dead)
        continue;
      int x = monsters[i].x + directions[monsters[i].direction][0];
      int y = monsters[i].y + directions[monsters[i].direction][1];
      if(check_can_move(x, y)) {
        monsters[i].x = x;
        monsters[i].y = y;
      } else {
        monsters[i].direction = rand() % DIRECTION_MAX;
      }
    }
  }
}
