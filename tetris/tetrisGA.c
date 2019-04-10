#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 25

#define MINO_WIDTH  4
#define MINO_HEIGHT 4

#define ELITE_LENGTH 5           // 選択するエリート(%)
#define POPULATION 100           // 遺伝子集団の数
#define INDIVIDUAL_MUTATION 1    // 個体突然変異確立(%)
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

enum { /*{{{*/
  EVAL_FIELD_HEIGHT,
  EVAL_MINO_HEIGHT,
  EVAL_DEAD_SPACE,
  EVAL_LINE_ERASE,
  EVAL_PROTRUSION,
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

//char block[][19+3+1] = /*{{{*/
//{
//  "\033[38;2;000;000;000m  ", // BLOCK_NONE
//  "\033[38;2;240;240;240m■", // BLOCK_WALL
//  "\033[38;2;000;255;255m■", // BLOCK_MINO_I
//  "\033[38;2;255;255;000m■", // BLOCK_MINO_O
//  "\033[38;2;000;224;000m■", // BLOCK_MINO_S
//  "\033[38;2;255;000;000m■", // BLOCK_MINO_Z
//  "\033[38;2;032;128;255m■", // BLOCK_MINO_J
//  "\033[38;2;255;128;000m■", // BLOCK_MINO_L
//  "\033[38;2;192;064;192m■", // BLOCK_MINO_T
//}; /*}}}*/

char block[][19+3+1] = /*{{{GitHub*/
{
  "\033[38;2;235;237;240m■", // BLOCK_NONE
  "\033[48;2;255;255;255m  ", // BLOCK_WALL
  "\033[38;2;198;228;139m■", // BLOCK_MINO_I
  "\033[38;2;198;228;139m■", // BLOCK_MINO_O
  "\033[38;2;123;201;111m■", // BLOCK_MINO_S
  "\033[38;2;123;201;111m■", // BLOCK_MINO_Z
  "\033[38;2;035;154;059m■", // BLOCK_MINO_J
  "\033[38;2;035;154;059m■", // BLOCK_MINO_L
  "\033[38;2;025;097;039m■", // BLOCK_MINO_T
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

  int score;
  int block_total;
  int line_total;

  int genos[EVAL_MAX];

} GENOME; /*}}}*/

GENOME genomes[POPULATION];
GENOME *gps = genomes;

int generation = 1;   // 現在の世代
int g_n        = -1;  // 現在の個体

// typedef int bool;
int draw       = 1;   // 描画するか否か
int devote     = 0;   // 学習に専念するか


// Collision Detection
int cd(GENOME *g, int _x, int _y, int _angle);

int cd_next(int tmp[][FIELD_WIDTH], int _x, int _y, int _angle, int _type);

// フィールドより上に固定されたか
int fixed_above_field(GENOME *g);

// ネクストミノがフィールドに被っているか
int suffer_next(GENOME *g);

// 仮フィールド上の全てのデッドスペースの個数を返す
// ※デッドスペース -> 上下にブロックがあるスペース
int get_dead_space(int tmp[][FIELD_WIDTH], int h);

// 突出列の数を返す *全体の平均の高さとの差が4以上の列
int protrusion(int tmp[][FIELD_WIDTH]);

// 積まれているブロックの一番上の高さ
int get_max_height(int tmp[][FIELD_WIDTH]);

// 消せるラインがあるなら消し, 埋め, スコア加算, 消したライン数を返す
int line_erase(GENOME *g);

// 仮ミノ時の消せるライン数を返す
int get_erase_line(int tmp[][FIELD_WIDTH]);

// フィールド上の総ブロック数を返す
int get_all_block(int tmp[][FIELD_WIDTH]);

// dest_x(dst)までmino_xを移動
void move_x(GENOME *g, int dst);

// ミノ配列の初期化
void init_mino(int *mino_stack);

// 次のミノを生成
void next_mino(GENOME *g);

// フィールドの0埋め等
void init(GENOME *g);

// エリートの選択
void select_elite();

// 交叉
void crossover();

// 突然変異
void mutation();

// 累乗
int _pow(int x, int y);

// クイックソート
void quick_sort(int *ptr, int *idx, int left, int right);

// Swap
void swap(int *ptr1, int *ptr2);

// 描画類全て
void display(GENOME *g);

// 終了処理
void end();


int main()
{
  srand((unsigned int)time(NULL));

  // 1世代目の遺伝子の初期化
  for(int i=0; i<POPULATION; i++)
    for(int j=0; j<EVAL_MAX; j++)
      (gps+i)->genos[j] = rand() % 100;

  // 任意のエリートを使う
  //{
  //  int elite[] = { 33, 21, 53, 90, 75 };
  //                //{ 31, 13, 56, 79, 15 };
  //  for(int i=0; i<EVAL_MAX; i++)
  //    (gps+0)->genos[i] = elite[i];
  //}

  for(int i=0; i<POPULATION; i++)
    init(gps+i);

  g_n = 0;

  printf("\033[2J\033[?25l");

  while(1) {
    // 落下出来るなら落下
    if(!cd(gps+g_n, (gps+g_n)->mino_x, (gps+g_n)->mino_y+1, (gps+g_n)->mino_angle))
      (gps+g_n)->mino_y++;
    else {
      // 現在のミノをフィールドに固定する
      memcpy((gps+g_n)->field, (gps+g_n)->field_buffer, sizeof (gps+g_n)->field);

      // 積んだブロックの合計
      (gps+g_n)->block_total++;

      // 消せるなら消し, 埋め, スコア加算
      // 消えたラインの合計に加算
      (gps+g_n)->line_total += line_erase(gps+g_n);

      // フィールド外に固定されたか
      if(fixed_above_field(gps+g_n))
        continue;

      // Nextミノがフィールドに被ったか
      next_mino(gps+g_n);
      display(gps+g_n);
      if(suffer_next(gps+g_n))
        continue;

      int dest_x = (gps+g_n)->mino_x,
          eval = -65536;

      for(int _angle=MINO_ANGLE_0; _angle<MINO_ANGLE_MAX; _angle++) {
        if(cd(gps+g_n, (gps+g_n)->mino_x, (gps+g_n)->mino_y, _angle))
          continue;
        // Range of X coordinate
        int min_x, max_x;
        min_x = max_x = (gps+g_n)->mino_x;
        while(!cd(gps+g_n, min_x-1, (gps+g_n)->mino_y, _angle)) min_x--;
        while(!cd(gps+g_n, max_x+1, (gps+g_n)->mino_y, _angle)) max_x++;

        for(int _x=min_x; _x<=max_x; _x++) {
          int _y = (gps+g_n)->mino_y;

          // 仮ミノ落下後の仮フィールド生成
          while(!cd(gps+g_n, _x, _y+1, _angle)) _y++;
          int tmp[FIELD_HEIGHT][FIELD_WIDTH];
          memcpy(tmp, (gps+g_n)->field, sizeof (gps+g_n)->field);
          for(int y=0; y<MINO_HEIGHT; y++)
            for(int x=0; x<MINO_WIDTH; x++)
              if(mino_aa[(gps+g_n)->mino_type][_angle][y][x])
                tmp[y+_y][x+_x] = BLOCK_MINO_I + (gps+g_n)->mino_type;


          // Nextミノも見る
          {
            int mino_next2;
            if(MINO_TYPE_MAX <= (gps+g_n)->mino_stack_cnt+1)
              mino_next2 = (gps+g_n)->mino_stack2[0];
            else
              mino_next2 = (gps+g_n)->mino_stack1[(gps+g_n)->mino_stack_cnt+1];

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


                /* マイナスにならないように調整する */

                // 最大の高さ
                // 低いほど良い
                int field_h = FIELD_HEIGHT - get_max_height(tmp2);

                // 最大の高さ(置いた仮ミノがyの上限)
                // 低いほど良い
                int mino_h  = _y;

                // 消せるライン数
                // 多いほど良い
                int line    = get_erase_line(tmp2);

                // デッドスペースの数
                // 少ない方が良い
                int dead_space_cnt
                  = (field_h * (FIELD_WIDTH - 2)) / 2
                    - get_dead_space(tmp2, field_h);

                // 突出列数
                // 少ない方が良い
                int pro_row = FIELD_WIDTH - 2 - protrusion(tmp2);

                // フィールド上のブロックの数
                // 多い方がはみ出てない
                // これは重みを変える必要ない
                // うまくいかない ;(
                int all_block = get_all_block(tmp2);

                // Iミノ置きたい

                // evaluation()
                field_h *= (gps+g_n)->genos[EVAL_FIELD_HEIGHT];
                mino_h *= (gps+g_n)->genos[EVAL_MINO_HEIGHT];
                line *= (gps+g_n)->genos[EVAL_LINE_ERASE];
                dead_space_cnt *= (gps+g_n)->genos[EVAL_DEAD_SPACE];
                pro_row *= (gps+g_n)->genos[EVAL_PROTRUSION];

                if((field_h<0)||(mino_h<0)||(dead_space_cnt<0)||(pro_row<0)) {
                  printf("\a%d,%d,%d,%d", field_h, mino_h, dead_space_cnt, pro_row);
                  getchar();
                }

                int _eval = (field_h + mino_h + line + dead_space_cnt
                            + pro_row) * all_block;
                if(eval < _eval) {
                  eval = _eval;
                  dest_x = _x;
                  (gps+g_n)->mino_angle = _angle;
                }
              }
            }//_angle2
          }

          // 評価関数のデバッグ用
          //{
          //  // 一番低くなるように積む
          //  if(max_mino_h > mino_h) {
          //    max_mino_h = mino_h;
          //    dest_x = _x;
          //    (gps+g_n)->mino_angle = _angle;
          //  }

          //  // 同じ高さならデッドスペースの少ない方を選択
          //  if(max_mino_h == mino_h) {
          //    dead_space_cnt = get_dead_space(tmp, field_h);
          //    if(min_dead_space > dead_space_cnt) {
          //      min_dead_space = dead_space_cnt;
          //      dest_x = _x;
          //      (gps+g_n)->mino_angle = _angle;
          //    }
          //  }
          //}

        }
      }
      move_x(gps+g_n, dest_x);
    }
    display(gps+g_n);
  }

  end();
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

