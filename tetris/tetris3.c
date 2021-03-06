#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>

#define FIELD_WIDTH    12
#define FIELD_HEIGTH   22

#define MINO_TYPE_MAX  7
#define MINO_WIDTH     4
#define MINO_HEIGHT    4
#define MINO_START_X   4
#define MINO_START_Y   0

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
  MINO_ANGLE_0,
  MINO_ANGLE_90,
  MINO_ANGLE_180,
  MINO_ANGLE_270,
  MINO_ANGLE_MAX
};

char BLOCK_COLORS[][19+2+1] =
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

int MINO_AA[MINO_TYPE_MAX][MINO_ANGLE_MAX][MINO_HEIGHT][MINO_WIDTH] =
{
  // BLOCK_MINO_I
  {
    // MINO_ANGLE_0
    {
      {0,0,0,0},
      {1,1,1,1},
      {0,0,0,0},
      {0,0,0,0},
    },
    // MINO_ANGLE_90
    {
      {0,0,1,0},
      {0,0,1,0},
      {0,0,1,0},
      {0,0,1,0},
    },
    // MINO_ANGLE_180
    {
      {0,0,0,0},
      {0,0,0,0},
      {1,1,1,1},
      {0,0,0,0},
    },
    // MINO_ANGLE_270
    {
      {0,1,0,0},
      {0,1,0,0},
      {0,1,0,0},
      {0,1,0,0},
    },
  },
  // BLOCK_MINO_O
  {
    // MINO_ANGLE_0
    {
      {0,0,0,0},
      {0,1,1,0},
      {0,1,1,0},
      {0,0,0,0},
    },
    // MINO_ANGLE_90
    {
      {0,0,0,0},
      {0,1,1,0},
      {0,1,1,0},
      {0,0,0,0},
    },
    // MINO_ANGLE_180
    {
      {0,0,0,0},
      {0,1,1,0},
      {0,1,1,0},
      {0,0,0,0},
    },
    // MINO_ANGLE_270
    {
      {0,0,0,0},
      {0,1,1,0},
      {0,1,1,0},
      {0,0,0,0},
    },
  },
  // BLOCK_MINO_S
  {
    // MINO_ANGLE_0
    {
      {0,0,0,0},
      {0,1,1,0},
      {1,1,0,0},
      {0,0,0,0},
    },
    // MINO_ANGLE_90
    {
      {0,0,0,0},
      {0,1,0,0},
      {0,1,1,0},
      {0,0,1,0},
    },
    // MINO_ANGLE_180
    {
      {0,0,0,0},
      {0,0,0,0},
      {0,1,1,0},
      {1,1,0,0},
    },
    // MINO_ANGLE_270
    {
      {0,0,0,0},
      {1,0,0,0},
      {1,1,0,0},
      {0,1,0,0},
    },
  },
  // BLOCK_MINO_Z
  {
    // MINO_ANGLE_0
    {
      {0,0,0,0},
      {1,1,0,0},
      {0,1,1,0},
      {0,0,0,0},
    },
    // MINO_ANGLE_90
    {
      {0,0,0,0},
      {0,0,1,0},
      {0,1,1,0},
      {0,1,0,0},
    },
    // MINO_ANGLE_180
    {
      {0,0,0,0},
      {0,0,0,0},
      {1,1,0,0},
      {0,1,1,0},
    },
    // MINO_ANGLE_270
    {
      {0,0,0,0},
      {0,1,0,0},
      {1,1,0,0},
      {1,0,0,0},
    },
  },
  // BLOCK_MINO_J
  {
    // MINO_ANGLE_0
    {
      {0,0,0,0},
      {0,0,0,0},
      {1,1,1,0},
      {0,0,1,0},
    },
    // MINO_ANGLE_90
    {
      {0,0,0,0},
      {0,1,0,0},
      {0,1,0,0},
      {1,1,0,0},
    },
    // MINO_ANGLE_180
    {
      {0,0,0,0},
      {1,0,0,0},
      {1,1,1,0},
      {0,0,0,0},
    },
    // MINO_ANGLE_270
    {
      {0,0,0,0},
      {0,1,1,0},
      {0,1,0,0},
      {0,1,0,0},
    },
  },
  // BLOCK_MINO_L
  {
    // MINO_ANGLE_0
    {
      {0,0,0,0},
      {0,0,0,0},
      {1,1,1,0},
      {1,0,0,0},
    },
    // MINO_ANGLE_90
    {
      {0,0,0,0},
      {0,1,1,0},
      {0,0,1,0},
      {0,0,1,0},
    },
    // MINO_ANGLE_180
    {
      {0,0,0,0},
      {0,0,1,0},
      {1,1,1,0},
      {0,0,0,0},
    },
    // MINO_ANGLE_270
    {
      {0,0,0,0},
      {0,1,0,0},
      {0,1,0,0},
      {0,1,1,0},
    },
  },
  // BLOCK_MINO_T
  {
    // MINO_ANGLE_0
    {
      {0,0,0,0},
      {0,1,0,0},
      {1,1,1,0},
      {0,0,0,0},
    },
    // MINO_ANGLE_90
    {
      {0,0,0,0},
      {0,1,0,0},
      {0,1,1,0},
      {0,1,0,0},
    },
    // MINO_ANGLE_180
    {
      {0,0,0,0},
      {0,0,0,0},
      {1,1,1,0},
      {0,1,0,0},
    },
    // MINO_ANGLE_270
    {
      {0,0,0,0},
      {0,1,0,0},
      {1,1,0,0},
      {0,1,0,0},
    },
  },
};

