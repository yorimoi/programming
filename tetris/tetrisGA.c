#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 25

#define MINO_WIDTH  4
#define MINO_HEIGHT 4

#define ELITE_LENGTH 30          // 選択するエリート(%)
#define POPULATION 1           // 遺伝子集団の数
#define INDIVIDUAL_MUTATION 1    // 個体突然変異確立(%)
#define GENERATION_MAX 1       // Max世代数
#define COEFFICIENT 199-99       // 係数
#define INFINITE 0               // 世代数無限
#define FILENAME "log.txt"       // ログ出力ファイル名
#define LIMIT 1000000               // 消去段数制限


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

// Default {{{
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

// GitHub
//char block[][19+3+1] =
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
//};

// 8色
//char block[][7+1] =
//{
//  "\033[0m  ", // BLOCK_NONE
//  "\033[37m #", // BLOCK_WALL
//  "\033[31m #", // BLOCK_MINO_I
//  "\033[32m #", // BLOCK_MINO_O
//  "\033[33m #", // BLOCK_MINO_S
//  "\033[34m #", // BLOCK_MINO_Z
//  "\033[35m #", // BLOCK_MINO_J
//  "\033[36m #", // BLOCK_MINO_L
//  "\033[37m #", // BLOCK_MINO_T
//};

// No color
//char block[][7+1] =
//{
//  "  ", // BLOCK_NONE
//  " #", // BLOCK_WALL
//  " I", // BLOCK_MINO_I
//  " O", // BLOCK_MINO_O
//  " S", // BLOCK_MINO_S
//  " Z", // BLOCK_MINO_Z
//  " J", // BLOCK_MINO_J
//  " L", // BLOCK_MINO_L
//  " T", // BLOCK_MINO_T
//};

// 草
//char block[][7+1] =
//{
//  "\033[0m  ", // BLOCK_NONE
//  "\033[32mWW", // BLOCK_WALL
//  "\033[32mww", // BLOCK_MINO_I
//  "\033[32mww", // BLOCK_MINO_O
//  "\033[32mww", // BLOCK_MINO_S
//  "\033[32mww", // BLOCK_MINO_Z
//  "\033[32mww", // BLOCK_MINO_J
//  "\033[32mww", // BLOCK_MINO_L
//  "\033[32mww", // BLOCK_MINO_T
//}; /*}}}*/