int protrusion(int tmp[][FIELD_WIDTH])
{
  int row = 0;
  int block_cnt = 0;
  for(int y=0; y<FIELD_HEIGHT-1; y++)
    for(int x=1; x<FIELD_WIDTH-1; x++)
      if(tmp[y][x] != BLOCK_NONE)
        //block_cnt++;
        block_cnt += FIELD_HEIGHT - y;
  float avg = block_cnt / (FIELD_WIDTH-2);

  //for(int i=1; i<FIELD_WIDTH-1; i++) {
  //  float cnt = 0.0f;
  //  for(int j=FIELD_HEIGHT-1; j>0; j--) {
  //    if(tmp[i][j] != BLOCK_NONE)
  //      cnt += 1.0f;
  //    else if(((cnt-avg)>=4.0f) || ((cnt-avg)<=-4.0f)) {
  //      row++;
  //      break;
  //    }
  //  }
  //}
  for(int x=1; x<FIELD_WIDTH-1; x++) {
    for(int y=0; y<FIELD_HEIGHT-1; y++) {
      if(tmp[y][x] != BLOCK_NONE) {
        if(((FIELD_HEIGHT - y - avg) >= 4.0f)
            || ((FIELD_HEIGHT - y - avg) <= -4.0f))
          row++;
        break;
      }
    }
  }
  return row;
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

int line_erase(GENOME *g)
{
  int line = 0;
  for(int y=4; y<FIELD_HEIGHT-1; y++) {
    int erase = 1;
    for(int x=1; x<FIELD_WIDTH-1; x++)
      if(g->field[y][x] == BLOCK_NONE)
        erase = 0;
    if(erase) {
      line++;
      for(int y2=y; y2>0; y2--)
        for(int x=1; x<FIELD_WIDTH-1; x++)
          g->field[y2][x] = g->field[y2-1][x];
    }
  }
  switch(line) {
    case 1: g->score += 40;   break;
    case 2: g->score += 100;  break;
    case 3: g->score += 300;  break;
    case 4: g->score += 1200; break;
  }

  return line;
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
  // I Z only
  //for(int i=0; i<MINO_TYPE_MAX; i++)
  //  *(mino_stack+i) = (rand()%2) ? 0 : 3;

  // S Z mashi mashi
  //for(int i=0; i<MINO_TYPE_MAX; i++) {
  //  int r = rand() % (MINO_TYPE_MAX + 2);
  //  *(mino_stack+i) =
  //    r<1 ? MINO_TYPE_I:
  //    r<2 ? MINO_TYPE_O:
  //    r<4 ? MINO_TYPE_S: // x2
  //    r<6 ? MINO_TYPE_Z: // x2
  //    r<7 ? MINO_TYPE_J:
  //    r<8 ? MINO_TYPE_L:
  //          MINO_TYPE_T;
  //}
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
    g->score = 0;
    g->block_total = 0;
    g->line_total = 0;
  }
  else {
    select_elite();

    //end();
    generation++;
    //if(generation%10==0)
      draw = 1;
    g_n  = -1;
    init(g);
  }
}

