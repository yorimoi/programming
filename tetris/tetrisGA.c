#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 25

#define MINO_WIDTH  4
#define MINO_HEIGHT 4

#define POPULATION 10//0           // 遺伝子集団の数
#define SLECT_GENOM 20           // 遺伝子選択数
#define INDIVIDUAL_MUTATION 0.1  // 個体突然変異確立
#define GENOM_MUTATION 0.1       // 遺伝子突然変異確立
#define GENERATION_MAX 100       // Max世代数


enum { /*{{{*/
  BLOCK_NONE,
  BLOCK_WALL,
  BLOCK_MINO_I,
  BLOCK_MINO_O,
  BLOCK_MINO_S,
  BLOCK_MINO_Z,
  BLOCK_MINO_J,
  BLOCK_MINO_L,
  BLOCK_MINO_T
}; /*}}}*/

enum { /*{{{*/
  MINO_TYPE_I,
  MINO_TYPE_O,
  MINO_TYPE_S,
  MINO_TYPE_Z,
  MINO_TYPE_J,
  MINO_TYPE_L,
  MINO_TYPE_T,
  MINO_TYPE_MAX
}; /*}}}*/

enum { /*{{{*/
  MINO_ANGLE_0,
  MINO_ANGLE_90,
  MINO_ANGLE_180,
  MINO_ANGLE_270,
  MINO_ANGLE_MAX
}; /*}}}*/

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

char block[][19+3+1] = /*{{{*/
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
}; /*}}}*/

typedef struct { /*{{{*/
  int field[FIELD_HEIGHT][FIELD_WIDTH];
  int field_buffer[FIELD_HEIGHT][FIELD_WIDTH];

  int mino_x, mino_y;
  int mino_type;
  int mino_angle;
  int mino_next;

  int mino_stack1[MINO_TYPE_MAX];
  int mino_stack2[MINO_TYPE_MAX];
  int mino_stack_cnt;
} GENOME; /*}}}*/

GENOME genomes[POPULATION];
GENOME *gps = genomes;

int generation = 1;
int g_n = -1;
int draw = 1;


// Collision Detection
int cd(GENOME *g, int _x, int _y, int _angle);

int fixed_above_field(GENOME *g);

int suffer_next(GENOME *g);

// 仮フィールド上の全てのデッドスペースの個数を返す
// ※デッドスペース -> 上下にブロックがあるスペース
int get_all_dead_space(int tmp[][FIELD_WIDTH], int h);

int get_max_height(int tmp[][FIELD_WIDTH]);
void move_x(GENOME *g, int dst);
void line_erase(GENOME *g);
void init_mino(int *mino_stack);
void next_mino(GENOME *g);
void init(GENOME *g);
void display(GENOME *g);
void end();


int main()
{
  srand((unsigned int)time(NULL));
  for(int i=0; i<POPULATION; i++)
    init(gps+i);
  g_n = 0;

  printf("\033[2J\033[?25l");

  while(1) { /*{{{*/
    // 落下出来るなら落下
    if(!cd(gps+g_n, (gps+g_n)->mino_x, (gps+g_n)->mino_y+1, (gps+g_n)->mino_angle))
      (gps+g_n)->mino_y++;
    else {
      // Fixed
      memcpy((gps+g_n)->field, (gps+g_n)->field_buffer, sizeof (gps+g_n)->field);

      // Line erase
      line_erase(gps+g_n);

      // フィールド外に固定されたか
      if(fixed_above_field(gps+g_n))
        continue;

      // Nextミノがフィールドに被ったか
      next_mino(gps+g_n);
      display(gps+g_n);
      if(suffer_next(gps+g_n))
        continue;

      int dest_x, low = FIELD_HEIGHT, min_x, max_x, min_dead_space = 128;
      for(int _angle=MINO_ANGLE_0; _angle<MINO_ANGLE_MAX; _angle++) {
        // Range of X coordinate
        min_x = max_x = (gps+g_n)->mino_x;
        while(!cd(gps+g_n, min_x-1, (gps+g_n)->mino_y, _angle)) min_x--;
        while(!cd(gps+g_n, max_x+1, (gps+g_n)->mino_y, _angle)) max_x++;

        for(int _x=min_x; _x<=max_x; _x++) {
          int _y = 0, h = FIELD_HEIGHT, dead_space_cnt;

          // 仮ミノ落下後の仮フィールド生成
          while(!cd(gps+g_n, _x, _y+1, _angle)) { _y++; h--; }
          int tmp[FIELD_HEIGHT][FIELD_WIDTH];
          memcpy(tmp, (gps+g_n)->field, sizeof (gps+g_n)->field);
          for(int y=0; y<MINO_HEIGHT; y++)
            for(int x=0; x<MINO_WIDTH; x++)
              if(mino_aa[(gps+g_n)->mino_type][_angle][y][x])
                tmp[y+_y][x+_x] = BLOCK_MINO_I + (gps+g_n)->mino_type;

          // 最大の高さ
          int max_h = get_max_height(tmp);

          // 一番低くなるように積む
          if(low > max_h) {
            low = max_h;
            dest_x = _x;
            (gps+g_n)->mino_angle = _angle;
          }

          // 同じ高さならデッドスペースの少ない方を選択
          if(low == max_h) {
            dead_space_cnt = get_all_dead_space(tmp, max_h);
            if(dead_space_cnt < min_dead_space) {
              min_dead_space = dead_space_cnt;
              dest_x = _x;
              (gps+g_n)->mino_angle = _angle;
            }
          }

        }
      }

      // dest_xまでmino_xを移動
      move_x(gps+g_n, dest_x);

    }
    display(gps+g_n);
  } /*}}}*/
}

