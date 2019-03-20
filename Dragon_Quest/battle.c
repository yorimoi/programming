#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>

#include "battle.h"

#define SWORD_OF_LOTO    40
#define ARMOR_OF_LOTO    28
#define WORTER_OF_SHIELD 20
#define SPELL_COST       10

int _getch()
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

enum {
  COMMAND_ATTACK,
  COMMAND_SPELL,
  COMMAND_MAX
};

char command_name[][4*3+1] =
{
  "たたかう", // COMMAND_ATTACK
  "ベホイミ", // COMMAND_SPELL
};

typedef struct {
  int strength;
  int agility;
  int maxHP;
  int maxMP;

  int level;
  int attack;
  int defence;
  int HP;
  int MP;
}CHARACTER;

CHARACTER statusTable[] =
{
  {  4,   4,  15,   0},
  {  5,   4,  22,   0},
  {  7,   6,  24,   5},
  {  7,   8,  31,  16},
  { 12,  10,  35,  20},
  { 16,  10,  38,  24},
  { 18,  17,  40,  26},
  { 22,  20,  46,  29},
  { 30,  22,  50,  36},
  { 35,  31,  54,  40},
  { 40,  35,  62,  50},
  { 48,  40,  63,  58},
  { 52,  48,  70,  64},
  { 60,  55,  78,  70},
  { 68,  64,  86,  72},
  { 72,  70,  92,  95},
  { 72,  78, 100, 100},
  { 85,  84, 115, 108},
  { 87,  86, 130, 115},
  { 92,  88, 138, 128},
  { 95,  90, 149, 135},
  { 97,  90, 158, 146},
  { 99,  94, 165, 153},
  {103,  98, 170, 161},
  {113, 100, 174, 161},
  {117, 105, 180, 168},
  {125, 107, 189, 175},
  {130, 115, 195, 180},
  {135, 120, 200, 190},
  {140, 130, 210, 200},
};

CHARACTER player;
CHARACTER enemy;

int command;

void draw()
{
  printf("\033[H\n");
  printf(" +-----勇者-----+\n"
         " | Level   %4d |\n"
         " | HP      %4d |\n"
         " | MP      %4d |\n"
         " +--------------+\n"
  ,player.level
  ,player.HP
  ,player.MP
  );
#include "aa.h"
  if(enemy.HP <= 0)
    for(int i=0; i<23; i++)
      printf("\n");
  else
    printf(aa);
}