void select_elite()
{
  int _block_total[POPULATION];
  //int _score[POPULATION];
  int index[POPULATION];
  for(int i=0; i<POPULATION; i++) {
    _block_total[i] = (gps+i)->block_total;
    //_score[i] = (gps+i)->score;
    index[i]  = i;
  }
  quick_sort(_block_total, index, 0, POPULATION-1);
  //quick_sort(_score, index, 0, POPULATION-1);

  for(int i=0; i<(int)((POPULATION/100.0f)*ELITE_LENGTH); i++) {
    for(int j=0; j<EVAL_MAX; j++) {
      //int tmp = (gps+i)->genos[j];
      (gps+i)->genos[j] = (gps+index[POPULATION-i-1])->genos[j];
      //(gps+index[POPULATION-i-1])->genos[j] = tmp;
    }
  }
  crossover();
  mutation();

  // 下位10% random
  for(int i=POPULATION-(int)((POPULATION/100.0f)*10); i<POPULATION; i++)
    for(int j=0; j<EVAL_MAX; j++)
      (gps+index[i])->genos[j] = rand() % 100;
}

void crossover()
{
  // 仮
  for(int i=(int)((POPULATION/100.0f)*ELITE_LENGTH); i<POPULATION-1; i+=2) {
    int j = rand() % EVAL_MAX;
    int tmp = (gps+i)->genos[j];
    (gps+i)->genos[j] = (gps+i+1)->genos[j];
    (gps+i+1)->genos[j] = tmp;
  }
}

