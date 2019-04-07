#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 25

#define MINO_WIDTH  4
#define MINO_HEIGHT 4

#define GENOM_LENGTH 20          // 遺伝子情報の長さ
#define POPULATION 10        // 遺伝子集団の数
#define SLECT_GENOM 20           // 遺伝子選択数
#define INDIVIDUAL_MUTATION 0.1  // 個体突然変異確立
#define GENOM_MUTATION 0.1       // 遺伝子突然変異確立
#define GENERATION_MAX 100       // Max世代数


void end();


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


typedef struct {
  int field[FIELD_HEIGHT][FIELD_WIDTH];
  int field_buffer[FIELD_HEIGHT][FIELD_WIDTH];

  int mino_x, mino_y;
  int mino_type;
  int mino_angle;
  int mino_next;

  int mino_stack1[MINO_TYPE_MAX];
  int mino_stack2[MINO_TYPE_MAX];
  int mino_stack_cnt;
} GENOME;

GENOME genomes[POPULATION];
GENOME *gps = genomes;

int generation = 1;
int g_n = -1;
int draw = 1;


void init_mino(int *mino_stack)
{
  for(int i=MINO_TYPE_MAX-1; i>0; i--) {
    int mix = rand() % i;
    int tmp = *(mino_stack+i);
    *(mino_stack+i) = *(mino_stack+mix);
    *(mino_stack+mix) = tmp;
 }
}

void next_mino(GENOME *g)
{
  if(MINO_TYPE_MAX <= ++g->mino_stack_cnt) {
    g->mino_stack_cnt = 0;
    memcpy(g->mino_stack1, g->mino_stack2, sizeof(int)*MINO_TYPE_MAX);
    init_mino(g->mino_stack2);
  }
  g->mino_type = g->mino_stack1[g->mino_stack_cnt];
  g->mino_angle = 0;
  g->mino_x     = 4;
  switch(g->mino_type) {
    case MINO_TYPE_J:
    case MINO_TYPE_L:
    case MINO_TYPE_O:
    case MINO_TYPE_S:
    case MINO_TYPE_Z:
    case MINO_TYPE_T: g->mino_y = 1; break;
    case MINO_TYPE_I: g->mino_y = 2; break;
  }
  if(g->mino_stack_cnt+1 < MINO_TYPE_MAX)
    g->mino_next = g->mino_stack1[g->mino_stack_cnt+1];
  else
    g->mino_next = g->mino_stack2[0];
}

void init(GENOME *g)
{
  if(++g_n < POPULATION) {
    memset(g->field, BLOCK_NONE, sizeof g->field);
    //for(int y=4; y<FIELD_HEIGHT; y++)
    for(int y=0; y<FIELD_HEIGHT; y++)
      g->field[y][0] = g->field[y][FIELD_WIDTH-1] = BLOCK_WALL;
    for(int x=1; x<FIELD_WIDTH-1; x++)
      g->field[FIELD_HEIGHT-1][x] = BLOCK_WALL;
    for(int j=0; j<MINO_TYPE_MAX; j++)
      g->mino_stack1[j] = g->mino_stack2[j] = j;
    init_mino(g->mino_stack1);
    init_mino(g->mino_stack2);
    g->mino_stack_cnt = -1;
    next_mino(gps+g_n);
  }
  else {
    //end();
    generation++;
    draw = 1;
    g_n  = -1;
    init(g);
  }
}

void display(GENOME *g)
{
  printf("\033[H");

  printf("\033[1;%dH", FIELD_WIDTH*2+1);
  printf("generation: %d", generation);
  //printf("next: ");
  //if(g->mino_stack_cnt+1 < MINO_TYPE_MAX) {
  //  for(int i=g->mino_stack_cnt+1; i<MINO_TYPE_MAX; i++)
  //    printf("\033[34m%d/", g->mino_stack1[i]);
  //  for(int i=0; i<g->mino_stack_cnt; i++)
  //    printf("\033[31m%d/", g->mino_stack2[i]);
  //} else {
  //  for(int i=0; i<MINO_TYPE_MAX-1; i++)
  //    printf("\033[31m%d/", g->mino_stack2[i]);
  //}
  printf("\033[2;%dH", FIELD_WIDTH*2+1);
  printf("\033[0mg_n: %d", g_n);
  printf("\033[H");

  memcpy(g->field_buffer, g->field, sizeof g->field);
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if(mino_aa[g->mino_type][g->mino_angle][y][x])
        g->field_buffer[y+g->mino_y][x+g->mino_x]
          = BLOCK_MINO_I + g->mino_type;

  if(!draw)
    return;

  // 見えない壁
  for(int y=2; y<4; y++) {
    printf("  ");
    for(int x=1; x<FIELD_WIDTH-1; x++)
      printf("%s", block[g->field_buffer[y][x]]);
    printf("\n");
  }
  // Main g->field
  for(int y=4; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++) {
      printf("%s", block[g->field_buffer[y][x]]);
    }
    printf("\n");
  }
}

