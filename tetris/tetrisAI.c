// グローバル変数で使うか構造体にすべき :(

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#include <windows.h>

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

/*{{{*/
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
char block[][7+1] =
{
  "  ", // BLOCK_NONE
  " #", // BLOCK_WALL
  " I", // BLOCK_MINO_I
  " O", // BLOCK_MINO_O
  " S", // BLOCK_MINO_S
  " Z", // BLOCK_MINO_Z
  " J", // BLOCK_MINO_J
  " L", // BLOCK_MINO_L
  " T", // BLOCK_MINO_T
}; /*}}}*/

// 出来る限りmain関数のローカル変数にする

int field[FIELD_HEIGHT][FIELD_WIDTH];
int field_buffer[FIELD_HEIGHT][FIELD_WIDTH];


// Collision Detection
int cd(int _x, int _y, int _type, int _angle);

int cd_next(int tmp[][FIELD_WIDTH], int _x, int _y, int _angle, int _type);

// フィールドより上に固定されたか
int fixed_above_field(int _mino_x, int _mino_y, int _type, int _angle);

// ネクストミノがフィールドに被っているか
int suffer_next(int _mino_x, int _mino_y, int _type, int _angle);

// 高低差(各列において右隣の列との高低差の絶対値の合計)
int diff_in_height(int tmp[][FIELD_WIDTH]);

// 仮フィールド上の全てのデッドスペースの個数を返す
// ※デッドスペース -> 上下にブロックがあるスペース
int get_dead_space(int tmp[][FIELD_WIDTH], int h);

// 突出列の数を返す *全体の平均の高さとの差が4以上の列
int protrusion(int tmp[][FIELD_WIDTH]);

// 積まれているブロックの一番上の高さ
int get_max_height(int tmp[][FIELD_WIDTH]);

// 消せるラインがあるなら消し, 埋める
void line_erase();

// 仮ミノ時の消せるライン数を返す
int get_erase_line(int tmp[][FIELD_WIDTH]);

// 仮フィールドのラインを消して, その数を返す
int tmp_erase_line(int tmp[][FIELD_WIDTH]);

// フィールド上の総ブロック数を返す
int get_all_block(int tmp[][FIELD_WIDTH]);

// dest_x(dst)までmino_xを移動
void move_x(int dst, int *mino_x, int _mino_y, int _type, int _angle, int _hold);

// 次のミノを生成
void next_mino( int *mino_x, int *mino_y, int *mino_type, int *mino_angle,
    int *mino_next, int *mino_stack);

// ネクストとホールドを読み取り, 結果を返す. 失敗は1
int read_next_hold();

// フィールドを読み取り, 結果を返す. 失敗は1
int read_field();

// Hold
void hold(int *mino_x, int *mino_y, int *mino_type, int *mino_angle,
    int *mino_next, int *mino_hold, int *mino_stack);

// フィールドの0埋め等
void init( int *mino_x, int *mino_y, int *mino_type, int *mino_angle,
    int *mino_next, int *mino_hold, int *mino_stack);

// 描画類全て
void display(int _mino_x, int _mino_y, int _type, int _angle,
    int _hold, int *mino_stack);

// 終了処理
void end();