//int main()
int battle()
{
  int clear = 0;
  srand((unsigned int)time(NULL));

  {
    int level = 20;
    player         = statusTable[level-1];
    player.level   = level;
    player.attack  = player.strength + SWORD_OF_LOTO;
    player.defence = player.agility / 2 + ARMOR_OF_LOTO + WORTER_OF_SHIELD;
    player.HP      = player.maxHP;
    player.MP      = player.maxMP;
  }
  {
    enemy.attack   = 140;
    enemy.defence  = 200;
    enemy.HP       = 130;
  }

  printf("\033[2J\a");
  draw();
  printf(" +-----------------------+\n"
         " | 竜王が しょうたいを   |\n"
         " | あらわした!           |\n"
         " |                       |\n"
         " +-----------------------+\n");
  _getch();

  while(1) {
    draw();
    while(1) {
      draw();
      printf(" +--------コマンド-------+\n");
      for(int i=0; i<COMMAND_MAX; i++)
        printf(" | %s %s            |\n",
          i==command?">":" ",
          command_name[i]
        );
      printf(" |                       |\n");
      printf(" +-----------------------+\n");

      int ch = _getch();
      if(ch == 0x0a) {
        break;
      }
      else {
        switch(ch) {
          case 'j': command--; break;
          case 'k': command++; break;
          case 'q': printf("\033[2J"); return 0;
        }
        command = (COMMAND_MAX + command) % COMMAND_MAX;
      }
    }

    draw();
    switch(command) {
      case COMMAND_ATTACK:
        {
          printf(" +-----------------------+\n"
                 " | 勇者の こうげき!      |\n"
                 " |                       |\n"
                 " |                       |\n"
                 " +-----------------------+\n");
          _getch();
          int damageBase = player.attack - enemy.defence / 2;
          int damageMin  = damageBase / 4;
          int damageMax  = damageBase / 2;
          int damage     = damageMin + rand() % (damageMax - damageMin);
          enemy.HP -= damage;
          draw();
          printf(" +-----------------------+\n"
                 " | 勇者の こうげき!      |\n"
                 " | 竜王に %2dポイントの   |\n"
                 " | ダメージを あたえた!  |\n"
                 " +-----------------------+\n"
          , damage);
          _getch();
        }

        break;
      case COMMAND_SPELL:
        {
          if(player.MP < SPELL_COST) {
            draw();
            printf(" +-----------------------+\n"
                   " | MPが たりない!        |\n"
                   " |                       |\n"
                   " |                       |\n"
                   " +-----------------------+\n");
            _getch();
            continue;
          }
          player.MP -= SPELL_COST;
          draw();
          printf(" +-----------------------+\n"
                 " | 勇者は ベホイミの     |\n"
                 " | じゅもんを となえた!  |\n"
                 " |                       |\n"
                 " +-----------------------+\n");
          _getch();
          int heal = 85 + rand() % 16;
          if(player.HP + heal > player.maxHP)
            heal = player.maxHP - player.HP;
          player.HP += heal;

          draw();
          printf(" +-----------------------+\n"
                 " | 勇者は ベホイミの     |\n"
                 " | じゅもんを となえた!  |\n"
                 " | %3dポイント 回復した! |\n"
                 " +-----------------------+\n"
          , heal);
          _getch();
          draw();
        }
        break;
    }

    draw();
    if(enemy.HP > 0) {
      if(rand()%2) {
        printf(" +-----------------------+\n"
               " | 竜王の こうげき!      |\n"
               " |                       |\n"
               " |                       |\n"
               " +-----------------------+\n");
        int damageBase = enemy.attack - player.defence / 2;
        int damageMin  = damageBase / 4;
        int damageMax  = damageBase / 2;
        int damage     = damageMin + rand() % (damageMax - damageMin);
        player.HP -= damage;
        if(player.HP < 0)
          player.HP = 0;
        _getch();
        draw();
        printf(" +-----------------------+\n"
               " | 竜王の こうげき!      |\n"
               " | 勇者は %2dポイントの   |\n"
               " | ダメージを うけた!    |\n"
               " +-----------------------+\n"
        , damage);
      } else {
        printf(" +-----------------------+\n"
               " | 竜王は 炎を吐いた!    |\n"
               " |                       |\n"
               " |                       |\n"
               " +-----------------------+\n");
        _getch();
        int damage = (64 + (rand()%8)) * 2 / 3;
        player.HP -= damage;
        if(player.HP < 0)
          player.HP = 0;
        draw();
        printf(" +-----------------------+\n"
               " | 竜王は 炎を吐いた!    |\n"
               " | 勇者は %2dポイントの   |\n"
               " | ダメージを うけた!    |\n"
               " +-----------------------+\n"
        , damage);
      }
      _getch();
    }

    if(enemy.HP <= 0) {
      draw();
      printf(" +-----------------------+\n"
             " | 竜王を たおした!      |\n"
             " |                       |\n"
             " |                       |\n"
             " +-----------------------+\n");
      _getch();
      draw();
      printf(" +-----------------------+\n"
             " | 光の玉を とりもどし   |\n"
             " | 世界に 平和が         |\n"
             " | もどった!             |\n"
             " +-----------------------+\n");
      _getch();
      draw();
      printf(" +-----------------------+\n"
             " |                       |\n"
             " |     T H E   E N D     |\n"
             " |                       |\n"
             " +-----------------------+\n");
      clear = 1;
      break;
    }

    if(player.HP <= 0) {
      draw();
      printf(" +-----------------------+\n"
             " | ぜんめつ した...      |\n"
             " |                       |\n"
             " |                       |\n"
             " +-----------------------+\n");
      _getch();
      break;
    }

  }
  //_getch();
  printf("\033[2J");
  return clear;
}
