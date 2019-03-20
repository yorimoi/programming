/* ドラクエを小一時間で作ってみた（フィールド、イベント編） */
/* https://youtu.be/TfSLsT4fkCE */
/* https://bussorenre.hatenablog.jp/entry/20110502/1304415417 */
/* ストーリー分からな過ぎた -> リベンジ ポケモン */
/* vim:set foldmethod=marker: */

#include <stdio.h>
#include <termios.h>

#define FIELD_WIDTH   136
#define FIELD_HEIGHT  136

#define SCREEN_WIDTH  16
#define SCREEN_HEIGHT 16

typedef int bool; /*{{{*/
#define true  1
#define false 0
// }}}
typedef unsigned char  BYTE; /*{{{*/
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long           LONG;
/*}}}*/
typedef struct BMPFILEHEADER{ /* {{{ */
  char  bfType[2];
  DWORD	bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD	bfOffBits;
}BMPFILEHEADER;
/*}}}*/
typedef struct BMPINFOHEADER{ /*{{{*/
  DWORD	biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD	biCompression;
  DWORD	biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD	biClrUsed;
  DWORD	biClrImportant;
}BMPINFOHEADER;
/*}}}*/

enum {
  CELL_TYPE_SEA,
  CELL_TYPE_SEA_2,
  CELL_TYPE_PLANE,
  CELL_TYPE_FOREST,
  CELL_TYPE_MOUNTAIN,
  CELL_TYPE_ROCK,
  CELL_TYPE_WALL,
  CELL_TYPE_DESERT,
  CELL_TYPE_POISON,
  CELL_TYPE_BRIDGE,
  CELL_TYPE_CASTLE,
  CELL_TYPE_TOWN,
  CELL_TYPE_DUNGEON,
  CELL_TYPE_SHRINE,
  CELL_TYPE_MAX,
};

enum {
  FLAG_MAGIC_KEY,
  FLAG_RORA_RESCUED,
  FLAG_RORA_LOVE,
  FLAG_SUN_STONE,
  FLAG_SILVER_HERP,
  FLAG_RAIN_CLOUD_STUFF,
  FLAG_FAIRY_FLUTE,
  FLAG_ROTO_ARMOR,
  FLAG_GOLEM_KILLED,
  FLAG_ROTO_EMBLEM,
  FLAG_RAINBOW_DROP,
  FLAG_RAINBOW_BRIDGE,
  FLAG_LIGHT_BALL,
  FLAG_MAX
};

enum {
  LOCATION_RADATOM,
  LOCATION_DUNGEON_NORTH,
  LOCATION_DUNGEON_SOUTH,
  LOCATION_RIMURUDARU,
  LOCATION_GARAI,
  LOCATION_RAIN_CLOUD,
  LOCATION_MAIRA,
  LOCATION_DOMUDORA,
  LOCATION_GOLEM,
  LOCATION_MERUKIDO,
  LOCATION_ROTO_EMBLEM,
  LOCATION_HOLLY,
  LOCATION_RAINBOW_BLIDGE,
  LOCATION_DRAGON_KING,
  LOCATION_MAX
};

int locations[LOCATION_MAX][2] =
{
  { 51, 51}, // LOCATION_RADATOM
  {112, 52}, // LOCATION_DUNGEON_NORTH
  {112, 57}, // LOCATION_DUNGEON_SOUTH
  {110, 80}, // LOCATION_RIMURUDARU
  { 10, 10}, // LOCATION_GARAI
  { 89,  9}, // LOCATION_RAIN_CLOUD
  {112, 18}, // LOCATION_MAIRA
  { 33, 97}, // LOCATION_DOMUDORA
  { 81,108}, // LOCATION_GOLEM
  { 81,110}, // LOCATION_MERUKIDO
  { 91,121}, // LOCATION_ROTO_EMBLEM
  {116,117}, // LOCATION_HOLLY
  { 73, 57}, // LOCATION_RAINBOW_BLIDGE
  { 56, 56}, // LOCATION_DRAGON_KING
};

typedef struct BGR{
  unsigned char b, g, r;
}BGR;
BGR pixels[FIELD_HEIGHT][FIELD_WIDTH];

typedef struct CELLDESC{
  BGR bgr;
  char aa[21+1]; // "\033[48;2;000;000;000mXX"
  bool canPen;   // Penetration
}CELLDESC;