int main()
{
  srand((unsigned int)time(NULL));

  int mino_x;
  int mino_y;
  int mino_type;
  int mino_angle;
  int mino_next;
  int mino_hold;
  int spin_angle;
  int genos[EVAL_MAX];
  int mino_stack[MINO_TYPE_MAX];

  init(&mino_x, &mino_y, &mino_type, &mino_angle, &mino_next,
      &mino_hold, mino_stack);

  // 任意の遺伝子を使う
  {
    int genomes[] = // Line ------------------------------------------------
                    //{  14, -78,  90, -77, -20}; // More than One million!
                    {  -5, -91,  90, -10, -29}; // 637, 10x7, over 1M
                    //{ -30, -82, -15, -53, -18}; // 70720, 1000line
    for(int i=0; i<EVAL_MAX; i++)
      genos[i] = genomes[i];
  }

  printf("\033[2J\033[?25l");

  // フィールドの初期化
  printf("\033[3;1H");
  for(int y=4; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++)
      printf("%s", block[field[y][x]]);
    printf("\n");
  }
  printf("\033[0m\033[H");


  while(1) {
    // Read
    while(read_next_hold());
    while(read_field());


    // 落下出来るなら落下
    while(!cd(mino_x, mino_y+1, mino_type, mino_angle))
      mino_y++;
    display(mino_x, mino_y, mino_type, mino_angle, mino_hold);

    // 最後にスピンさせて表示
    if(mino_angle != spin_angle) {
      mino_angle = spin_angle;
    }

    // 現在のミノをフィールドに固定する
    memcpy(field, field_buffer,
        sizeof field);

    // 消せるなら消し, 埋め, スコア加算
    // 消えたラインの合計に加算
    line_erase();

    // フィールド外に固定されたか
    if(fixed_above_field(mino_x, mino_y, mino_type, mino_angle)) {
      init(&mino_x, &mino_y, &mino_type, &mino_angle, &mino_next,
          &mino_hold, mino_stack);
      continue;
    }

    // Nextミノがフィールドに被ったか
    next_mino(&mino_x, &mino_y, &mino_type, &mino_angle, &mino_next, mino_stack);
    display(mino_x, mino_y, mino_type, mino_angle, mino_hold);
    if(suffer_next(mino_x, mino_y, mino_type, mino_angle)) {
      init(&mino_x, &mino_y, &mino_type, &mino_angle, &mino_next,
          &mino_hold, mino_stack);
      continue;
    }

    int dest_x = mino_x,
        eval   = -65536,
        isHold = 0;

    // 約 10(x)*4(angle)*4(spin)*2(hold)*10(x2)*4(angle2) = 12800パターン
    // 実際はそんなにない

    // 現在落下中のミノ, ホールドの2パターン
    for(int _hold=0; _hold<2; _hold++) {

      int mino_buf_y,
          mino_buf_type;

      // ホールドミノの調整
      if(_hold) {
        mino_buf_y = mino_hold == MINO_TYPE_I ? 2 : 1;
        mino_buf_type = mino_hold;
      }
      // 落下中ミノの調整
      else {
        mino_buf_y = mino_y;
        mino_buf_type = mino_type;
      }

      // mino_buf毎に全角度, 全x座標で調査
      for(int _angle=MINO_ANGLE_0; _angle<MINO_ANGLE_MAX; _angle++) {
        if(cd(mino_x, mino_buf_type, mino_buf_y, _angle))
          continue;
        // Range of X coordinate
        int min_x, max_x;
        min_x = max_x = mino_x;
        while(!cd(min_x-1, mino_buf_y, mino_buf_type, _angle)) min_x--;
        while(!cd(max_x+1, mino_buf_y, mino_buf_type, _angle)) max_x++;

        for(int _x=min_x; _x<=max_x; _x++) {
          int _y = mino_buf_y;

          // ハードドロップ
          while(!cd(_x, _y+1, mino_buf_type, _angle)) _y++;

          // スピンさせる 全方位
          for(int _angle_s=MINO_ANGLE_0; _angle_s<MINO_ANGLE_MAX; _angle_s++) {
            if(cd(_x, _y, mino_buf_type, _angle_s))
              continue;

            // 仮ミノ落下後の仮フィールド生成
            int tmp[FIELD_HEIGHT][FIELD_WIDTH];
            memcpy(tmp, field, sizeof field);
            for(int y=0; y<MINO_HEIGHT; y++)
              for(int x=0; x<MINO_WIDTH; x++)
                if(mino_aa[mino_buf_type][_angle_s][y][x])
                  tmp[y+_y][x+_x] = BLOCK_MINO_I + mino_buf_type;

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
                  // 少ない方が良い
                  int diff = diff_in_height(tmp2);

                  // 重み掛けて線形和
                  field_h *= genos[EVAL_FIELD_HEIGHT];
                  line *= genos[EVAL_LINE_ERASE];
                  dead_space_cnt *= genos[EVAL_DEAD_SPACE];
                  pro_row *= genos[EVAL_PROTRUSION];
                  diff *= genos[EVAL_DIFF];
                  int _eval;
                  _eval = (field_h + line + dead_space_cnt + pro_row + diff);

                  // 最も評価の高いものを採用
                  if(eval < _eval) {
                    eval = _eval;
                    dest_x = _x;
                    mino_angle = _angle;
                    isHold = _hold;
                    spin_angle = _angle_s;
                  }
                }
              } // _angle2
            } // Next2
          } // Spin
        }
      }
    }
    if(isHold)
      hold(&mino_x, &mino_y, &mino_type, &mino_angle, &mino_next,
          &mino_hold, mino_stack);
    move_x(dest_x, &mino_x, mino_y, mino_type, mino_angle, mino_hold);
  }

  end();
}

int read_next_hold()
{
  return 0;
}

int read_field()
{
  return 0;
}

