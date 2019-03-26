#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 25

#define MINO_SIZE    4

enum {
  BLOCK_NONE,
  BLOCK_WALL,
  BLOCK_MINO_I,
  BLOCK_MINO_O,
  BLOCK_MINO_S,
  BLOCK_MINO_Z,
  BLOCK_MINO_J,
  BLOCK_MINO_L,
  BLOCK_MINO_T,
  BLOCK_MAX
};

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
  MINO_ANGLE_0,
  MINO_ANGLE_90,
  MINO_ANGLE_180,
  MINO_ANGLE_270,
  MINO_ANGLE_MAX
};

char BLOCK_AA[BLOCK_MAX][19+2+1] =
{
  "\033[38;2;000;000;000m  ", // BLOCK_NONE
  "\033[38;2;240;240;240m #", // BLOCK_WALL
  "\033[38;2;000;255;255m #", // BLOCK_MINO_I
  "\033[38;2;255;255;000m #", // BLOCK_MINO_O
  "\033[38;2;000;224;000m #", // BLOCK_MINO_S
  "\033[38;2;255;000;000m #", // BLOCK_MINO_Z
  "\033[38;2;032;128;255m #", // BLOCK_MINO_J
  "\033[38;2;255;128;000m #", // BLOCK_MINO_L
  "\033[38;2;192;064;192m #", // BLOCK_MINO_T
};

int MINO_TYPE[MINO_TYPE_MAX][MINO_ANGLE_MAX][MINO_SIZE] =
{
  // MINO_TYPE_I
  {
    // MINO_ANGLE_0
      { 4, 5, 6, 7},
    // MINO_ANGLE_90
      { 2, 6,10,14},
    // MINO_ANGLE_180
      { 8, 9,10,11},
    // MINO_ANGLE_270
      { 1, 5, 9,13},
  },
  // MINO_TYPE_O
  {
    // MINO_ANGLE_0
    { 5, 6, 9,10},
    // MINO_ANGLE_90
    { 5, 6, 9,10},
    // MINO_ANGLE_180
    { 5, 6, 9,10},
    // MINO_ANGLE_270
    { 5, 6, 9,10},
  },
  // MINO_TYPE_S
  {
    // MINO_ANGLE_0
    { 5, 6, 8, 9},
    // MINO_ANGLE_90
    { 5, 9,10,14},
    // MINO_ANGLE_180
    { 9,10,12,13},
    // MINO_ANGLE_270
    { 4, 8, 9,13},
  },
  // MINO_TYPE_Z
  {
    // MINO_ANGLE_0
    { 4, 5, 9,10},
    // MINO_ANGLE_90
    { 6, 9,10,13},
    // MINO_ANGLE_180
    { 8, 9, 13,14},
    // MINO_ANGLE_270
    { 5, 8, 9,12},
  },
  // MINO_TYPE_J
  {
    // MINO_ANGLE_0
    { 8, 9,10,14},
    // MINO_ANGLE_90
    { 5, 9,12,13},
    // MINO_ANGLE_180
    { 4, 8, 9,10},
    // MINO_ANGLE_270
    { 5, 6, 9,13},
  },
  // MINO_TYPE_L
  {
    // MINO_ANGLE_0
    { 8, 9,10,12},
    // MINO_ANGLE_90
    { 5, 6,10,14},
    // MINO_ANGLE_180
    { 6, 8, 9,10},
    // MINO_ANGLE_270
    { 5, 9,13,14},
  },
  // MINO_TYPE_T
  {
    // MINO_ANGLE_0
    { 5, 8, 9,10},
    // MINO_ANGLE_90
    { 5, 9,10,13},
    // MINO_ANGLE_180
    { 8, 9,10,13},
    // MINO_ANGLE_270
    { 5, 8, 9,13},
  }
};

int field[FIELD_HEIGHT][FIELD_WIDTH];
int field_buffer[FIELD_HEIGHT][FIELD_WIDTH];

int mino_x, mino_y, mino_type, mino_angle;
int mino_stack1[MINO_TYPE_MAX] = {0,1,2,3,4,5,6};
int mino_stack2[MINO_TYPE_MAX] = {0,1,2,3,4,5,6};
int mino_stack_cnt = -1;

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
int kbhit() /*{{{*/
{
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
} /*}}}*/