CELLDESC cell_descs[] =
{
  //  G   R   B                R   G   B
  {{ 99, 66,231}, "\033[48;2;066;099;231m~~", false}, // CELL_TYPE_SEA
  {{110, 53,188}, "\033[48;2;053;110;188m~~", false}, // CELL_TYPE_SEA_2
  {{198,132,  0}, "\033[48;2;132;198;000mww", true},  // CELL_TYPE_PLANE
  {{165,  0,  0}, "\033[48;2;000;165;000m44", true},  // CELL_TYPE_FOREST
  {{214,193, 66}, "\033[48;2;193;214;066m^^", true},  // CELL_TYPE_MOUNTAIN
  {{132,132,132}, "\033[48;2;132;132;132mMM", false}, // CELL_TYPE_ROCK
  {{198,198,198}, "\033[48;2;198;198;198m##", false}, // CELL_TYPE_WALL
  {{231,255,132}, "\033[48;2;255;231;132m..", true},  // CELL_TYPE_DESERT
  {{ 82,  0,  0}, "\033[48;2;000;082;000m**", true},  // CELL_TYPE_POISON
  {{226,226,226}, "\033[48;2;226;226;226m[]", true},  // CELL_TYPE_BRIDGE
  {{ 26,237, 61}, "\033[48;2;237;026;061m城", true},  // CELL_TYPE_CASTLE
  {{129,239, 15}, "\033[48;2;239;129;015m町", true},  // CELL_TYPE_TOWN
  {{ 76,153,  0}, "\033[48;2;153;076;000m %", true},  // CELL_TYPE_DUNGEON
  {{ 87,167,168}, "\033[48;2;167;087;168m +", true},  // CELL_TYPE_SHRINE
};

int cells[FIELD_HEIGHT][FIELD_WIDTH];

//int player_x = 51;
//int player_y = 52;
int player_x = 55;
int player_y = 57;

bool quit = false;

bool flags[FLAG_MAX];
//=
//{
//  true,  // FLAG_MAGIC_KEY
//  true,  // FLAG_RORA_RESCUED
//  true,  // FLAG_RORA_LOVE
//  true,  // FLAG_SUN_STONE
//  true,  // FLAG_SILVER_HERP
//  true,  // FLAG_RAIN_CLOUD_STUFF
//  true,  // FLAG_FAIRY_FLUTE
//  true,  // FLAG_ROTO_ARMOR
//  true,  // FLAG_GOLEM_KILLED
//  true,  // FLAG_ROTO_EMBLEM
//  true,  // FLAG_RAINBOW_DROP
//  true,  // FLAG_RAINBOW_BRIDGE
//  false,  // FLAG_LIGHT_BALL
//};

int getch()
{
  char c;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  c = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return c;
}

void display()
{
  printf("                                \n");
  printf("                                ");
  printf("\033[H");
  //printf("x:%d y:%d\n", player_x, player_y);
  /*
  if(flags[FLAG_RORA_LOVE]) {
    int x = locations[LOCATION_RADATOM][0] - player_x;
    int y = locations[LOCATION_RADATOM][1] - player_y;
    printf("ローラ「ラダトーム城まで%sへ%d, %sへ%dです.」\n"
        , x>0?"東":"西", x>=0?x:x*-1
        , y>0?"南":"北", y>=0?y:y*-1
      );
  } */

  for(int y=-SCREEN_HEIGHT/2; y<SCREEN_HEIGHT/2; y++) {
    for(int x=-SCREEN_WIDTH/2; x<SCREEN_WIDTH/2; x++) {
      int x2 = player_x + x;
      int y2 = player_y + y;
      if(x2==player_x && y2==player_y)
        printf("\033[30;47m＠\033[0m");
      else
        printf("%s", cell_descs[cells[y2][x2]].aa);
    }
    printf("\n");
  }
  printf("\033[0m");
}

void player_dead()
{
  player_x = locations[LOCATION_RADATOM][0];
  player_y = locations[LOCATION_RADATOM][1];

  display();
  printf("王様「勇者よ,              \n"
         "死んでしまうとは情けない.」\a");
  getch();
  printf("\033[2J");
  display();
  printf("王様「しっかり頼むぞ!」");
  getch();
}