int field[FIELD_HEIGTH][FIELD_WIDTH];
int field_buffer[FIELD_HEIGTH][FIELD_WIDTH];

int mino_type;
int mino_angle;
int mino_x;
int mino_y;

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
}/*}}}*/
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

void display()
{
  printf("\033[H");
  memcpy(field_buffer, field, sizeof field);
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if(MINO_AA[mino_type-2][mino_angle][y][x])
        field_buffer[y+mino_y][x+mino_x] = mino_type;

  for(int y=0; y<FIELD_HEIGTH; y++) {
    for(int x=0; x<FIELD_WIDTH; x++) {
      printf("%s", BLOCK_COLORS[field_buffer[y][x]]);
    }
    printf("\n");
  }
}

/* Collision Detection */
int cd(int _mino_x, int _mino_y, int _mino_type, int _mino_angle)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if(
        (field[_mino_y+y][_mino_x+x] != BLOCK_NONE)
        && (MINO_AA[_mino_type-2][_mino_angle][y][x])
      )
        return 1;
  return 0;
}

void reset_mino()
{
  mino_x = MINO_START_X;
  mino_y = MINO_START_Y;
  mino_angle = 0;
  mino_type  = BLOCK_MINO_I + rand() % MINO_TYPE_MAX;
}

void quit()
{
  printf("\033[?25h");
  exit(0);
}

void gameover()
{
  printf("       GAME OVER!       \a\n");
  quit();
}

int main()
{
  srand((unsigned int)time(NULL));
  time_t t = 0;

  for(int y=0; y<FIELD_HEIGTH; y++)
    field[y][0] = field[y][FIELD_WIDTH-1] = BLOCK_WALL;
  for(int x=1; x<FIELD_WIDTH-1; x++)
    field[FIELD_HEIGTH-1][x] = BLOCK_WALL;

  reset_mino();

  printf("\033[2J\033[H\033[?25l");
  display();

  while(1) {
    if(time(NULL) != t) {
      t = time(NULL);
      if(!cd(mino_x,mino_y+1,mino_type,mino_angle))
        mino_y++;
      else {
        memcpy(field, field_buffer, sizeof field_buffer);
        reset_mino();
        for(int y=0; y<FIELD_HEIGTH-1; y++) {
          int line_erase = 1;
          for(int x=1; x<FIELD_WIDTH-1; x++)
            if(field[y][x] == BLOCK_NONE)
              line_erase = 0;
          if(line_erase) {
            for(int i=y; i>0; i--)
              memcpy(field[i], field[i-1], (sizeof(int)) * FIELD_WIDTH);
          }
        }

        for(int x=1; x<FIELD_WIDTH-1; x++)
          if(field[1][x])
            gameover();
      }
      display();
    }

    if(kbhit()) {
      switch(getch()) {
        case 'q': quit(); break;
        case 'h': if(!cd(mino_x-1,mino_y,mino_type,mino_angle))mino_x--; break;
        case 'j': if(!cd(mino_x,mino_y+1,mino_type,mino_angle))mino_y++; break;
        case 'k':
          while(1) {
            if(!cd(mino_x,mino_y+1,mino_type,mino_angle)) mino_y++;
            else break;
          }
          break;
        case 'l': if(!cd(mino_x+1,mino_y,mino_type,mino_angle))mino_x++; break;
        case ' ':
          if(!cd(mino_x, mino_y, mino_type, (mino_angle+1)%MINO_ANGLE_MAX))
            mino_angle = (mino_angle + 1) % MINO_ANGLE_MAX;
          break;
      }
      display();
    }
  }
}