typedef struct { /*{{{*/
  int field[FIELD_HEIGHT][FIELD_WIDTH];
  int field_buffer[FIELD_HEIGHT][FIELD_WIDTH];

  int mino_x, mino_y;
  int mino_type;
  int mino_angle;
  int mino_next;
  int mino_hold;
  int can_hold;

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

int generation = 1;   // 現在の世代 1~
int individual = 0;   // 現在の個体 0~

// typedef int bool;
int draw       = 1;   // 描画するか否か
int devote     = 0;   // 学習に専念するか

int max_block_total = 0;
int max_line_total  = 0;
int max_score       = 0;
int elite_of_line[EVAL_MAX];
int elite_of_score[EVAL_MAX];


// Collision Detection
int cd(GENOME *g, int _x, int _y, int _type, int _angle);

int cd_next(int tmp[][FIELD_WIDTH], int _x, int _y, int _angle, int _type);

// フィールドより上に固定されたか
int fixed_above_field(GENOME *g);

// ネクストミノがフィールドに被っているか
int suffer_next(GENOME *g);

// 高低差(各列において右隣の列との高低差の絶対値の合計)
int diff_in_height(int tmp[][FIELD_WIDTH]);

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

// 仮フィールドのラインを消して, その数を返す
int tmp_erase_line(int tmp[][FIELD_WIDTH]);

// フィールド上の総ブロック数を返す
int get_all_block(int tmp[][FIELD_WIDTH]);

// dest_x(dst)までmino_xを移動
void move_x(GENOME *g, int dst);

// ミノ配列の初期化
void init_mino(int *mino_stack);

// 次のミノを生成
void next_mino(GENOME *g);

// Hold
void hold(GENOME *g);

// フィールドの0埋め等
void init();

// 次の個体へシフト
void reset();

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
  //if(setvbuf(stdout, NULL, _IOFBF, 65536) != 0) {
  //  perror("setvbuf");
  //  exit(1);
  //}

  // 1世代目の遺伝子の初期化
  for(int i=0; i<POPULATION; i++)
    for(int j=0; j<EVAL_MAX; j++)
      (gps+i)->genos[j] = rand() % COEFFICIENT;
  init();

  // 任意の遺伝子を使う
  {
    int genomes[] = // Line ------------------------------------------------
                    //{ -32, -69,  97, -84, -19}; // 65341, 199-99 Rework
                    //{  14, -78,  90, -77, -20}; // More than One million!
                    //{ -44, -77,  71, -79, -11}; // 389, 10x7
                    //{ -19, -65,  60, -58, -24}; // 1063, 10x8
                    //{ -42, -99,  98,   8, -22}; // 1485, 10x8
                    //{  -3, -58,  43, -60, -11}; // 17091, 10x10
                    {  -5, -91,  90, -10, -29}; // 637, 10x7, over 1M
                    //{ -30, -82, -15, -53, -18}; // 70720, 1000line
                    // Socre -----------------------------------------------
                    //{ -30, 10, -20, -10, -5}; // My thinking
                    //{ -52, -79, -11, -14, -12}; // 76,900
    for(int i=0; i<EVAL_MAX; i++)
      (gps+0)->genos[i] = genomes[i];
  }

  printf("\033[2J\033[?25l");

  // フィールドの初期化
  printf("\033[3;1H");
  for(int y=4; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++)
      printf("%s", block[gps->field[y][x]]);
    printf("\n");
  }
  printf("\033[0m\033[H");


  while(generation <= GENERATION_MAX || INFINITE) {
    // 落下出来るなら落下
    if(!cd(gps+individual, (gps+individual)->mino_x, (gps+individual)->mino_y+1,
          (gps+individual)->mino_type, (gps+individual)->mino_angle))
      (gps+individual)->mino_y++;
    else {
      // 現在のミノをフィールドに固定する
      memcpy((gps+individual)->field, (gps+individual)->field_buffer,
          sizeof (gps+individual)->field);

      // 消せるなら消し, 埋め, スコア加算
      // 消えたラインの合計に加算
      (gps+individual)->line_total += line_erase(gps+individual);

      // フィールド外に固定されたか
      if(fixed_above_field(gps+individual))
        continue;

      // Nextミノがフィールドに被ったか
      next_mino(gps+individual);
      display(gps+individual);
      if(suffer_next(gps+individual))
        continue;

      // 消去段数制限
      if((gps+individual)->line_total > LIMIT) {
        draw = 0;
        reset();
        continue;
      }

      // 積んだブロックの合計
      (gps+individual)->block_total++;

      int dest_x = (gps+individual)->mino_x,
          eval   = -65536,
          isHold = 0;

      // 現在落下中のミノ, ホールドの2パターン
      for(int _hold=0; _hold<2; _hold++) {

        int mino_buf_y,
            mino_buf_type;

        // ホールドミノの調整
        if(_hold) {
          mino_buf_y = (gps+individual)->mino_hold == MINO_TYPE_I ? 2 : 1;
          mino_buf_type = (gps+individual)->mino_hold;
        }
        // 落下中ミノの調整
        else {
          mino_buf_y = (gps+individual)->mino_y;
          mino_buf_type = (gps+individual)->mino_type;
        }

        // mino_buf毎に全角度, 全x座標で調査
        for(int _angle=MINO_ANGLE_0; _angle<MINO_ANGLE_MAX; _angle++) {
          if(cd(gps+individual, (gps+individual)->mino_x,
               mino_buf_type, mino_buf_y, _angle))
            continue;
          // Range of X coordinate
          int min_x, max_x;
          min_x = max_x = (gps+individual)->mino_x;
          while(!cd(gps+individual, min_x-1, mino_buf_y, mino_buf_type, _angle))
            min_x--;
          while(!cd(gps+individual, max_x+1, mino_buf_y, mino_buf_type, _angle))
            max_x++;

          for(int _x=min_x; _x<=max_x; _x++) {
            int _y = mino_buf_y;

            // 仮ミノ落下後の仮フィールド生成
            while(!cd(gps+individual, _x, _y+1, mino_buf_type, _angle))
              _y++;
            int tmp[FIELD_HEIGHT][FIELD_WIDTH];
            memcpy(tmp, (gps+individual)->field, sizeof (gps+individual)->field);
            for(int y=0; y<MINO_HEIGHT; y++)
              for(int x=0; x<MINO_WIDTH; x++)
                if(mino_aa[mino_buf_type][_angle][y][x])
                  tmp[y+_y][x+_x] = BLOCK_MINO_I + mino_buf_type;

            // Nextミノも見る
            {
              int mino_next2 = (gps+individual)->mino_next;

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
                  // 少ない方が良い
                  int diff = diff_in_height(tmp2);

                  // evaluation()
                  field_h *= (gps+individual)->genos[EVAL_FIELD_HEIGHT];
                  line *= (gps+individual)->genos[EVAL_LINE_ERASE];
                  dead_space_cnt *= (gps+individual)->genos[EVAL_DEAD_SPACE];
                  pro_row *= (gps+individual)->genos[EVAL_PROTRUSION];
                  diff *= (gps+individual)->genos[EVAL_DIFF];

                  int _eval = (field_h
                             + line
                             + dead_space_cnt
                             + pro_row
                             + diff
                             );
                  if(eval < _eval) {
                    eval = _eval;
                    dest_x = _x;
                    (gps+individual)->mino_angle = _angle;
                    isHold = _hold;
                  }
                }
              }//_angle2
            }//Next2
          }
        }
      }
      if(isHold)
        hold(gps+individual);
      move_x(gps+individual, dest_x);
    }
    display(gps+individual);
  }

  end();
}