void event()
{
  int location = -1;
  for(int i=0; i<LOCATION_MAX; i++)
    if(player_x==locations[i][0] && player_y==locations[i][1]) {
      location = i;
      break;
    }

  if(location < 0)
    return;

  display();

  switch(location) {
    case LOCATION_RADATOM:
      if(flags[FLAG_LIGHT_BALL]) {
        printf("王様「勇者よ!\n"
               "よくぞ竜王を倒してくれた!」\a");
        getch();
        printf("\033[2J");
        display();
        printf("王様「わしに代わって\n"
               "この国を治めてくれい!」\a");
        getch();
        printf("\033[2J");
        display();
        printf("勇者「自分の治める国があるなら,\n"
               "それは自分で探したいのです.」\a");
        getch();
        printf("\033[2J");
        display();
        printf("ローラ「私も連れて行ってください!」\a");
        getch();
        printf("\033[2J");
        display();
        printf("勇者「わかりました.\n"
               "一緒に参りましょう.」\a");
        getch();
        printf("\033[2J");
        display();
        printf("          ～ THE END ～          \n\a");
        getch();
        quit = true;
      }
      if(!flags[FLAG_RORA_LOVE] && flags[FLAG_RORA_RESCUED]){
        flags[FLAG_RORA_LOVE] = true;
        printf("ローラの愛を手に入れた!\a");
        getch();
        display();
      }
      if(!flags[FLAG_SUN_STONE]) {
        if(flags[FLAG_MAGIC_KEY]) {
          flags[FLAG_SUN_STONE] = true;
          printf("城の裏の地下で\n"
                 "太陽の石を手に入れた!\a");
        } else {
          printf("城の裏に何かありそうだが\n"
                 "鍵が掛かっていては入れない.\a");
        }
        getch();
      }
      break;
    case LOCATION_DUNGEON_NORTH:
    case LOCATION_DUNGEON_SOUTH:
      if(!flags[FLAG_RORA_RESCUED]) {
        if(flags[FLAG_MAGIC_KEY]) {
          flags[FLAG_RORA_RESCUED] = true;
          printf("ドラゴンを倒して\n"
                 "ローラを救出した!\a");
        } else {
         printf("ダンジョンに扉があるが,\n"
                "鍵が掛かっていては入れない.\a");
        }
        getch();
      }

      if(( player_x == locations[LOCATION_DUNGEON_NORTH][0])
        && (player_y == locations[LOCATION_DUNGEON_NORTH][1])) {
        player_x = locations[LOCATION_DUNGEON_SOUTH][0];
        player_y = locations[LOCATION_DUNGEON_SOUTH][1];
      } else {
        player_x = locations[LOCATION_DUNGEON_NORTH][0];
        player_y = locations[LOCATION_DUNGEON_NORTH][1];
      }
      break;
    case LOCATION_RIMURUDARU:
      if(!flags[FLAG_MAGIC_KEY]) {
        flags[FLAG_MAGIC_KEY] = true;
        printf("魔法の鍵を買った.\a");
        getch();
      }
      break;
    case LOCATION_GARAI:
      if(!flags[FLAG_SILVER_HERP]) {
        if(flags[FLAG_MAGIC_KEY]) {
          flags[FLAG_SILVER_HERP] = true;
          printf("ガライの墓で銀の竪琴を\n"
                 "手に入れた!\a");
        } else {
          printf("怪しい建物があるが, 鍵が\n"
                 "掛かっていては開けられない.\a");
        }
        getch();
      }
      break;
    case LOCATION_RAIN_CLOUD:
      if(!flags[FLAG_RAIN_CLOUD_STUFF]) {
        if(flags[FLAG_SILVER_HERP]) {
          flags[FLAG_RAIN_CLOUD_STUFF] = true;
          printf("老人「銀の竪琴の音色は\n"
                 "素晴らしいのう.」\a");
          getch();
          display();
          printf("老人「そなたに雨雲の杖を\n"
                 "さずけよう.」\a");
        } else {
          printf("老人「銀の竪琴の音色を\n"
                 "聞きたいのぅ.(チラッ)」\a");
        }
        getch();
      }
      break;
    case LOCATION_MAIRA:
      if(!flags[FLAG_FAIRY_FLUTE]) {
        flags[FLAG_FAIRY_FLUTE] = true;
        printf("温泉の近くで妖精の笛を拾った.\a");
        getch();
      }
      break;
    case LOCATION_DOMUDORA:
      if(!flags[FLAG_ROTO_ARMOR]) {
        flags[FLAG_ROTO_ARMOR] = true;
        printf("悪魔の騎士を倒して,\n"
               "ロトの鎧を手に入れた!\a");
        getch();
      }
      break;
    case LOCATION_GOLEM:
      if(!flags[FLAG_GOLEM_KILLED]) {
        if(flags[FLAG_FAIRY_FLUTE]) {
          flags[FLAG_GOLEM_KILLED] = true;
          printf("ゴーレムを妖精の笛で\n"
                 "眠らせて倒した!\a");
        } else {
          printf("ゴーレムにやられてしまった...\a");
          player_dead();
        }
        getch();
      }
      break;
    case LOCATION_MERUKIDO:
      {
        int x = locations[LOCATION_ROTO_EMBLEM][0]
              - locations[LOCATION_RADATOM][0];
        int y = locations[LOCATION_ROTO_EMBLEM][1]
              - locations[LOCATION_RADATOM][1];
        printf("老人「ラダトーム城まで西に%d,\n"
               "北に%dの場所を調べなされ.」\a", x, y);
        getch();
      }
      break;
    case LOCATION_ROTO_EMBLEM:
      if(!flags[FLAG_ROTO_EMBLEM]) {
        flags[FLAG_ROTO_EMBLEM] = true;
        printf("ロトの紋章を手に入れた!\a");
        getch();
      }
      break;
    case LOCATION_HOLLY:
      if(!flags[FLAG_RAINBOW_DROP]) {
        if(!flags[FLAG_ROTO_EMBLEM]) {
          printf("本物の勇者なら印があるはずじゃ!\a");
        } else {
          if(flags[FLAG_SUN_STONE] && flags[FLAG_RAIN_CLOUD_STUFF]) {
            flags[FLAG_RAINBOW_DROP] = true;
            printf("この虹の雫があれば竜王への道が\n"
                   "開けるであろう.\a");
          } else {
            printf("老人「太陽と雨雲が合わさるとき,\n"
                   "虹の橋が架かるとの言い伝えじゃ.」\a");
          }
        }
        getch();
      }
      break;
    case LOCATION_RAINBOW_BLIDGE:
      if(!flags[FLAG_RAINBOW_BRIDGE] && flags[FLAG_RAINBOW_DROP]) {
        flags[FLAG_RAINBOW_BRIDGE] = true;
        cells[57][72] = CELL_TYPE_BRIDGE;
        display();
        printf("虹のしずくを掲げると\n"
               "虹の橋が架かった!\a");
        getch();
      }
      break;
    case LOCATION_DRAGON_KING:
      if(!flags[FLAG_LIGHT_BALL]) {
        if(flags[FLAG_ROTO_ARMOR]) {
#include "battle.h"
          if(battle()) {
            flags[FLAG_LIGHT_BALL] = true;
            printf("\033[2J");
            display();
            printf("竜王を倒した!\a");
            getch();
            printf("\n光の玉を手に入れた!\a");
            getch();
          } else {
            printf("負けてしまった...\a");
            getch();
            player_dead();
          }
        } else {
          printf("竜王の圧倒的な力に\n"
                 "まったく歯が立たなかった...\a");
          getch();
          player_dead();
        }
      }
      break;
  }
}