void init_mino(int *mino_stack)
{
  for(int i=MINO_TYPE_MAX-1; i>0; i--) {
    int mix = rand() % i;
    int tmp = *(mino_stack+i);
    *(mino_stack+i) = *(mino_stack+mix);
    *(mino_stack+mix) = tmp;
 }
}

void next_mino()
{
  if(MINO_TYPE_MAX <= ++mino_stack_cnt) {
    mino_stack_cnt = 0;
    memcpy(mino_stack1, mino_stack2, sizeof(int)*MINO_TYPE_MAX);
    init_mino(mino_stack2);
  }
  mino_type = mino_stack1[mino_stack_cnt];
  mino_angle = 0;
  mino_x     = 4;
  switch(mino_type) {
    case MINO_TYPE_I: mino_y = 2; break;
    case MINO_TYPE_O: mino_y = 1; break;
    case MINO_TYPE_S: mino_y = 1; break;
    case MINO_TYPE_Z: mino_y = 1; break;
    case MINO_TYPE_J: mino_y = 0; break;
    case MINO_TYPE_L: mino_y = 0; break;
    case MINO_TYPE_T: mino_y = 1; break;
  }
}

void display()
{
  memcpy(field_buffer, field, sizeof field);
  for(int i=0, j=0; j<MINO_SIZE; i++)
    if(MINO_TYPE[mino_type][mino_angle][j] == i)
      field_buffer[mino_y+i/4][mino_x+i%4] = mino_type + 2, j++;

  printf("\033[H");
  for(int y=2; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++)
      printf("%s", BLOCK_AA[field_buffer[y][x]]);
    printf("\n");
  }
}

/* Collision Detection */
int cd(int _mino_x, int _mino_y, int _mino_angle)
{
  for(int i=0,j=0; j<MINO_SIZE; i++)
    if(MINO_TYPE[mino_type][_mino_angle][j] == i) {
      j++;
      if(field[_mino_y+i/4][_mino_x+i%4] != BLOCK_NONE)
        return 1;
    }
  return 0;
}

void quit()
{
  printf("\033[?25h");
  exit(0);
}

void gameover()
{
  printf("       GAME OVER!\a\n");
  quit();
}

int main()
{
  time_t t = 0;
  srand((unsigned int)time(NULL));

  for(int y=3; y<FIELD_HEIGHT; y++)
    field[y][0] = field[y][FIELD_WIDTH-1] = BLOCK_WALL;
  for(int x=1; x<FIELD_WIDTH-1; x++)
    field[FIELD_HEIGHT-1][x] = BLOCK_WALL;

  init_mino(mino_stack1);
  init_mino(mino_stack2);
  next_mino();
  printf("\033[2J\033[H\033[?25l");

  while(1) {
    if(time(NULL) != t) {
      t = time(NULL);
      if(!cd(mino_x, mino_y+1, mino_angle)) {
        mino_y++;
      } else {
        memcpy(field, field_buffer, sizeof field_buffer);
        next_mino();

        for(int y=0; y<FIELD_HEIGHT-1; y++) {
          int line_erase = 1;
          for(int x=1; x<FIELD_WIDTH-1; x++)
            if(field[y][x] == BLOCK_NONE)
              line_erase = 0;
          if(line_erase) {
            for(int y2=y; y2>0; y2--)
              for(int x2=1; x2<FIELD_WIDTH-1; x2++)
                field[y2][x2] = field[y2-1][x2];
          }
        }

        for(int i=0, j=0; j<MINO_SIZE; i++)
          if(MINO_TYPE[mino_type][mino_angle][j] == i) {
            j++;
            if(field[mino_y+i/4][mino_x+i%4] != BLOCK_NONE) {
              display();
              gameover();
            }
          }
      }
      display();
    }

    if(kbhit()) {
      switch(getch()) {
        case 'h': if(!cd(mino_x-1, mino_y, mino_angle)) mino_x--; break;
        case 'j': if(!cd(mino_x, mino_y+1, mino_angle)) mino_y++; break;
        case 'k':
          while(1) {
            if(!cd(mino_x, mino_y+1, mino_angle)) mino_y++;
            else break;
          }
          break;
        case 'l': if(!cd(mino_x+1, mino_y, mino_angle)) mino_x++; break;
        case 'q': quit();  break;
        case ' ':
          if(!cd(mino_x, mino_y, (mino_angle + 1) % MINO_ANGLE_MAX))
            mino_angle = (mino_angle + 1) % MINO_ANGLE_MAX;
          break;
      }
      display();
    }
  }
}