void move_x(GENOME *g, int dst)
{
  if(dst < g->mino_x) {
    while(!cd(g, g->mino_x-1, g->mino_y, g->mino_angle) && dst < g->mino_x) {
      g->mino_x--;
      display(g);
    }
  }
  else if(dst > g->mino_x) {
    while(!cd(g, g->mino_x+1, g->mino_y, g->mino_angle) && dst > g->mino_x) {
      g->mino_x++;
      display(g);
    }
  }
}

int get_max_height(int tmp[][FIELD_WIDTH])
{
  int height = 0, flag;
  for(int y=FIELD_HEIGHT-2; y>0; y--) {
    flag = 0;
    for(int x=1; x<FIELD_WIDTH-1; x++)
      if(tmp[y][x] != BLOCK_NONE)
        flag = 1;
    if(flag)
      height++;
    else
      return height;
  }
  return -1;
}

int get_all_dead_space(int tmp[][FIELD_WIDTH], int h)
{
  int cnt = 0;
  for(int y=FIELD_HEIGHT-2; y>FIELD_HEIGHT-2-h; y--) {
    for(int x=1; x<FIELD_WIDTH-1; x++) {
      if(tmp[y][x] == BLOCK_NONE) {
        int space = 0;
        for(int y2=y-1; y2>4; y2--)
          if(tmp[y2][x] != BLOCK_NONE) space = 1;
        if(space) cnt++;
      }
    }
  }
  return cnt;
}