int main()
{
  char filename[] = "map.bmp";
  FILE *fp = fopen(filename, "rb");
  if(fp == NULL) {
    printf("%s open failed!\n", filename);
    return 1;
  }

  BMPFILEHEADER bh;
  BMPINFOHEADER bi;

  fread(&bh, sizeof(BMPFILEHEADER), 1, fp);
  fread(&bi, sizeof(BMPINFOHEADER), 1, fp);

  for(int y=0; y<FIELD_HEIGHT; y++)
    for(int x=0; x<FIELD_WIDTH; x++) {
      if(0==y && 0==x) x=19;
      BGR bgr;
      fread(&bgr, sizeof(BGR), 1, fp);
      int cell_type = -1;
      for(int i=0; i<CELL_TYPE_MAX; i++) {
        if((bgr.b == cell_descs[i].bgr.b)
         &&(bgr.g == cell_descs[i].bgr.g)
         &&(bgr.b == cell_descs[i].bgr.b)) //&&(bgr.r == cell_descs[i].bgr.r))
          cell_type = i;
      }
      if(cell_type < 0) {
        printf("Unknown cell type! x:%d y:%d b:%d g:%d r:%d\n",
            x, y, bgr.b, bgr.g, bgr.r);
        return 1;
      }

      cells[FIELD_HEIGHT-1-y][x] = cell_type;
    }


  fclose(fp);

  printf("\033[2J\033[H\033[?25l");

  while(!quit) {
    display();

    int x = player_x;
    int y = player_y;
    switch(getch()) {
      case 'h': x--;         break;
      case 'j': y++;         break;
      case 'k': y--;         break;
      case 'l': x++;         break;
      case 'q': quit = true; break;
    }
    if(cell_descs[cells[y][x]].canPen) {
      player_x = x;
      player_y = y;
      event();
    }
  }

  /* ALL MAP
  for(int y=0; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++) {
      printf("%s", cell_descs[cells[y][x]].aa);
    }
    printf("\n");
  } //*/

  printf("\033[?25h");

}