void move_x(int dst, int *mino_x, int _mino_y, int _type, int _angle, int _hold)
{
  if(dst < *mino_x) {
    while(!cd(*mino_x-1, _mino_y, _type, _angle) && dst < *mino_x)
      *mino_x -= 1;
    display(*mino_x, _mino_y, _type, _angle, _hold, );
  }
  else if(dst > *mino_x) {
    while(!cd(*mino_x+1, _mino_y, _type, _angle) && dst > *mino_x)
      *mino_x += 1;
    display(*mino_x, _mino_y, _type, _angle, _hold);
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

void next_mino( int *mino_x, int *mino_y, int *mino_type, int *mino_angle,
    int *mino_next, int *mino_stack)
{
  *mino_type = mino_stack[0];
  *mino_angle = 0;
  *mino_x     = 4;
  switch(*mino_type) {
    case MINO_TYPE_J:
    case MINO_TYPE_L:
    case MINO_TYPE_O:
    case MINO_TYPE_S:
    case MINO_TYPE_Z:
    case MINO_TYPE_T: *mino_y = 1; break;
    case MINO_TYPE_I: *mino_y = 2; break;
  }
  *mino_next = mino_stack[1];
}

void hold(int *mino_x, int *mino_y, int *mino_type, int *mino_angle,
    int *mino_next, int *mino_hold, int *mino_stack)
{
  if(*mino_hold != -1) {
    int tmp      = *mino_hold;
    *mino_hold = *mino_type;
    *mino_type = tmp;
  }
  else {
    *mino_hold = *mino_type;
    next_mino(mino_x, mino_y, mino_type, mino_angle, mino_next, mino_stack);
  }
}

void init( int *mino_x, int *mino_y, int *mino_type, int *mino_angle,
    int *mino_next, int *mino_hold, int *mino_stack)
{
  memset(field, BLOCK_NONE, sizeof field);
  for(int y=0; y<FIELD_HEIGHT; y++)
    field[y][0] = field[y][FIELD_WIDTH-1] = BLOCK_WALL;
  for(int x=1; x<FIELD_WIDTH-1; x++)
    field[FIELD_HEIGHT-1][x] = BLOCK_WALL;
  *mino_hold = -1;
  hold(mino_x, mino_y, mino_type, mino_angle, mino_next, mino_hold, mino_stack);
  next_mino(mino_x, mino_y, mino_type, mino_angle, mino_next, mino_stack);
}

void display(int _mino_x, int _mino_y, int _type, int _angle,
    int _hold, int *mino_stack)
{
  memcpy(field_buffer, field, sizeof field);
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if(mino_aa[_type][_angle][y][x])
        field_buffer[y+_mino_y][x+_mino_x]
          = BLOCK_MINO_I + _type;

  printf("\033[H");

  printf("\033[1;%dH", FIELD_WIDTH*2+2);
  printf(" hold:");
  if(_hold != -1) {
    for(int i=1; i<MINO_HEIGHT; i++) {
      printf("\033[%d;%dH", i+1, FIELD_WIDTH*2+3);
      for(int j=0; j<MINO_WIDTH; j++) {
        if(mino_aa[_hold][MINO_ANGLE_0][i][j])
          printf("%s", block[BLOCK_MINO_I + _hold]);
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
  //    if(mino_aa[mino_next][MINO_ANGLE_0][i][j])
  //      printf("%s", block[BLOCK_MINO_I + mino_next]);
  //    else
  //      printf("  ");
  //  }
  //} /*}}}*/

  for(int k=0; k<MINO_TYPE_MAX-1; k++) {
    for(int i=1; i<MINO_HEIGHT-1; i++) {
      printf("\033[%d;%dH", i+3+(k+1)*3, FIELD_WIDTH*2+3);
      for(int j=0; j<MINO_WIDTH; j++) {
        if(mino_aa[mino_stack[k]][MINO_ANGLE_0][i][j])
          printf("%s", block[BLOCK_MINO_I + mino_stack[k]]);
        else
          printf("  ");
      }
    }
  }

  printf("\033[H");

  // 見えない壁
  for(int y=2; y<4; y++) {
    printf("  ");
    for(int x=1; x<FIELD_WIDTH-1; x++)
      printf("%s", block[field_buffer[y][x]]);
    printf("\n");
  }
  // Main field
  for(int y=4; y<FIELD_HEIGHT-1; y++) {
    for(int x=0; x<FIELD_WIDTH-1; x++) {
      printf("%s", block[field_buffer[y][x]]);
    }
    printf("\n");
  }
  printf("\033[0m");
}

int cd(int _x, int _y, int _type, int _angle)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if((mino_aa[_type][_angle][y][x])
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

int fixed_above_field(int _mino_x, int _mino_y, int _type, int _angle)
{
  int cnt = 0;
  for(int y=0; y<MINO_HEIGHT; y++) {
    for(int x=0; x<MINO_WIDTH; x++) {
      if(field[y+_mino_y][x+_mino_x]
          && mino_aa[_type][_angle][y][x]
          && (_mino_y+y<MINO_HEIGHT)) {
        if(++cnt==4) {
          return 1;
        }
      }
    }
  }
  return 0;
}

int suffer_next(int _mino_x, int _mino_y, int _type, int _angle)
{
  for(int y=0; y<MINO_HEIGHT; y++) {
    for(int x=0; x<MINO_WIDTH; x++) {
      if((mino_aa[_type][_angle][y][x])
        && (field[y+_mino_y][x+_mino_x])) {
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

