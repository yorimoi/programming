/* https://youtu.be/tb5TYiruVmI */
/* vim: set nowrap : */

#include <stdio.h>
#include <string.h>
#include <termios.h>

#include "aa.h"
#include "aad.h"
#include "all.h"

#define AA_WIDTH  28
#define AA_HEIGHT 28

#define LOCATION_MAX 15

#define FIELD_WIDTH  20
#define FIELD_HEIGHT 20

#define WALL_TYPE_WALL 1
#define WALL_TYPE_DOOR 1

enum {
  DIRECTION_NORTH,
  DIRECTION_WEST,
  DIRECTION_SOUTH,
  DIRECTION_EAST,
  DIRECTION_MAX
};

enum {
  WALL_TYPE_NONE,
  //WALL_TYPE_WALL,
  //WALL_TYPE_DOOR,
  WALL_TYPE_MAX
};

int locations[DIRECTION_MAX][LOCATION_MAX][2] =
{
  // DIRECTION_NORTH
  {
    {-1,-4}, { 1,-4}, { 0,-4},
    {-1,-3}, { 1,-3}, { 0,-3},
    {-1,-2}, { 1,-2}, { 0,-2},
    {-1,-1}, { 1,-1}, { 0,-1},
    {-1, 0}, { 1, 0}, { 0, 0},
  },
  // DIRECTION_WEST
  {
    {-4, 1}, {-4,-1}, {-4, 0},
    {-3, 1}, {-3,-1}, {-3, 0},
    {-2, 1}, {-2,-1}, {-2, 0},
    {-1, 1}, {-1,-1}, {-1, 0},
    { 0, 1}, { 0,-1}, { 0, 0},
  },
  // DIRECTION_SOUTH
  {
    { 1, 4}, {-1, 4}, { 0, 4},
    { 1, 3}, {-1, 3}, { 0, 3},
    { 1, 2}, {-1, 2}, { 0, 2},
    { 1, 1}, {-1, 1}, { 0, 1},
    { 1, 0}, {-1, 0}, { 0, 0},
  },
  // DIRECTION_EAST
  {
    { 4,-1}, { 4, 1}, { 4, 0},
    { 3,-1}, { 3, 1}, { 3, 0},
    { 2,-1}, { 2, 1}, { 2, 0},
    { 1,-1}, { 1, 1}, { 1, 0},
    { 0,-1}, { 0, 1}, { 0, 0},
  },
};