void mutation()
{
  for(int i=0; i<POPULATION; i++) {
    if((rand()%100)==0) {
      (gps+i)->genos[rand()%EVAL_MAX] = rand() % 100;
    }
  }
}

int _pow(int x, int y)
{
  for(int _i=0; _i<y; _i++)
    x*=x;
  return x;
}

void quick_sort(int *ptr, int *idx, int left, int right)
{
    int i, j;
    if(left >= right)
        return;

    j = left;

    for(i=left+1; i <= right; i++)
        if(*(ptr+i) < *(ptr+left)) {
            swap(ptr+(++j), ptr+i);
            swap(idx+(++j), idx+i);
        }

    swap(ptr+left, ptr+j);
    swap(idx+left, idx+j);

    quick_sort(ptr, idx, left, j-1);
    quick_sort(ptr, idx, j+1, right);
}

void swap(int *ptr1, int *ptr2)
{
  int w = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = w;
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

  printf("\033[3;%dH", FIELD_WIDTH*3+2);
  printf("\033[0mgeneration: %04d", generation);

  printf("\033[4;%dH", FIELD_WIDTH*3+2);
  printf("\033[0mg_n: %03d", g_n);

  printf("\033[H");

  if(!draw)
    return;

  printf("\033[5;%dH", FIELD_WIDTH*3+2);
  printf("\033[0mscore: %07d", g->score);

  printf("\033[6;%dH", FIELD_WIDTH*3+2);
  printf("\033[0mblcok_total: %07d", g->block_total);

  printf("\033[7;%dH", FIELD_WIDTH*3+2);
  printf("\033[0mline_total: %07d", g->line_total);

  printf("\033[8;%dH", FIELD_WIDTH*3+2);
  printf("\033[0melite: {% 3d", g->genos[0]);
  for(int i=1; i<EVAL_MAX; i++)
    printf(",% 3d", g->genos[i]);
  printf(" }");

  // ブロックの描画を一切せず, 学習に専念する場合
  if(devote)
    return;

  printf("\033[3;%dH", FIELD_WIDTH*2+2);
  printf("next: ");
  /*{{{*/
  //for(int i=1; i<MINO_HEIGHT; i++) {
  //  printf("\033[%d;%dH", i+4, FIELD_WIDTH*2+3);
  //  for(int j=0; j<MINO_WIDTH; j++) {
  //    if(mino_aa[g->mino_next][MINO_ANGLE_0][i][j])
  //      printf("%s", block[BLOCK_MINO_I + g->mino_next]);
  //    else
  //      printf("  ");
  //  }
  //} /*}}}*/

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
      for(int i=1; i<MINO_HEIGHT-1; i++) {
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
      for(int i=1; i<MINO_HEIGHT-1; i++) {
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
      for(int i=1; i<MINO_HEIGHT-1; i++) {
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
  printf("\033[0m");
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

int cd_next(int tmp[][FIELD_WIDTH], int _x, int _y, int _angle, int _type)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if((mino_aa[_type][_angle][y][x])
        && (tmp[_y+y][_x+x] != BLOCK_NONE))
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