void move_x(GENOME *g, int dst)
{
  if(dst < g->mino_x) {
    while(!cd(g, g->mino_x-1, g->mino_y, g->mino_type, g->mino_angle)
        && dst < g->mino_x) {
      g->mino_x--;
      display(g);
    }
  }
  else if(dst > g->mino_x) {
    while(!cd(g, g->mino_x+1, g->mino_y, g->mino_type, g->mino_angle)
        && dst > g->mino_x) {
      g->mino_x++;
      display(g);
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
  g->can_hold = 1;
}

void hold(GENOME *g)
{
  g->can_hold = 0;
  if(g->mino_hold != -1) {
    int tmp      = g->mino_hold;
    g->mino_hold = g->mino_type;
    g->mino_type = tmp;
  }
  else {
    g->mino_hold = g->mino_type;
    next_mino(g);
  }
}

void init()
{
  individual  = 0;
  for(int i=0; i<POPULATION; i++) {
    memset((gps+i)->field, BLOCK_NONE, sizeof (gps+i)->field);
    for(int y=0; y<FIELD_HEIGHT; y++)
      (gps+i)->field[y][0] = (gps+i)->field[y][FIELD_WIDTH-1] = BLOCK_WALL;
    for(int x=1; x<FIELD_WIDTH-1; x++)
      (gps+i)->field[FIELD_HEIGHT-1][x] = BLOCK_WALL;
    for(int j=0; j<MINO_TYPE_MAX; j++)
      (gps+i)->mino_stack1[j] = (gps+i)->mino_stack2[j] = j;
    init_mino((gps+i)->mino_stack1);
    init_mino((gps+i)->mino_stack2);
    (gps+i)->mino_stack_cnt = -1;
    (gps+i)->mino_hold = -1;
    (gps+i)->can_hold = 1;
    next_mino(gps+i);
    (gps+i)->score = 0;
    (gps+i)->block_total = 0;
    (gps+i)->line_total = 0;
    // いきなりホールド
      hold(gps+i);
  }
}

void reset()
{
  if(++individual >= POPULATION) {
    select_elite();

    //end();
    generation++;
    //if(generation%10==0)
      draw = 1;
    init();
  }
}

void select_elite()
{
  int _line_total[POPULATION];
  //int _score[POPULATION];
  int index[POPULATION];
  for(int i=0; i<POPULATION; i++) {
    _line_total[i] = (gps+i)->line_total;
    //_score[i] = (gps+i)->score;
    index[i]  = i;
  }
  //quick_sort(_block_total, index, 0, POPULATION-1);
  //quick_sort(_score, index, 0, POPULATION-1);
  for(int i=0; i<POPULATION; i++) {
    for(int j=POPULATION-1; j>i; j--) {
      if(_line_total[j] < _line_total[j-1]) {
      //if(_score[j] < _score[j-1]) {
        swap(&_line_total[j], &_line_total[j-1]);
        //swap(&_score[j], &_score[j-1]);
        swap(&index[j], &index[j-1]);
      }
    }
  }
  //for(int i=0; i<POPULATION; i++)
  //  printf("%d,", (gps+index[POPULATION-i-1])->block_total);

  for(int i=0; i<(int)((POPULATION/100.0f)*ELITE_LENGTH); i++) {
    for(int j=0; j<EVAL_MAX; j++) {
      //int tmp = (gps+i)->genos[j];
      (gps+i)->genos[j] = (gps+index[POPULATION-i-1])->genos[j];
      //(gps+index[POPULATION-i-1])->genos[j] = tmp;
    }
  }
  crossover();
  //mutation();

  // 下位50% random
  for(int i=POPULATION-(int)((POPULATION/100.0f)*50); i<POPULATION; i++)
    for(int j=0; j<EVAL_MAX; j++)
      (gps+index[i])->genos[j] = rand() % COEFFICIENT;
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
  // 0.1%
  for(int i=0; i<POPULATION; i++) {
    if((rand()%1000)==0) {
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

  printf("\033[3;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
  printf("\033[0mgeneration: %d", generation);

  printf("\033[4;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
  printf("\033[0mindividual: %3d / %-d", individual, POPULATION - 1);

  if(max_line_total < g->line_total) {
    max_line_total = g->line_total;
    printf("\033[8;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
    printf("\033[0mmax_line_total: %07d", max_line_total);
    printf("\033[9;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
    printf("\033[0m -> elite: {%4d", g->genos[0]);
    for(int i=1; i<EVAL_MAX; i++)
      printf(",%4d", g->genos[i]);
    printf("}");
    for(int i=0; i<EVAL_MAX; i++)
      elite_of_line[i] = g->genos[i];
  }

  if((max_score < g->score) && (g->line_total >= LIMIT)) {
    max_score = g->score;
    printf("\033[10;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
    printf("\033[0mmax_score: %07d", max_score);
    printf("\033[11;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
    printf("\033[0m -> elite: {%4d", g->genos[0]);
    for(int i=1; i<EVAL_MAX; i++)
      printf(",%4d", g->genos[i]);
    printf("}");
    for(int i=0; i<EVAL_MAX; i++)
      elite_of_score[i] = g->genos[i];
  }

  if(POPULATION <= 20) {
    printf("\033[%d;%dH", 12+individual, FIELD_WIDTH*2+MINO_WIDTH*3+2);
    printf("\033[0mgenos: {%4d", g->genos[0]);
    for(int i=1; i<EVAL_MAX; i++)
      printf(",%4d", g->genos[i]);
    printf("} %d  ", g->block_total);
  } else {
    printf("\033[12;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
    printf("\033[0mgenos: {%4d", g->genos[0]);
    for(int i=1; i<EVAL_MAX; i++)
      printf(",%4d", g->genos[i]);
    printf("} %d   ", g->block_total);
  }

  printf("\033[H");

  if(!draw)
    return;

  printf("\033[5;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
  printf("\033[0mscore: %07d", g->score);

  printf("\033[6;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
  printf("\033[0mblcok_total: %07d", g->block_total);

  printf("\033[7;%dH", FIELD_WIDTH*2+MINO_WIDTH*3+2);
  printf("\033[0mline_total: %07d", g->line_total);

  // ブロックの描画を一切せず, 学習に専念する場合
  if(devote)
    return;

  printf("\033[1;%dH", FIELD_WIDTH*2+2);
  printf(" hold:");
  if(g->mino_hold != -1) {
    for(int i=1; i<MINO_HEIGHT; i++) {
      printf("\033[%d;%dH", i+1, FIELD_WIDTH*2+3);
      for(int j=0; j<MINO_WIDTH; j++) {
        if(mino_aa[g->mino_hold][MINO_ANGLE_0][i][j])
          printf("%s", block[BLOCK_MINO_I + g->mino_hold]);
        else
          printf("  ");
      }
    }
  }

  printf("\033[5;%dH", FIELD_WIDTH*2+2);
  printf("\033[0m next:");
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
        printf("\033[%d;%dH", i+3+next_cnt*3, FIELD_WIDTH*2+3);
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
        printf("\033[%d;%dH", i+3+next_cnt*3, FIELD_WIDTH*2+3);
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
        printf("\033[%d;%dH", i+3+(k+1)*3, FIELD_WIDTH*2+3);
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
  // Main field
  for(int y=4; y<FIELD_HEIGHT-1; y++) {
    for(int x=0; x<FIELD_WIDTH-1; x++) {
      printf("%s", block[g->field_buffer[y][x]]);
    }
    printf("\n");
  }
  printf("\033[0m");
}

int cd(GENOME *g, int _x, int _y, int _type, int _angle)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if((mino_aa[_type][_angle][y][x])
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
          reset();
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
        reset();
        return 1;
      }
    }
  }
  return 0;
}

void end()
{
  printf("\033[0m\033[2J");
  printf("\033[2;1H\033[0mFIELD_WIDTH: %d", FIELD_WIDTH-2);
  printf("\033[3;1H\033[0mFIELD_HEIGHT: %d", FIELD_HEIGHT-5);
  printf("\033[4;1H\033[0mgeneration: %d", generation-1);
  printf("\033[5;1H\033[0mmax_line_total: %07d / %-d", max_line_total, LIMIT);
  printf("\033[6;1H\033[0melite_of_line: {%4d", elite_of_line[0]);
  for(int i=1; i<EVAL_MAX; i++)
    printf(",%4d", elite_of_line[i]);
  printf("}\n");
  printf("\033[7;1H\033[0mmax_score: %07d", max_score);
  printf("\033[8;1H\033[0melite_of_score: {%4d", elite_of_score[0]);
  for(int i=1; i<EVAL_MAX; i++)
    printf(",%4d", elite_of_score[i]);
  printf("}\n");
  printf("\033[0m\033[?25h");

  FILE *fp;
  if((fp = fopen(FILENAME, "a")) == NULL) {
    fprintf(stderr, "%s: Open failured.\n", FILENAME);
    exit(1);
  }

  time_t t = time(NULL);
  struct tm *local = localtime(&t);

  fprintf(fp, "%04d/", local->tm_year + 1900);
  fprintf(fp, "%02d/", local->tm_mon + 1);
  fprintf(fp, "%02d ", local->tm_mday);
  fprintf(fp, "%02d:", local->tm_hour);
  fprintf(fp, "%02d:", local->tm_min);
  fprintf(fp, "%02d\n", local->tm_sec);

  fprintf(fp, "FIELD_WIDTH: %d\n", FIELD_WIDTH-2);
  fprintf(fp, "FIELD_HEIGHT: %d\n", FIELD_HEIGHT-5);
  fprintf(fp, "generation: %d\n", generation-1);
  fprintf(fp, "max_line_total: %07d / %-d\n", max_line_total, LIMIT);
  fprintf(fp, "elite_of_line: {%4d", elite_of_line[0]);
  for(int i=1; i<EVAL_MAX; i++)
    fprintf(fp, ",%4d", elite_of_line[i]);
  fprintf(fp, "}\n");
  fprintf(fp, "max_score: %07d\n", max_score);
  fprintf(fp, "elite_of_score: {%4d", elite_of_score[0]);
  for(int i=1; i<EVAL_MAX; i++)
    fprintf(fp, ",%4d", elite_of_score[i]);
  fprintf(fp, "}\n\n");

  fclose(fp);
  exit(0);
}