void line_erase(GENOME *g)
{
  for(int y=4; y<FIELD_HEIGHT-1; y++) {
    int erase = 1;
    for(int x=1; x<FIELD_WIDTH-1; x++)
      if(g->field[y][x] == BLOCK_NONE)
        erase = 0;
    if(erase) {
      for(int y2=y; y2>0; y2--)
        for(int x=1; x<FIELD_WIDTH-1; x++)
          g->field[y2][x] = g->field[y2-1][x];
    }
  }
}

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
  memcpy(g->field_buffer, g->field, sizeof g->field);
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if(mino_aa[g->mino_type][g->mino_angle][y][x])
        g->field_buffer[y+g->mino_y][x+g->mino_x]
          = BLOCK_MINO_I + g->mino_type;

  printf("\033[H");

  printf("\033[1;%dH", FIELD_WIDTH*2+2);
  printf("generation: %03d", generation);

  printf("\033[2;%dH", FIELD_WIDTH*2+2);
  printf("\033[0mg_n: %03d", g_n);

  printf("\033[H");

  if(!draw)
    return;

  printf("\033[3;%dH", FIELD_WIDTH*2+2);
  printf("next: ");
  //for(int i=1; i<MINO_HEIGHT; i++) {
  //  printf("\033[%d;%dH", i+4, FIELD_WIDTH*2+3);
  //  for(int j=0; j<MINO_WIDTH; j++) {
  //    if(mino_aa[g->mino_next][MINO_ANGLE_0][i][j])
  //      printf("%s", block[BLOCK_MINO_I + g->mino_next]);
  //    else
  //      printf("  ");
  //  }
  //}

  // Next 6 {{{
  // 表示しない. 最終的に6手先まで読めたら良いな的な
  //if(g->mino_stack_cnt+1 < MINO_TYPE_MAX) {
  //  for(int i=g->mino_stack_cnt+1; i<MINO_TYPE_MAX; i++)
  //    printf("\033[34m%d/", g->mino_stack1[i]);
  //  for(int i=0; i<g->mino_stack_cnt; i++)
  //    printf("\033[31m%d/", g->mino_stack2[i]);
  //} else {
  //  for(int i=0; i<MINO_TYPE_MAX-1; i++)
  //    printf("\033[31m%d/", g->mino_stack2[i]);
  //} /*}}}*/

  // Next 6 AA 表示してみたくなっただけ /*{{{*/
  int next_cnt = 1;
  if(g->mino_stack_cnt+1 < MINO_TYPE_MAX) {
    for(int k=g->mino_stack_cnt+1; k<MINO_TYPE_MAX; k++) {
      for(int i=1; i<MINO_HEIGHT; i++) {
        printf("\033[%d;%dH", i+1+next_cnt*3, FIELD_WIDTH*2+3);
        for(int j=0; j<MINO_WIDTH; j++) {
          if(mino_aa[g->mino_stack1[k]][MINO_ANGLE_0][i][j])
            printf("%s", block[BLOCK_MINO_I + g->mino_stack1[k]]);
          else
            printf("  ");
        }
      }
      next_cnt++;
    }
    for(int k=0; k<g->mino_stack_cnt; k++) {
      for(int i=1; i<MINO_HEIGHT; i++) {
        printf("\033[%d;%dH", i+1+next_cnt*3, FIELD_WIDTH*2+3);
        for(int j=0; j<MINO_WIDTH; j++) {
          if(mino_aa[g->mino_stack2[k]][MINO_ANGLE_0][i][j])
            printf("%s", block[BLOCK_MINO_I + g->mino_stack2[k]]);
          else
            printf("  ");
        }
      }
      next_cnt++;
    }
  } else {
    for(int k=0; k<MINO_TYPE_MAX-1; k++) {
      for(int i=1; i<MINO_HEIGHT; i++) {
        printf("\033[%d;%dH", i+1+(k+1)*3, FIELD_WIDTH*2+3);
        for(int j=0; j<MINO_WIDTH; j++) {
          if(mino_aa[g->mino_stack2[k]][MINO_ANGLE_0][i][j])
            printf("%s", block[BLOCK_MINO_I + g->mino_stack2[k]]);
          else
            printf("  ");
        }
      }
    }
  } /*}}}*/

  printf("\033[H");

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

int cd(GENOME *g, int _x, int _y, int _angle)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if((mino_aa[g->mino_type][_angle][y][x])
        && (g->field[_y+y][_x+x] != BLOCK_NONE))
        return 1;
  return 0;
}

int fixed_above_field(GENOME *g)
{
  int cnt = 0;
  for(int y=0; y<MINO_HEIGHT; y++) {
    for(int x=0; x<MINO_WIDTH; x++) {
      if(g->field[y+g->mino_y][x+g->mino_x]
          && mino_aa[g->mino_type][g->mino_angle][y][x]
          && (g)->mino_y+y<MINO_HEIGHT) {
        if(++cnt==4) {
          //end();
          draw = 0;
          init(g);
          return 1;
        }
      }
    }
  }
  return 0;
}

int suffer_next(GENOME *g)
{
  for(int y=0; y<MINO_HEIGHT; y++) {
    for(int x=0; x<MINO_WIDTH; x++) {
      if((mino_aa[g->mino_type][g->mino_angle][y][x])
        && (g->field[y+g->mino_y][x+g->mino_x])) {
        //end();
        draw = 0;
        init(g);
        return 1;
      }
    }
  }
  return 0;
}

void end()
{
  printf("\033[0m\033[?25h");
  printf("Game Over!\n");
  exit(0);
}

