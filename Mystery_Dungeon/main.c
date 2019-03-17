/* 不思議のダンジョンを小一時間で作ってみた */
/* https://youtu.be/xEA-K5HSw08 */
/* vim:set foldmethod=marker: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>

typedef int bool;
#define true  1
#define false 0

#define FIELD_WIDTH   32
#define FIELD_HEIGHT  32

#define AREA_MAX      32
#define AREA_SIZE_MIN 16
#define STAIRS_MAX    3

#define SCREEN_RANGE  8

enum {
  CELL_TYPE_NONE,
  CELL_TYPE_WALL,
  CELL_TYPE_STAIRS,
  CELL_TYPE_AMULET,
  CELL_TYPE_PLAYER,
  CELL_TYPE_ENEMY,
};

char cell_AA[][2+1] =
{
  " .", // CELL_TYPE_NONE
  " #", // CELL_TYPE_WALL
  " %", // CELL_TYPE_STAIRS
  " ,", // CELL_TYPE_AMULET
  " @", // CELL_TYPE_PLAYER
  " D", // CELL_TYPE_ENEMY
};

typedef struct {
  int x, y, w, h;
}ROOM;

typedef struct {
  int x, y, w, h;
  ROOM room;
}AREA;

AREA areas[AREA_MAX];
int area_cnt;

int field[FIELD_HEIGHT][FIELD_WIDTH];

typedef struct {
  int x,y;
  int hp, max_hp;
  int attack;
}CHARACTER;

CHARACTER player;
static int floor = 1;

CHARACTER enemy;

int getch() // {{{
{
  char c;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  c = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return c;
} // }}}

void display_area()
{
  int buf[FIELD_HEIGHT][FIELD_WIDTH] = {};
  for(int i=0; i<area_cnt; i++)
    for(int y=areas[i].y; y<areas[i].y+areas[i].h; y++)
      for(int x=areas[i].x; x<areas[i].x+areas[i].w; x++)
        buf[y][x] = i;

  printf("\033[H");
  for(int y=0; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++) {
      printf("\033[38;5;%03dm%2d", buf[y][x] + 1, buf[y][x]);
    }
    printf("\n");
  }
}

void split_area(int area_idx)
{
  int new_area_idx = area_cnt;
  int w = areas[area_idx].w;
  int h = areas[area_idx].h;

  if(rand()%2)
  {
    areas[area_idx].w /= 2;
    areas[new_area_idx].x = areas[area_idx].x + areas[area_idx].w;
    areas[new_area_idx].y = areas[area_idx].y;
    areas[new_area_idx].w = w - areas[area_idx].w;
    areas[new_area_idx].h = areas[area_idx].h;
  }
  else
  {
    areas[area_idx].h /= 2;
    areas[new_area_idx].x = areas[area_idx].x;
    areas[new_area_idx].y = areas[area_idx].y + areas[area_idx].h;
    areas[new_area_idx].w = areas[area_idx].w;
    areas[new_area_idx].h = h - areas[area_idx].h;
  }

  if(
      areas[area_idx].w < AREA_SIZE_MIN
      || areas[area_idx].h < AREA_SIZE_MIN
      || areas[new_area_idx].w < AREA_SIZE_MIN
      || areas[new_area_idx].h < AREA_SIZE_MIN
  ) {
    areas[area_idx].w = w;
    areas[area_idx].h = h;
    return;
  }
  area_cnt++;

  //display_area(); getch();

  split_area(area_idx);
  split_area(new_area_idx);
}

void set_rand_pos(int *_x, int *_y)
{
  int area_idx = rand()%area_cnt;
  *_x = areas[area_idx].room.x + rand() % areas[area_idx].room.w;
  *_y = areas[area_idx].room.y + rand() % areas[area_idx].room.h;
}

void generate_field()
{
  area_cnt   = 0;
  areas[0].x = 0;
  areas[0].y = 0;
  areas[0].w = FIELD_WIDTH;
  areas[0].h = FIELD_HEIGHT;
  area_cnt++;
  split_area(0);

  for(int y=0; y<FIELD_HEIGHT; y++)
    for(int x=0; x<FIELD_WIDTH; x++)
      field[y][x] = CELL_TYPE_WALL;

  for(int i=0; i<area_cnt; i++) {
    areas[i].room.x = areas[i].x + 2;
    areas[i].room.y = areas[i].y + 2;
    areas[i].room.w = areas[i].w - 4;
    areas[i].room.h = areas[i].h - 4;

    for(int y=areas[i].room.y; y<areas[i].room.y+areas[i].room.h; y++)
      for(int x=areas[i].room.x; x<areas[i].room.x+areas[i].room.w; x++)
        field[y][x] = CELL_TYPE_NONE;

    for(int x=areas[i].x; x<areas[i].x+areas[i].w; x++)
      field[areas[i].y + areas[i].h - 1][x] = CELL_TYPE_NONE;

    for(int y=areas[i].y; y<areas[i].y + areas[i].h; y++)
      field[y][areas[i].x + areas[i].w - 1] = CELL_TYPE_NONE;

    for(int y2=areas[i].y; y2<areas[i].room.y; y2++)
      field[y2][areas[i].x + areas[i].room.w / 2] = CELL_TYPE_NONE;

    for(int y2=areas[i].room.y + areas[i].room.h; y2<areas[i].y + areas[i].h; y2++)
      field[y2][areas[i].x + areas[i].room.w / 2] = CELL_TYPE_NONE;

    for(int x2=areas[i].x; x2<areas[i].room.x; x2++)
      field[areas[i].y + areas[i].room.h / 2][x2] = CELL_TYPE_NONE;

    for(int x2=areas[i].room.x + areas[i].room.w; x2<areas[i].x + areas[i].w; x2++)
      field[areas[i].y + areas[i].room.h / 2][x2] = CELL_TYPE_NONE;
  }

  for(int y=0; y<FIELD_HEIGHT; y++)
      field[y][FIELD_WIDTH-1] = CELL_TYPE_WALL;

  for(int x=0; x<FIELD_WIDTH; x++)
      field[FIELD_HEIGHT-1][x] = CELL_TYPE_WALL;

  bool filled = false;
  while(!filled) {
  for(int y=0; y<FIELD_HEIGHT; y++)
    for(int x=0; x<FIELD_WIDTH; x++) {
      if(field[y][x] == CELL_TYPE_WALL)
        continue;

      int v[][2] = {
        {  0, -1},
        { -1,  0},
        {  0,  1},
        {  1,  0},
      };
      int n = 0;

      for(int i=0; i<4; i++) {
        int x2 = x+v[i][0];
        int y2 = y+v[i][1];
        if(x2<0 || x2>=FIELD_WIDTH || y2<0 || y2>=FIELD_HEIGHT)
          n++;
        else if(field[y2][x2] == CELL_TYPE_WALL)
          n++;
      }
      if(n>=3) {
        field[y][x] = CELL_TYPE_WALL;
        filled = true;
      }
    }
  }

  set_rand_pos(&player.x, &player.y);
  {
    int x, y;
    set_rand_pos(&x, &y);
    field[y][x] = floor<STAIRS_MAX ? CELL_TYPE_STAIRS : CELL_TYPE_AMULET;
  }

  set_rand_pos(&enemy.x, &enemy.y);
  enemy.hp = enemy.max_hp = 2 + floor;
  enemy.attack = 2 + floor;
}

void draw_field()
{
  printf("\033[H");
  for(int y=0; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++)
      printf("%s", cell_AA[field[y][x]]);
    printf("\n");
  }
}

int get_room(int _x, int _y)
{
  for(int i=0; i<area_cnt; i++)
    if((_x>=areas[i].room.x) && (_x<areas[i].room.x+areas[i].room.w)
     &&(_y>=areas[i].room.y) && (_y<areas[i].room.y+areas[i].room.h))
      return i;
  return -1;
}

void display()
{
  int buf[FIELD_HEIGHT][FIELD_WIDTH];
  memcpy(buf, field, sizeof field);

  if(player.hp > 0)
    buf[player.y][player.x] = CELL_TYPE_PLAYER;
  if(enemy.hp > 0)
    buf[enemy.y][enemy.x]   = CELL_TYPE_ENEMY;

  printf("\033[H");
  printf("  %2dF    HP %3d/%3d\n", floor, player.hp, player.max_hp);
  for(int y=player.y-SCREEN_RANGE; y<player.y+SCREEN_RANGE; y++) {
    for(int x=player.x-SCREEN_RANGE; x<player.x+SCREEN_RANGE; x++) {
      if(x<0 || x>=FIELD_WIDTH || y<0 || y>=FIELD_HEIGHT)
        printf(" #");
      else
        printf("%s", cell_AA[buf[y][x]]);
    }
    printf("\n");
  }
}

void init()
{
  srand((unsigned int)time(NULL));
  printf("\033[2J\033[H\033[?25l");
}

void end()
{
  printf("\033[?25h");
  exit(0);
}

int main()
{
  init();
  int turn = 0;
  player.hp = player.max_hp = 15;
  player.attack = 3;
  generate_field();

  while(1) {
    display();

    int x = player.x;
    int y = player.y;
    switch(getch()) {
      case 'h': x--;      break;
      case 'j': y++;      break;
      case 'k': y--;      break;
      case 'l': x++;      break;
      case 'y': x--; y--; break;
      case 'u': x++; y--; break;
      case 'b': x--; y++; break;
      case 'n': x++; y++; break;
      case 'q': end();
      case '.': break;
      default: continue;
    }

    if(x==enemy.x && y==enemy.y) {
      printf(" Attack!            \n");
      getch();
      int attack = player.attack / 2 + rand() % player.attack;
      enemy.hp -= attack;
      printf(" %d damage!      ", attack);

      if(enemy.hp <= 0) {
        display();
        printf(" Dragon taoreta");
      }
    }
    else {
      switch(field[y][x]) {
        case CELL_TYPE_NONE:
          player.x = x;
          player.y = y;
          if((turn%10 == 0) && (player.hp < player.max_hp))
            player.hp++;
          break;
        case CELL_TYPE_WALL:
          break;
        case CELL_TYPE_STAIRS:
          floor++;
          generate_field();
          break;
        case CELL_TYPE_AMULET:
          printf("  Shien had Amulet of Yendor!  \n");
          printf("            --END--            \n\a");
          getch();
          end();
          break;
      }
      //printf("[%d]", rand());

      if(enemy.hp > 0) {
        int room = get_room(enemy.x, enemy.y);
        int distance = abs(player.x - enemy.x) + abs(player.y - enemy.y);
        if(1==distance) {
          display();
          printf(" Dragon Attack!      \n");
          getch();
          int attack = enemy.attack / 2 + rand() % enemy.attack;
          player.hp -= attack;
          printf(" %d Damage!         \n", attack);

          if(player.hp <= 0) {
            display();
            printf(" Ded...                        \n");
            printf("         --GAME OVER--         \n\a");
            getch();
            end();
          }
        }
        else if((room>=0 && get_room(player.x, player.y)==room) || 2==distance) {
          int x = enemy.x;
          int y = enemy.y;

          if(x < player.x) x++;
          else if(x > player.x) x--;
          if(y < player.y) y++;
          else if(y > player.y) y--;

          if(x==player.x && y==player.y) {
          }
          else if(field[y][x] == CELL_TYPE_NONE) {
            enemy.x = x;
            enemy.y = y;
          }
        }
      }
    }
    turn++;
  }
}