// Collision Detection
int cd(GENOME *g, int _x, int _y, int _angle)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if(
        (mino_aa[g->mino_type][_angle][y][x])
        && (g->field[_y+y][_x+x] != BLOCK_NONE)
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
  for(int i=0; i<POPULATION; i++)
    init(gps+i);
  g_n = 0;

  printf("\033[2J\033[?25l");

  //pid_t pid = fork();
  //if(pid == -1) {
  //  perror("fork");
  //  return 1;
  //}

  while(1) { /*{{{*/
    if(!cd(gps+g_n, (gps+g_n)->mino_x, (gps+g_n)->mino_y+1, (gps+g_n)->mino_angle))
      (gps+g_n)->mino_y++;
    else {
      memcpy((gps+g_n)->field, (gps+g_n)->field_buffer, sizeof (gps+g_n)->field);
      for(int y=4; y<FIELD_HEIGHT-1; y++) {
        int erase = 1;
        for(int x=1; x<FIELD_WIDTH-1; x++)
          if((gps+g_n)->field[y][x] == BLOCK_NONE)
            erase = 0;
        if(erase) {
          for(int y2=y; y2>0; y2--)
            for(int x=1; x<FIELD_WIDTH-1; x++)
              (gps+g_n)->field[y2][x] = (gps+g_n)->field[y2-1][x];
        }
      }

      // フィールド外に固定されたか
      {
        int exit_flag = 0, cnt = 0;
        for(int y=0; y<MINO_HEIGHT; y++)
          for(int x=0; x<MINO_WIDTH; x++)
            if((gps+g_n)->field[y+(gps+g_n)->mino_y][x+(gps+g_n)->mino_x]
                && mino_aa[(gps+g_n)->mino_type][(gps+g_n)->mino_angle][y][x]
                && (gps+g_n)->mino_y+y<MINO_HEIGHT)
              cnt++;
        if(cnt==4) {
          exit_flag = 1;
          //end();
          draw = 0;
          init(gps+g_n);
        }
        if(exit_flag) {
          continue;
        }
      }

      // Nextミノがフィールドに被ったか
      {
        int exit_flag = 0;
        next_mino(gps+g_n);
        display(gps+g_n);
        for(int y=0; y<MINO_HEIGHT; y++) {
          for(int x=0; x<MINO_WIDTH; x++) {
            if((mino_aa[(gps+g_n)->mino_type][(gps+g_n)->mino_angle][y][x])
              && ((gps+g_n)->field[y+(gps+g_n)->mino_y][x+(gps+g_n)->mino_x])) {
              exit_flag = 1;
              //end();
              draw = 0;
              init(gps+g_n);
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
        min_x = max_x = (gps+g_n)->mino_x;
        while(!cd(gps+g_n, min_x-1, (gps+g_n)->mino_y, _angle)) min_x--;
        while(!cd(gps+g_n, max_x+1, (gps+g_n)->mino_y, _angle)) max_x++;
        for(int _x=min_x; _x<=max_x; _x++) {
          int _y = 0, h = FIELD_HEIGHT;
          while(!cd(gps+g_n, _x, _y+1, _angle)) {
            _y++;
            h--;
          }

          /*
          // もしラインが消せるなら, そこにする
          {
            int tmp[FIELD_HEIGHT][FIELD_WIDTH];
            memcpy(tmp, (gps+g_n)->field, sizeof (gps+g_n)->field);
            for(int y=0; y<MINO_HEIGHT; y++)
              for(int x=0; x<MINO_WIDTH; x++)
                if(mino_aa[(gps+g_n)->mino_type][_angle][y][x])
                  tmp[y+_y][x+_x] = BLOCK_MINO_I + (gps+g_n)->mino_type;

            // なるべく穴が開かないようにする
            int hole = 0, cnt = 0, _h = -1;
            for(int y=0; y<MINO_HEIGHT; y++)
              for(int x=0; x<MINO_WIDTH; x++) {
                if(mino_aa[(gps+g_n)->mino_type][_angle][y][x])
                  cnt++;
                if(cnt==4 && _h<0)
                  _h = y;
              }
            for(int x=0; x<MINO_WIDTH; x++)
              if(tmp[_h+1][x] == BLOCK_NONE)
                hole = 1;
            if(!hole) {
              low = -1;
              dest_x = _x;
              (gps+g_n)->mino_angle = _angle;
              continue;
            }

            for(int y=4; y<FIELD_HEIGHT-1; y++) {
              int erase = 1;
              for(int x=1; x<FIELD_WIDTH-1; x++)
                if(tmp[y][x] == BLOCK_NONE)
                  erase = 0;
              if(erase) {
                low = -1;
                dest_x = _x;
                (gps+g_n)->mino_angle = _angle;
              }
            }
          } //*/

          // 一番低くなるように積む
          if(low > h) {
            low = h;
            dest_x = _x;
            (gps+g_n)->mino_angle = _angle;
          }
          if((low == h)) {
            int hole = 0, cnt = 0, y;
            for(y=0; y<MINO_HEIGHT; y++) {
              for(int x=0; x<MINO_WIDTH; x++)
                if(mino_aa[(gps+g_n)->mino_type][_angle][y][x]) cnt++;
              if(cnt==4)
                break;
            }
            for(int x=0; x<MINO_WIDTH; x++)
              if((gps+g_n)->field[_y+y+1][x] == BLOCK_NONE)
                hole = 1;
            if((!hole) && (rand()%2)) {
              dest_x = _x;
              (gps+g_n)->mino_angle = _angle;
            }
          }
        }
      }
      if(dest_x < (gps+g_n)->mino_x) {
        while(!cd(gps+g_n,
              (gps+g_n)->mino_x-1, (gps+g_n)->mino_y, (gps+g_n)->mino_angle)
            && dest_x < (gps+g_n)->mino_x) {
          (gps+g_n)->mino_x--;
          display(gps+g_n);
        }
      }
      else if(dest_x > (gps+g_n)->mino_x) {
        while(!cd(gps+g_n,
              (gps+g_n)->mino_x+1, (gps+g_n)->mino_y, (gps+g_n)->mino_angle)
            && dest_x > (gps+g_n)->mino_x) {
          (gps+g_n)->mino_x++;
          display(gps+g_n);
        }
      }
    }
    display(gps+g_n);
  } /*}}}*/
}
