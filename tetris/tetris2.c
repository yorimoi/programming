#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 22

enum {
  STATE_NONE,
  STATE_WALL_SIDE,
  STATE_WALL_DOWN,
  STATE_MINO_I,
  STATE_MINO_O,
  STATE_MINO_S,
  STATE_MINO_Z,
  STATE_MINO_J,
  STATE_MINO_L,
  STATE_MINO_T,
  STATE_MAX
};

int STATES[FIELD_HEIGHT][FIELD_WIDTH];
int BUFF[FIELD_HEIGHT][FIELD_WIDTH];

enum {
  MINO_TYPE_I,
  MINO_TYPE_O,
  MINO_TYPE_S,
  MINO_TYPE_Z,
  MINO_TYPE_J,
  MINO_TYPE_L,
  MINO_TYPE_T,
  MINO_TYPE_MAX
};

enum {
  ANGLE_0,
  ANGLE_90,
  ANGLE_180,
  ANGLE_270,
  ANGLE_MAX,
};

#define MINO_WIDTH  4
#define MINO_HEIGHT 4
int MINO_TYPE[MINO_TYPE_MAX][ANGLE_MAX][MINO_HEIGHT][MINO_WIDTH] =
{
  /* MINO_TYPE_I */
  {
    {
      { 0, 0, 0, 0},
      { STATE_MINO_I, STATE_MINO_I, STATE_MINO_I, STATE_MINO_I},
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
    },
    {
      { 0, 0, STATE_MINO_I, 0},
      { 0, 0, STATE_MINO_I, 0},
      { 0, 0, STATE_MINO_I, 0},
      { 0, 0, STATE_MINO_I, 0},
    },
    {
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
      { STATE_MINO_I, STATE_MINO_I, STATE_MINO_I, STATE_MINO_I},
      { 0, 0, 0, 0},
    },
    {
      { 0, STATE_MINO_I, 0, 0},
      { 0, STATE_MINO_I, 0, 0},
      { 0, STATE_MINO_I, 0, 0},
      { 0, STATE_MINO_I, 0, 0},
    }
  },
  /* MINO_TYPE_O */
  /* MINO_TYPE_S */
  /* MINO_TYPE_Z */
  /* MINO_TYPE_J */
  /* MINO_TYPE_L */
  /* MINO_TYPE_T */
};

int getch()
{
  char c;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  c = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return c;
}

int kbhit()
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
}


void draw(int mino_x, int mino_y)
{
  printf("\033[H");

  memcpy(BUFF, STATES, sizeof(STATES));
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      BUFF[y+mino_y][x+mino_x] = MINO_TYPE[MINO_TYPE_I][ANGLE_0][y][x];

  for(int y=0; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++) {
      switch(BUFF[y][x])
      {
        case STATE_WALL_SIDE: printf("\033[37m |"); break;
        case STATE_WALL_DOWN: printf("\033[37m -"); break;
        case STATE_MINO_I:    printf("\033[31m #"); break;
        case STATE_MINO_O:    printf("\033[32m #"); break;
        case STATE_MINO_S:    printf("\033[33m #"); break;
        case STATE_MINO_Z:    printf("\033[34m #"); break;
        case STATE_MINO_J:    printf("\033[35m #"); break;
        case STATE_MINO_L:    printf("\033[36m #"); break;
        case STATE_MINO_T:    printf("\033[37m #"); break;
        default:              printf("  ");         break;
      }
    }
    printf("\n");
  }
}

int main()
{
  int mino_x = 4;
  int mino_y = 0;
  int quit_flag = 0;
  srand((unsigned int)time(NULL));
  time_t t = time(NULL);

  for(int y=0; y<FIELD_HEIGHT-1; y++)
    for(int x=0; x<FIELD_WIDTH; x+=FIELD_WIDTH-1)
      STATES[y][x] = STATE_WALL_SIDE;
  for(int x=0; x<FIELD_WIDTH; x++)
    STATES[FIELD_HEIGHT-1][x] = STATE_WALL_DOWN;

  printf("\033[2J\033[H\033[?25l");

  while(!quit_flag)
  {
    if(kbhit())
    {
      switch(getchar())
      {
        case 'h': /*if(is_hit(mino_x, mino_y))*/ mino_x--; break;
        case 'j': mino_y++; break;
        case 'k': break;
        case 'l': mino_x++; break;
        case 'q': quit_flag = 1;
        default: continue;
      }
      draw(mino_x, mino_y);
    }

    if(t == time(NULL)) continue;
    t = time(NULL);
    draw(mino_x, mino_y);
    mino_y++;
  }

  printf("\033[?25h");

  return 0;
}