int walls[FIELD_HEIGHT][FIELD_WIDTH] =
{
/* 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0, 64,  0,  0,  0,  2,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  7,  5,  5,  5,  5, 20,  5,  5,  5,  0,  8, 64,  0, 64,  0, 64,  0, 64,  0,  0},
{  5,  5,  5,  5,  5,  5,  5,  5, 13,  2,  4, 20,  5, 20,  5, 20,  5, 20,  5,133},
{  3,  5,  5,  1,  5,  5,  1,  5,133, 44,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{ 10,  0,  0, 10,  0,  0, 74,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{ 10,  0,  0, 10,  0,  3, 16,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{ 10,  0,  0, 10,  0,  6,  4, 12,  0,  0,  0,  0,  0,  0, 48, 24,  0,  0,  0,  0},
{ 10,  0,  0, 74,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 32,  8,  0,  0,  0,  0},
{ 10,  0,  3, 16,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0, 96, 72,  0,  0,  0,  0},
{ 10,  0,  6,  4, 12,  0,  3,  1,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{ 10,  0,  0,  0,  0,  0,  6, 64, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
{  6,  5,  5,  5,  5,  5,  5, 20, 13,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
};

char *aa[LOCATION_MAX][DIRECTION_MAX] =
{
  {NULL,NULL,aa0_2,NULL},
  {NULL,NULL,aa1_2,NULL},
  {NULL,NULL,aa2_2,NULL},
  {aa3_0,NULL,aa3_2,aa3_3},
  {aa4_0,aa4_1,aa4_2,NULL},
  {aa5_0,aa5_1,aa5_2,aa5_3},
  {aa6_0,NULL,aa6_2,aa6_3},
  {aa7_0,aa7_1,aa7_2,NULL},
  {aa8_0,aa8_1,aa8_2,aa8_3},
  {aa9_0,NULL,aa9_2,aa9_3},
  {aa10_0,aa10_1,aa10_2,NULL},
  {aa11_0,aa11_1,aa11_2,aa11_3},
  {aa12_0,NULL,NULL,aa12_3},
  {aa13_0,aa13_1,NULL,NULL},
  {aa14_0,aa14_1,NULL,aa14_3},
};

char *aad[LOCATION_MAX][DIRECTION_MAX] =
{
  {NULL,NULL,aa0_2d,NULL},
  {NULL,NULL,aa1_2d,NULL},
  {NULL,NULL,aa2_2d,NULL},
  {aa3_0d,NULL,aa3_2d,aa3_3d},
  {aa4_0d,aa4_1d,aa4_2d,NULL},
  {aa5_0d,aa5_1d,aa5_2d,aa5_3d},
  {aa6_0d,NULL,aa6_2d,aa6_3d},
  {aa7_0d,aa7_1d,aa7_2d,NULL},
  {aa8_0d,aa8_1d,aa8_2d,aa8_3d},
  {aa9_0d,NULL,aa9_2d,aa9_3d},
  {aa10_0d,aa10_1d,aa10_2d,NULL},
  {aa11_0d,aa11_1d,aa11_2d,aa11_3d},
  {aa12_0d,NULL,NULL,aa12_3d},
  {aa13_0d,aa13_1d,NULL,NULL},
  {aa14_0d,aa14_1d,NULL,aa14_3d},
};

int player_x = 0,
    player_y = 19;
int player_dir;

int getch()
{
  char ch;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  ch = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return ch;
}

int main()
{
  while(1) {
    char buffer[AA_WIDTH * AA_HEIGHT];
    //memcpy(buffer, aaAll, sizeof buffer);
    memset(buffer, ' ', sizeof buffer);
    for(int i=0; i<LOCATION_MAX; i++) {
      int x = player_x + locations[player_dir][i][0];
      int y = player_y + locations[player_dir][i][1];
      x = (FIELD_WIDTH  + x) % FIELD_WIDTH;
      y = (FIELD_HEIGHT + y) % FIELD_HEIGHT;
      for(int j=0; j<DIRECTION_MAX; j++) {
        int dir = (player_dir + j) % DIRECTION_MAX;
        if(((walls[y][x]>>dir)&1) == WALL_TYPE_WALL) {
          char *src = aa[i][j];
          if(src != NULL) {
            for(int k=0; k<AA_WIDTH * AA_HEIGHT; k++) {
              if(src[k] != ' ')
                buffer[k] = src[k];
            }
          }
        } else if(((walls[y][x]>>(dir+4))&1) == WALL_TYPE_DOOR) {
          char *src = aad[i][j];
          if(src != NULL) {
            for(int k=0; k<AA_WIDTH * AA_HEIGHT; k++) {
              if(src[k] != ' ')
                buffer[k] = src[k];
            }
          }
        }
      }
    }

    printf("\033[2J\033[H");

    printf("\033[H");
    for(int y=0; y<AA_HEIGHT; y++) {
      for(int x=0; x<AA_WIDTH; x++) {
        switch(buffer[y*AA_WIDTH + x]) {
          case ' ': printf("　"); break;
          case '.': printf("・"); break;
          case '/': printf("／"); break;
          case '?': printf("＼"); break;
          case '|': printf("｜"); break;
          case '_': printf("＿"); break;
          default:  printf("？"); break;
        }
      }
      printf("\n");
    }

    switch(getch()) {
      case 'h':
        player_dir = (player_dir+1) % DIRECTION_MAX;
        break;
      case 'j':
        player_dir = (player_dir+2) % DIRECTION_MAX;
        break;
      case 'k':
        {
          int v[][2] =
          {
            { 0,-1}, // DIRECTION_NORTH
            {-1, 0}, // DIRECTION_WEST
            { 0, 1}, // DIRECTION_SOUTH
            { 1, 0}, // DIRECTION_EAST
          };
          player_x = (FIELD_WIDTH + player_x + v[player_dir][0]) % FIELD_WIDTH;
          player_y = (FIELD_HEIGHT+ player_y + v[player_dir][1]) % FIELD_HEIGHT;
          if(player_x==5 && player_y==10) {
            player_x = 15;
            player_y = 15;
          }
        }
        break;
      case 'l':
        player_dir = (DIRECTION_MAX + player_dir - 1) % DIRECTION_MAX;
        break;
      case 'q': return 0; break;
    }
  }
}
