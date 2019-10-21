#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 25

#define MINO_WIDTH  4
#define MINO_HEIGHT 4


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

enum { /*{{{*/
  EVAL_FIELD_HEIGHT,
  EVAL_DEAD_SPACE,
  EVAL_LINE_ERASE,
  EVAL_PROTRUSION,
  EVAL_DIFF,
  EVAL_MAX,
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

//char block[][19+3+1] = /*{{{GitHub*/
//{
//  "\033[38;2;235;237;240m■", // BLOCK_NONE
//  "\033[48;2;255;255;255m  ", // BLOCK_WALL
//  "\033[38;2;198;228;139m■", // BLOCK_MINO_I
//  "\033[38;2;198;228;139m■", // BLOCK_MINO_O
//  "\033[38;2;123;201;111m■", // BLOCK_MINO_S
//  "\033[38;2;123;201;111m■", // BLOCK_MINO_Z
//  "\033[38;2;035;154;059m■", // BLOCK_MINO_J
//  "\033[38;2;035;154;059m■", // BLOCK_MINO_L
//  "\033[38;2;025;097;039m■", // BLOCK_MINO_T
//}; /*}}}*/

int field[FIELD_HEIGHT][FIELD_WIDTH];
int field_buffer[FIELD_HEIGHT][FIELD_WIDTH];

int mino_x, mino_y;
int mino_type;
int mino_angle;
int mino_next;

int mino_stack[MINO_TYPE_MAX];

int score;

int genos[EVAL_MAX];

// typedef int bool;
int draw       = 1;   // 描画するか否か

int elite_of_elite[EVAL_MAX];


// Collision Detection
int cd(int _x, int _y, int _angle);

int cd_next(int tmp[][FIELD_WIDTH], int _x, int _y, int _angle, int _type);

// フィールドより上に固定されたか
int fixed_above_field();

// ネクストミノがフィールドに被っているか
int suffer_next();

// 高低差(各列において右隣の列との高低差の絶対値の合計)
int diff_in_height(int tmp[][FIELD_WIDTH]);

// 仮フィールド上の全てのデッドスペースの個数を返す
// ※デッドスペース -> 上下にブロックがあるスペース
int get_dead_space(int tmp[][FIELD_WIDTH], int h);

// 突出列の数を返す *全体の平均の高さとの差が4以上の列
int protrusion(int tmp[][FIELD_WIDTH]);

// 積まれているブロックの一番上の高さ
int get_max_height(int tmp[][FIELD_WIDTH]);

// 消せるラインがあるなら消し, 埋め, スコア加算
void line_erase();

// 仮ミノ時の消せるライン数を返す
int get_erase_line(int tmp[][FIELD_WIDTH]);

// 仮フィールドのラインを消して, その数を返す
int tmp_erase_line(int tmp[][FIELD_WIDTH]);

// フィールド上の総ブロック数を返す
int get_all_block(int tmp[][FIELD_WIDTH]);

// dest_x(dst)までmino_xを移動
void move_x(int dst);

// ミノ配列の初期化
void init_mino(int *mino_stack);

// 次のミノを生成
void next_mino();

// フィールドの0埋め等
void init();

// 次の個体へシフト
void reset();

// 描画類全て
void display();

// 終了処理
void end();


int main()
{
  srand((unsigned int)time(NULL));

  init();

  // 任意の遺伝子を使う
  {
    int genomes[] =
                    //{ -32, -69,  97, -84, -19}; // 65341, 199-99 Rework
                    {  14, -78,  90, -77, -20}; // , 199-99
    for(int i=0; i<EVAL_MAX; i++)
      genos[i] = genomes[i];
  }

  printf("\033[2J\033[?25l");

  while(1) {
    // 落下出来るなら落下
    if(!cd(mino_x, mino_y+1, mino_angle))
      mino_y++;
    else {
      // 現在のミノをフィールドに固定する
      memcpy(field, field_buffer, sizeof field);

      // 消せるなら消し, 埋める
      line_erase();

      // フィールド外に固定されたか
      if(fixed_above_field())
        continue;

      // Nextミノがフィールドに被ったか
      next_mino();
      display();
      if(suffer_next())
        continue;

      int dest_x = mino_x,
          eval = -65536;

      for(int _angle=MINO_ANGLE_0; _angle<MINO_ANGLE_MAX; _angle++) {
        if(cd(mino_x, mino_y, _angle))
          continue;
        // Range of X coordinate
        int min_x, max_x;
        min_x = max_x = mino_x;
        while(!cd(min_x-1, mino_y, _angle)) min_x--;
        while(!cd(max_x+1, mino_y, _angle)) max_x++;

        for(int _x=min_x; _x<=max_x; _x++) {
          int _y = mino_y;

          // 仮ミノ落下後の仮フィールド生成
          while(!cd(_x, _y+1, _angle)) _y++;
          int tmp[FIELD_HEIGHT][FIELD_WIDTH];
          memcpy(tmp, field, sizeof field);
          for(int y=0; y<MINO_HEIGHT; y++)
            for(int x=0; x<MINO_WIDTH; x++)
              if(mino_aa[mino_type][_angle][y][x])
                tmp[y+_y][x+_x] = BLOCK_MINO_I + mino_type;

          // Nextミノも見る
          {
            int mino_next2 = mino_next;

            int mino_x2 = 4;
            int mino_y2 = mino_next2 == MINO_TYPE_I ? 2 : 1;

            for(int _angle2=MINO_ANGLE_0; _angle2<MINO_ANGLE_MAX; _angle2++) {
              if(cd_next(tmp, mino_x2, mino_y2, _angle2, mino_next2))
                continue;

              int min_x2, max_x2;
              min_x2 = max_x2 = mino_x2;

              while(!cd_next(tmp, min_x2-1, mino_y2, _angle2, mino_next2))
                min_x2--;

              while(!cd_next(tmp, max_x2+1, mino_y2, _angle2, mino_next2))
                max_x2++;

              for(int _x2=min_x2; _x2<=max_x2; _x2++) {
                int _y2 = mino_y2;
                // Next2ミノ落下後の仮フィールド生成
                while(!cd_next(tmp, _x2, _y2+1, _angle2, mino_next2))
                  _y2++;
                int tmp2[FIELD_HEIGHT][FIELD_WIDTH];
                memcpy(tmp2, tmp, sizeof tmp);
                for(int y=0; y<MINO_HEIGHT; y++)
                  for(int x=0; x<MINO_WIDTH; x++)
                    if(mino_aa[mino_next2][_angle2][y][x])
                      tmp2[y+_y2][x+_x2] = BLOCK_MINO_I + mino_next2;


                // 最大の高さ
                // 低いほど良い
                int field_h = get_max_height(tmp2);

                // 消せるライン数
                // 多いほど良い?
                int line = get_erase_line(tmp2);

                // デッドスペースの数
                // 少ない方が良い
                int dead_space_cnt = get_dead_space(tmp2, field_h);

                // 突出列数
                // 少ない方が良い
                int pro_row = protrusion(tmp2);

                // 高低差
                int diff = diff_in_height(tmp2);

                // evaluation()
                field_h *= genos[EVAL_FIELD_HEIGHT];
                line *= genos[EVAL_LINE_ERASE];
                dead_space_cnt *= genos[EVAL_DEAD_SPACE];
                pro_row *= genos[EVAL_PROTRUSION];
                diff *= genos[EVAL_DIFF];

                int _eval = (field_h
                           + line
                           + dead_space_cnt
                           + pro_row
                           + diff
                           );
                if(eval < _eval) {
                  eval = _eval;
                  dest_x = _x;
                  mino_angle = _angle;
                }
              }
            }//_angle2
          }//Next2
        }
      }
      move_x(dest_x);
    }
    display();
  }

  end();
}

void move_x(int dst)
{
  if(dst < mino_x) {
    while(!cd(mino_x-1, mino_y, mino_angle) && dst < mino_x) {
      mino_x--;
      display();
    }
  }
  else if(dst > mino_x) {
    while(!cd(mino_x+1, mino_y, mino_angle) && dst > mino_x) {
      mino_x++;
      display();
    }
  }
}

int diff_in_height(int tmp[][FIELD_WIDTH])
{
  int diff = 0;
  int h[FIELD_WIDTH-2] = {};
  for(int x=1; x<FIELD_WIDTH-1; x++)
    for(int y=0; y<FIELD_HEIGHT-1; y++)
      if(tmp[y][x] != BLOCK_NONE) {
        h[x-1] = FIELD_HEIGHT - 1 - y;
        break;
      }
  for(int i=0; i<FIELD_WIDTH-2-1; i++) {
    int d = h[i] - h[i+1];
    if(d < 0)
      d *= -1;
    diff += d;
  }

  return diff;
}

int protrusion(int tmp[][FIELD_WIDTH])
{
  int row = 0;
  int block_cnt = 0;
  for(int x=1; x<FIELD_WIDTH-1; x++) {
    for(int y=0; y<FIELD_HEIGHT-1; y++) {
      if(tmp[y][x] != BLOCK_NONE) {
        block_cnt += FIELD_HEIGHT - 1 - y;
        break;
      }
    }
  }
  float avg = block_cnt / (FIELD_WIDTH-2);

  for(int x=1; x<FIELD_WIDTH-1; x++) {
    for(int y=0; y<FIELD_HEIGHT-1; y++) {
      if(tmp[y][x] != BLOCK_NONE) {
        if(((FIELD_HEIGHT - 1 - y - avg) >= 4.0f)
            || ((FIELD_HEIGHT - 1 - y - avg) <= -4.0f))
          row++;
        break;
      }
    }
  }
  return row;
}

int get_max_height(int tmp[][FIELD_WIDTH])
{
  for(int y=0; y<FIELD_HEIGHT-1; y++)
    for(int x=1; x<FIELD_WIDTH-1; x++)
      if(tmp[y][x] != BLOCK_NONE)
        return FIELD_HEIGHT - y;
  return -1;
}

int get_dead_space(int tmp[][FIELD_WIDTH], int h)
{
  int cnt = 0;
  for(int y=FIELD_HEIGHT-2; y>FIELD_HEIGHT-2-h; y--) {
    for(int x=1; x<FIELD_WIDTH-1; x++) {
      if(tmp[y][x] == BLOCK_NONE/* && tmp[y+1][x] != BLOCK_NONE*/) {
        int space = 0;
        for(int y2=y-1; y2>4; y2--)
          if(tmp[y2][x] != BLOCK_NONE) y2 = space = 1;
        if(space) cnt++;
      }
    }
  }
  return cnt;
}

void line_erase()
{
  int line = 0;
  for(int y=4; y<FIELD_HEIGHT-1; y++) {
    int erase = 1;
    for(int x=1; x<FIELD_WIDTH-1; x++)
      if(field[y][x] == BLOCK_NONE)
        erase = 0;
    if(erase) {
      line++;
      for(int y2=y; y2>0; y2--)
        for(int x=1; x<FIELD_WIDTH-1; x++)
          field[y2][x] = field[y2-1][x];
    }
  }
  switch(line) {
    case 1: score += 40;   break;
    case 2: score += 100;  break;
    case 3: score += 300;  break;
    case 4: score += 1200; break;
  }
}

int get_erase_line(int tmp[][FIELD_WIDTH])
{
  int line = 0;
  for(int y=4; y<FIELD_HEIGHT-1; y++) {
    int erase = 1;
    for(int x=1; x<FIELD_WIDTH-1; x++)
      if(tmp[y][x] == BLOCK_NONE)
        erase = 0;
    if(erase)
      line++;
  }
  return line;
}

int tmp_erase_line(int tmp[][FIELD_WIDTH])
{
  int line = 0;
  for(int y=4; y<FIELD_HEIGHT-1; y++) {
    int erase = 1;
    for(int x=1; x<FIELD_WIDTH-1; x++)
      if(tmp[y][x] == BLOCK_NONE)
        erase = 0;
    if(erase) {
      line++;
      for(int y2=y; y2>0; y2--)
        for(int x=1; x<FIELD_WIDTH-1; x++)
          tmp[y2][x] = tmp[y2-1][x];
    }
  }
  return line;
}

int get_all_block(int tmp[][FIELD_WIDTH])
{
  int cnt = 0;
  for(int y=4; y<FIELD_HEIGHT-1; y++)
    for(int x=1; x<FIELD_WIDTH-1; x++)
      if(tmp[y][x] != BLOCK_NONE)
        cnt++;
  return cnt;
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

void next_mino()
{
  // mino_stack 初期化
  // ~

  mino_type = mino_stack[0];
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
  mino_next = mino_stack[1];
}

void init()
{
}

void reset()
{
  //end();
  draw = 1;
  init();
}

void display()
{
  memcpy(field_buffer, field, sizeof field);
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if(mino_aa[mino_type][mino_angle][y][x])
        field_buffer[y+mino_y][x+mino_x]
          = BLOCK_MINO_I + mino_type;

  if(!draw)
    return;

  printf("\033[H");

  printf("\033[5;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
  printf("\033[0mscore: %07d", score);

  printf("\033[3;%dH", FIELD_WIDTH*2+2);
  printf(" next: ");
  /*{{{*/
  for(int i=1; i<MINO_HEIGHT; i++) {
    printf("\033[%d;%dH", i+4, FIELD_WIDTH*2+3);
    for(int j=0; j<MINO_WIDTH; j++) {
      if(mino_aa[mino_next][MINO_ANGLE_0][i][j])
        printf("%s", block[BLOCK_MINO_I + mino_next]);
      else
        printf("  ");
    }
  } /*}}}*/

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

  printf("\033[H");

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
  printf("\033[0m");
}

int cd(int _x, int _y, int _angle)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if((mino_aa[mino_type][_angle][y][x])
        && (field[_y+y][_x+x] != BLOCK_NONE))
        return 1;
  return 0;
}

int cd_next(int tmp[][FIELD_WIDTH], int _x, int _y, int _angle, int _type)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if((mino_aa[_type][_angle][y][x])
        && (tmp[_y+y][_x+x] != BLOCK_NONE))
        return 1;
  return 0;
}

int fixed_above_field()
{
  int cnt = 0;
  for(int y=0; y<MINO_HEIGHT; y++) {
    for(int x=0; x<MINO_WIDTH; x++) {
      if(field[y+mino_y][x+mino_x]
          && mino_aa[mino_type][mino_angle][y][x]
          && mino_y+y<MINO_HEIGHT) {
        if(++cnt==4) {
          reset();
          return 1;
        }
      }
    }
  }
  return 0;
}

int suffer_next()
{
  for(int y=0; y<MINO_HEIGHT; y++) {
    for(int x=0; x<MINO_WIDTH; x++) {
      if((mino_aa[mino_type][mino_angle][y][x])
        && (field[y+mino_y][x+mino_x])) {
        reset();
        return 1;
      }
    }
  }
  return 0;
}

void end()
{
  printf("\033[0m\033[?25h");
  exit(0);
}

