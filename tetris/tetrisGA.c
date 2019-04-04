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

/*mino_aa {{{*/
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
      { 1, 0, 0, 0},
      { 1, 1, 1, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_90
    {
      { 0, 0, 0, 0},
      { 0, 1, 1, 0},
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
    },
    // MINO_ANGLE_180
    {
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
      { 1, 1, 1, 0},
      { 0, 0, 1, 0},
    },
    // MINO_ANGLE_270
    {
      { 0, 0, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
      { 1, 1, 0, 0},
    },
  },
  // MINO_TYPE_L
  {
    // MINO_ANGLE_0
    {
      { 0, 0, 0, 0},
      { 0, 0, 1, 0},
      { 1, 1, 1, 0},
      { 0, 0, 0, 0},
    },
    // MINO_ANGLE_90
    {
      { 0, 0, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 1, 0},
    },
    // MINO_ANGLE_180
    {
      { 0, 0, 0, 0},
      { 0, 0, 0, 0},
      { 1, 1, 1, 0},
      { 1, 0, 0, 0},
    },
    // MINO_ANGLE_270
    {
      { 0, 0, 0, 0},
      { 1, 1, 0, 0},
      { 0, 1, 0, 0},
      { 0, 1, 0, 0},
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
/*}}}*/

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

int mino_stack1[MINO_TYPE_MAX] = {0,1,2,3,4,5,6};
int mino_stack2[MINO_TYPE_MAX] = {0,1,2,3,4,5,6};
int mino_stack_cnt = -1;


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
    case MINO_TYPE_J:
    case MINO_TYPE_L:
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
  //for(int y=4; y<FIELD_HEIGHT; y++)
  for(int y=0; y<FIELD_HEIGHT; y++)
    field[y][0] = field[y][FIELD_WIDTH-1] = BLOCK_WALL;
  for(int x=1; x<FIELD_WIDTH-1; x++)
    field[FIELD_HEIGHT-1][x] = BLOCK_WALL;
  init_mino(mino_stack1);
  init_mino(mino_stack2);
  mino_stack_cnt = -1;
  next_mino();
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

  // 見えない壁
  for(int y=2; y<4; y++) {
    printf("  ");
    for(int x=1; x<FIELD_WIDTH-1; x++)
      printf("%s", block[field_buffer[y][x]]);
    printf("\n");
  }
  // Main field
  for(int y=4; y<FIELD_HEIGHT; y++) {
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
    if(!cd(mino_x, mino_y+1, mino_angle))
      mino_y++;
    else {
      memcpy(field, field_buffer, sizeof field);
      for(int y=4; y<FIELD_HEIGHT-1; y++) {
        int erase = 1;
        for(int x=1; x<FIELD_WIDTH-1; x++)
          if(field[y][x] == BLOCK_NONE)
            erase = 0;
        if(erase) {
          for(int y2=y; y2>0; y2--)
            for(int x=1; x<FIELD_WIDTH-1; x++)
              field[y2][x] = field[y2-1][x];
        }
      }

      // フィールド外に固定されたか
      {
        int exit_flag = 0, cnt = 0;
        for(int y=0; y<MINO_HEIGHT; y++)
          for(int x=0; x<MINO_WIDTH; x++)
            if(field[y+mino_y][x+mino_x]
                && mino_aa[mino_type][mino_angle][y][x]
                && mino_y+y<MINO_HEIGHT)
              cnt++;
        if(cnt==4) {
          exit_flag = 1;
          //end();
          init();
        }
        if(exit_flag) {
          continue;
        }
      }

      // Nextミノがフィールドに被ったか
      {
        int exit_flag = 0;
        next_mino();
        draw();
        for(int y=0; y<MINO_HEIGHT; y++) {
          for(int x=0; x<MINO_WIDTH; x++) {
            if((mino_aa[mino_type][mino_angle][y][x])
              && (field[y+mino_y][x+mino_x])) {
              exit_flag = 1;
              //end();
              init();
            }
          }
        }
        if(exit_flag) {
          continue;
        }
      }

      int dest_x, min_x, max_x, low = FIELD_HEIGHT;
      for(int _angle=MINO_ANGLE_0; _angle<MINO_ANGLE_MAX; _angle++) {
        if(low<0)
          continue;
        min_x = max_x = mino_x;
        while(!cd(min_x-1, mino_y, _angle)) min_x--;
        while(!cd(max_x+1, mino_y, _angle)) max_x++;
        for(int _x=min_x; _x<=max_x; _x++) {
          int _y = 0, h = FIELD_HEIGHT;
          while(!cd(_x, _y+1, _angle)) {
            _y++;
            h--;
          }

          // もしラインが消せるなら, そこにする
          {
            int tmp[FIELD_HEIGHT][FIELD_WIDTH];
            memcpy(tmp, field, sizeof field);
            for(int y=0; y<MINO_HEIGHT; y++)
              for(int x=0; x<MINO_WIDTH; x++)
                if(mino_aa[mino_type][_angle][y][x])
                  tmp[y+_y][x+_x] = BLOCK_MINO_I + mino_type;
            for(int y=4; y<FIELD_HEIGHT-1; y++) {
              int erase = 1;
              for(int x=1; x<FIELD_WIDTH-1; x++)
                if(tmp[y][x] == BLOCK_NONE)
                  erase = 0;
              if(erase) {
                // なるべく穴が開かないようにする
                for(int x=0; x<MINO_WIDTH; x++)
                  if(tmp[_y+1][x] == BLOCK_NONE)
                    low = 0;
                low = -1;
                dest_x = _x;
                mino_angle = _angle;
              }
            }
          }

          // 一番低くなるように積む
          if( (low > h)                     // 一番低い
           || ((low == h) && (rand()%2))) { // 同じなら, 1/2の確立で更新
            low = h;
            dest_x = _x;
            mino_angle = _angle;
          }
          //if(low > h) {
          //  low = h;
          //  dest_x = _x;
          //  mino_angle = _angle;
          //}
          //if((low == h)) {
          //  int hole = 0;
          //  for(int x=0; x<MINO_WIDTH; x++)
          //    if(field[_y+1][x] == BLOCK_NONE)
          //      hole = 1;
          //  if(!hole) {
          //    dest_x = _x;
          //    mino_angle = _angle;
          //  }
          //}
        }
      }
      if(dest_x < mino_x) {
        while(!cd(mino_x-1, mino_y, mino_angle) && dest_x < mino_x) {
          mino_x--;
          draw();
        }
      }
      else if(dest_x > mino_x) {
        while(!cd(mino_x+1, mino_y, mino_angle) && dest_x > mino_x) {
          mino_x++;
          draw();
        }
      }
    }
    draw();
  }
}
