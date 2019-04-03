#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 25

#define MINO_WIDTH  4
#define MINO_HEIGHT 4

enum {
  BLOCK_NONE,
  BLOCK_WALL,
  BLOCK_MINO_I,
  BLOCK_MINO_O,
  BLOCK_MINO_S,
  BLOCK_MINO_Z,
  BLOCK_MINO_J,
  BLOCK_MINO_L,
  BLOCK_MINO_T
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

int mino_aa[MINO_TYPE_MAX][MINO_ANGLE_MAX][MINO_HEIGHT][MINO_WIDTH] =
{
  // MINO_TYPE_I
  {
    // MINO_ANGLE_0
    {
      { 0, 0, 0, 0},
      { 1, 1, 1, 1},
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_90
    {
      { 0, 0, 1, 0},
      { 0, 0, 1, 0},
      { 0, 0, 1, 0},
      { 0, 0, 1, 0},
    },
    // MINO_ANGLE_180
    {
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
      { 1, 1, 1, 1},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_270
    {
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
    },
  },
  // MINO_TYPE_O
  {
    // MINO_ANGLE_0
    {
      { 0, 0, 0, 0},
      { 0, 1, 1, 0},
      { 0, 1, 1, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_90
    {
      { 0, 0, 0, 0},
      { 0, 1, 1, 0},
      { 0, 1, 1, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_180
    {
      { 0, 0, 0, 0},
      { 0, 1, 1, 0},
      { 0, 1, 1, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_270
    {
      { 0, 0, 0, 0},
      { 0, 1, 1, 0},
      { 0, 1, 1, 0},
      { 0, 0, 0, 0},
    },
  },
  // MINO_TYPE_S
  {
    // MINO_ANGLE_0
    {
      { 0, 0, 0, 0},
      { 0, 1, 1, 0},
      { 1, 1, 0, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_90
    {
      { 0, 0, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 1, 0},
      { 0, 0, 1, 0},
    },
    // MINO_ANGLE_180
    {
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
      { 0, 1, 1, 0},
      { 1, 1, 0, 0},
    },
    // MINO_ANGLE_270
    {
      { 0, 0, 0, 0},
      { 1, 0, 0, 0},
      { 1, 1, 0, 0},
      { 0, 1, 0, 0},
    },
  },
  // MINO_TYPE_Z
  {
    // MINO_ANGLE_0
    {
      { 0, 0, 0, 0},
      { 1, 1, 0, 0},
      { 0, 1, 1, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_90
    {
      { 0, 0, 0, 0},
      { 0, 0, 1, 0},
      { 0, 1, 1, 0},
      { 0, 1, 0, 0},
    },
    // MINO_ANGLE_180
    {
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
      { 1, 1, 0, 0},
      { 0, 1, 1, 0},
    },
    // MINO_ANGLE_270
    {
      { 0, 0, 0, 0},
      { 0, 1, 0, 0},
      { 1, 1, 0, 0},
      { 1, 0, 0, 0},
    },
  },
  // MINO_TYPE_J
  {
    // MINO_ANGLE_0
    {
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
      { 1, 1, 1, 0},
      { 0, 0, 1, 0},
    },
    // MINO_ANGLE_90
    {
      { 0, 0, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
      { 1, 1, 0, 0},
    },
    // MINO_ANGLE_180
    {
      { 0, 0, 0, 0},
      { 1, 0, 0, 0},
      { 1, 1, 1, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_270
    {
      { 0, 0, 0, 0},
      { 0, 1, 1, 0},
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
    },
  },
  // MINO_TYPE_L
  {
    // MINO_ANGLE_0
    {
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
      { 1, 1, 1, 0},
      { 1, 0, 0, 0},
    },
    // MINO_ANGLE_90
    {
      { 0, 0, 0, 0},
      { 1, 1, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
    },
    // MINO_ANGLE_180
    {
      { 0, 0, 0, 0},
      { 0, 0, 1, 0},
      { 1, 1, 1, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_270
    {
      { 0, 0, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 1, 0},
    },
  },
  // MINO_TYPE_T
  {
    // MINO_ANGLE_0
    {
      { 0, 0, 0, 0},
      { 0, 1, 0, 0},
      { 1, 1, 1, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_90
    {
      { 0, 0, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 1, 0},
      { 0, 1, 0, 0},
    },
    // MINO_ANGLE_180
    {
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
      { 1, 1, 1, 0},
      { 0, 1, 0, 0},
    },
    // MINO_ANGLE_270
    {
      { 0, 0, 0, 0},
      { 0, 1, 0, 0},
      { 1, 1, 0, 0},
      { 0, 1, 0, 0},
    },
  },
};

char block[][19+3+1] =
{
  "\033[38;2;000;000;000m  ", // BLOCK_NONE
  "\033[38;2;240;240;240m■", // BLOCK_WALL
  "\033[38;2;000;255;255m■", // BLOCK_MINO_I
  "\033[38;2;255;255;000m■", // BLOCK_MINO_O
  "\033[38;2;000;224;000m■", // BLOCK_MINO_S
  "\033[38;2;255;000;000m■", // BLOCK_MINO_Z
  "\033[38;2;032;128;255m■", // BLOCK_MINO_J
  "\033[38;2;255;128;000m■", // BLOCK_MINO_L
  "\033[38;2;192;064;192m■", // BLOCK_MINO_T
};

int field[FIELD_HEIGHT][FIELD_WIDTH];
int field_buffer[FIELD_HEIGHT][FIELD_WIDTH];

int mino_x, mino_y;
int mino_type;
int mino_angle;

void reset_mino()
{
  mino_type = rand() % MINO_TYPE_MAX;
  mino_angle = 0;
  mino_x = 4;
  switch(mino_type) {
    case MINO_TYPE_J:
    case MINO_TYPE_L: mino_y = 0; break;
    case MINO_TYPE_O:
    case MINO_TYPE_S:
    case MINO_TYPE_Z:
    case MINO_TYPE_T: mino_y = 1; break;
    case MINO_TYPE_I: mino_y = 2; break;
  }
}

void init()
{
  memset(field, BLOCK_NONE, sizeof field);
  for(int y=4; y<FIELD_HEIGHT; y++)
    field[y][0] = field[y][FIELD_WIDTH-1] = BLOCK_WALL;
  for(int x=1; x<FIELD_WIDTH-1; x++)
    field[FIELD_HEIGHT-1][x] = BLOCK_WALL;
  reset_mino();
}

void draw()
{
  printf("\033[H");
  memcpy(field_buffer, field, sizeof field);
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if(mino_aa[mino_type][mino_angle][y][x])
        field_buffer[y+mino_y][x+mino_x]
          = BLOCK_MINO_I + mino_type;

  for(int y=2; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++) {
      printf("%s", block[field_buffer[y][x]]);
    }
    printf("\n");
  }
}

// Collision Detection
int cd(int _x, int _y, int _angle)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if(
        (mino_aa[mino_type][_angle][y][x])
        && (field[_y+y][_x+x] != BLOCK_NONE)
      )
        return 1;
  return 0;
}

void end()
{
  printf("\033[0m\033[?25h");
  printf("Game Over!\n");
  exit(0);
}

int main()
{
  srand((unsigned int)time(NULL));
  init();

  printf("\033[2J\033[?25l");

  while(1) {
    switch(rand()%4) {
      case 0: if(!cd(mino_x-1, mino_y, mino_angle)) mino_x--; break;
      case 1: if(!cd(mino_x+1, mino_y, mino_angle)) mino_x++; break;
      case 2:
        if(!cd(mino_x, mino_y, (mino_angle+1)%MINO_ANGLE_MAX))
          mino_angle = (mino_angle + 1) % MINO_ANGLE_MAX;
        break;
      case 3: break;
    }
    draw();
    if(!cd(mino_x, mino_y+1, mino_angle))
      mino_y++;
    else {
      memcpy(field, field_buffer, sizeof field);
      for(int y=4; y<FIELD_HEIGHT-1; y++) {
        int erase = 1;
        for(int x=1; x<FIELD_WIDTH-1; x++)
          if(field[y][x] == BLOCK_NONE)
            erase = 0;
        if(erase)
          for(int y2=y; y2>0; y2--)
            for(int x=1; x<FIELD_WIDTH-1; x++)
              field[y2][x] = field[y2-1][x];
      }
      for(int x=1; x<FIELD_WIDTH-1; x++)
        if(field[3][x] != BLOCK_NONE)
          end();
          //init();

      reset_mino();
    }
    draw();
  }
}
