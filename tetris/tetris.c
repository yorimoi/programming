/* https://youtu.be/iosmmQvhyzM */
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lce.h"

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 22

char FIELD[FIELD_HEIGHT][FIELD_WIDTH];
char FIELD_BUF[FIELD_HEIGHT][FIELD_WIDTH];

/* -----------------------------------------------------------------------
 ■ H O L D ■                     ■ N E X T ■
 ■         ■                     ■         ■
 ■         ■                     ■         ■
 ■         ■                     ■         ■
 ■         ■                     ■         ■
 ■ ■ ■ ■ ■ ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■     ■                     ■         ■
     ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ■
------------------------------------------------------------------------- */

typedef struct {
  int MINO_TYPE;
} MINO_COLOR;
MINO_COLOR COLORS[FIELD_HEIGHT][FIELD_WIDTH];

enum {
  MINO_TYPE_NONE,
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

#define MINO_WIDTH  4
#define MINO_HEIGHT 4
char MINO_SHAPES[MINO_TYPE_MAX][MINO_ANGLE_MAX][MINO_HEIGHT][MINO_WIDTH] =
{
  /* MINO_TYPE_I */
  {
    {
      {0, 0, 0, 0},
      {1, 1, 1, 1},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
    },
    {
      {0, 0, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {1, 1, 1, 1},
      {0, 0, 0, 0},
    },
    {
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
    },
  },
  /* MINO_TYPE_O */
  {
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
    },
  },
  /* MINO_TYPE_S */
  {
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {1, 1, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {1, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
    },
  },
  /* MINO_TYPE_Z */
  {
    {
      {0, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 0, 1, 0},
      {0, 1, 1, 0},
      {0, 1, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 1, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {1, 0, 0, 0},
    },
  },
  /* MINO_TYPE_J */
  {
    {
      {0, 0, 0, 0},
      {1, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 1, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 0, 0},
    },
  },
  /* MINO_TYPE_L */
  {
    {
      {0, 0, 0, 0},
      {0, 0, 1, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {1, 1, 1, 0},
      {1, 0, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
    },
  },
  /* MINO_TYPE_T */
  {
    {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 1, 0, 0},
    },
    {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
    },
  },
};

int  MINO_BUF;
int  MINO_HOLD = -1;
bool HOLD_FLAG = 2;

void display(int _minoX, int _minoY, int _minoType, int _minoAngle)
{
  memcpy(FIELD_BUF, FIELD, sizeof FIELD);

  cursorPos(0, 0);

  printf("\n");

  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++) {
      FIELD_BUF[_minoY+y][_minoX+x] |= MINO_SHAPES[_minoType][_minoAngle][y][x];
      int buf;
      switch(_minoType) {
        case 0:  buf = MINO_TYPE_I;    break;
        case 1:  buf = MINO_TYPE_O;    break;
        case 2:  buf = MINO_TYPE_S;    break;
        case 3:  buf = MINO_TYPE_Z;    break;
        case 4:  buf = MINO_TYPE_J;    break;
        case 5:  buf = MINO_TYPE_L;    break;
        case 6:  buf = MINO_TYPE_T;    break;
        default: buf = MINO_TYPE_NONE; break;
      }
      if( MINO_SHAPES[_minoType][_minoAngle][y][x] )
        COLORS[_minoY+y][_minoX+x].MINO_TYPE = buf;
    }

  for(int y=0; y<FIELD_HEIGHT; y++) {
    switch(y) {
      case 0:  printf(" ■ H O L D"); break;
      case 1:
      case 2:
      case 3:
      case 4:  printf(" ■");
               if(0 <= MINO_HOLD) {
                 switch(MINO_HOLD) {
                   case MINO_TYPE_I-1: printf(CYAN);             break;
                   case MINO_TYPE_O-1: printf(YELLOW);           break;
                   case MINO_TYPE_S-1: printf(GREEN);            break;
                   case MINO_TYPE_Z-1: printf(RED);              break;
                   case MINO_TYPE_J-1: printf(BLUE);             break;
                   case MINO_TYPE_L-1: printf("\033[38;5;208m"); break;
                   case MINO_TYPE_T-1: printf(MAGENTA);          break;
                   default: break;
                 }
                 for(int i=0; i<MINO_WIDTH; i++)
                   printf(MINO_SHAPES[MINO_HOLD][MINO_ANGLE_0][y-1][i]?" ■":"  ");
               } else {
                 printf("        ");
               }
               resetColor();
               break;
      case 5:  printf(" ■ ■ ■ ■ ■"); break;
      case FIELD_HEIGHT-1:  printf("     ■ ■ ■"); break;
      default: printf("     ■    "); break;
    }
    for(int x=0; x<FIELD_WIDTH; x++) {
      switch(COLORS[y][x].MINO_TYPE) {
        case MINO_TYPE_I: printf(CYAN);             break;
        case MINO_TYPE_O: printf(YELLOW);           break;
        case MINO_TYPE_S: printf(GREEN);            break;
        case MINO_TYPE_Z: printf(RED);              break;
        case MINO_TYPE_J: printf(BLUE);             break;
        case MINO_TYPE_L: printf("\033[38;5;208m"); break;
        case MINO_TYPE_T: printf(MAGENTA);          break;
        default:          resetColor();
      }
      printf(FIELD_BUF[y][x]?" ■":"  ");
      resetColor();
    }
    switch(y) {
      case 0:  printf(" N E X T ■");     break;
      case FIELD_HEIGHT-1:
               printf(" ■ ■ ■ ■ ■"); break;
      default: printf("         ■");     break;
    }
    printf("\n");
  }
  fflush(stdout);
}

bool isHit(int _minoX, int _minoY, int _minoType, int _minoAngle)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if( MINO_SHAPES[_minoType][_minoAngle][y][x]
          && FIELD[_minoY+y][_minoX+x] )
        return true;
  return false;
}

int isHitRotate(int _minoX, int _minoY, int _minoType, int _minoAngle)
{
  for(int y=0; y<MINO_HEIGHT; y++)
    for(int x=0; x<MINO_WIDTH; x++)
      if( MINO_SHAPES[_minoType][_minoAngle][y][x]
          && FIELD[_minoY+y][_minoX+x] )
        return x;
  return -1;
}

void initMino(int *_type, int *_type2, int *_cnt, int *_angle, int *_x, int *_y)
{
  *_cnt = MINO_TYPE_MAX - 1;
  for(int _=0; _<10; _++) {
    for(int i=0; i<MINO_TYPE_MAX-2; ++i) {
      int j = rand() % (MINO_TYPE_MAX-2);
      int b = *(_type+j);
      *(_type+j) = *(_type+MINO_TYPE_MAX-2);
      *(_type+MINO_TYPE_MAX-2) = b;

      int j2 = rand() % (MINO_TYPE_MAX-2);
      int b2 = *(_type2+j2);
      *(_type2+j2) = *(_type2+MINO_TYPE_MAX-2);
      *(_type2+MINO_TYPE_MAX-2) = b2;
    }
  }
  *_angle = 0;
  *_x     = 4;
  *_y     = 0;
  MINO_BUF = _type[*_cnt];
}

void nextMino(int *_type, int *_type2, int *_cnt, int *_angle, int *_x, int *_y)
{
  if( 0<*_cnt ) {
    *_cnt-=1;
  } else {
    memcpy(_type, _type2, sizeof(int)*7);
    *_cnt = MINO_TYPE_MAX - 1;
    //int i = MINO_TYPE_MAX;
    //while(i > 1) {
    //  int j    = rand() % i--;
    //  int b    = _type[i];
    //  _type[i] = _type[j];
    //  _type[j] = b;
    //}
    for(int i=0; i<MINO_TYPE_MAX-2; ++i) {
      int j = rand() % (MINO_TYPE_MAX-2);
      int b = *(_type2+j);
      *(_type2+j) = *(_type2+MINO_TYPE_MAX-2);
      *(_type2+MINO_TYPE_MAX-2) = b;
    }
  }
  *_angle = 0;
  *_x     = 4;
  *_y     = 0;
  MINO_BUF = _type[*_cnt];
  if(2!=HOLD_FLAG)
    HOLD_FLAG = false;
}

void gameover()
{
  t_reset();
  printf(" Game Over\n");
  exit(0);
}

int main()
{
  clearScreen();
  t_init();
  srand((unsigned int)time(NULL));
  int  minoX, minoY, minoAngle;
  int  minoCnt = 0;
  int  MINO_TYPE_RAND[MINO_TYPE_MAX]  = {0,1,2,3,4,5,6};
  int  MINO_TYPE_RAND2[MINO_TYPE_MAX] = {0,1,2,3,4,5,6};

  initMino(MINO_TYPE_RAND, MINO_TYPE_RAND2, &minoCnt, &minoAngle, &minoX, &minoY);

  //loop {
  //  for(int i=0; i<7; i++)
  //    printf("[%d]", MINO_TYPE_RAND[i]);
  //  printf("\n");
  //  for(int i=0; i<7; i++)
  //    nextMino(MINO_TYPE_RAND, &minoCnt, &minoAngle, &minoX, &minoY);
  //  getchar();
  //}

  for(int y=0; y<FIELD_HEIGHT; y++) {
    FIELD[y][0] =
    FIELD[y][FIELD_WIDTH-1] = 1;
    COLORS[y][0].MINO_TYPE =
    COLORS[y][FIELD_WIDTH-1].MINO_TYPE = MINO_TYPE_NONE;
  }
  for(int x=0; x<FIELD_WIDTH; x++) {
    FIELD[FIELD_HEIGHT-1][x] = 1;
    COLORS[FIELD_HEIGHT-1][x].MINO_TYPE = MINO_TYPE_NONE;
  }

  time_t t = time(NULL);
  bool kflag = false;

  setvbuf(stdout, NULL, _IOFBF, FIELD_WIDTH * FIELD_HEIGHT * 16);

  loop {

    if( kbhit() ) {
      switch( getchar() ) {
        case 'h':
          if( !isHit(minoX-1, minoY, MINO_BUF, minoAngle) )
            minoX--;
          break;
        case 'j':
          if( !isHit(minoX, minoY+1, MINO_BUF, minoAngle) )
            minoY++;
          break;
        case '\n':
        case 'k':
          while( !isHit(minoX, minoY+1, MINO_BUF, minoAngle) )
            minoY++;
          kflag = true;
          break;
        case 'l':
          if( !isHit(minoX+1, minoY, MINO_BUF, minoAngle) )
            minoX++;
          break;
        case 'a':
        case 'f':
          if( !isHit(minoX, minoY, MINO_BUF, (minoAngle+1)%MINO_ANGLE_MAX) )
            minoAngle = (minoAngle + 1) % MINO_ANGLE_MAX;
          //while( isHit(minoX, minoY, MINO_BUF, minoAngle) )
          //{
          //  if(0<isHitRotate(minoX, minoY, MINO_BUF, minoAngle))
          //    minoX<MINO_WIDTH/2 ? minoX++ : minoX--;
          //}
          break;
        case 'b':
        case 'd':
          if( !isHit(minoX, minoY, MINO_BUF,
              minoAngle-1<0 ? MINO_ANGLE_MAX-1 : minoAngle-1) )
            minoAngle = minoAngle-1<0 ? MINO_ANGLE_MAX-1 : minoAngle-1;
          //while( isHit(minoX, minoY, MINO_BUF, minoAngle) )
          //{
          //  if(0<isHitRotate(minoX, minoY, MINO_BUF, minoAngle))
          //    minoX<MINO_WIDTH/2 ? minoX++ : minoX--;
          //}
          break;
        case ' ':
          if(2==HOLD_FLAG) {
            MINO_HOLD = MINO_BUF;
            nextMino(MINO_TYPE_RAND, MINO_TYPE_RAND2, &minoCnt, &minoAngle, &minoX, &minoY);
            HOLD_FLAG = true;
          } else if(!HOLD_FLAG) {
            HOLD_FLAG = true;
            int b = MINO_HOLD;
            MINO_HOLD = MINO_BUF;
            MINO_BUF = b;
            minoX = 4;
            minoY = 0;
          }
          break;
        case 'q': t_reset(); exit(0);
      }
      display(minoX, minoY, MINO_BUF, minoAngle);
    }

    if( time(NULL)==t && !kflag ) continue;
    t = time(NULL);
    kflag = false;

    if( isHit(minoX, minoY+1, MINO_BUF, minoAngle) ) {
      for(int y=0; y<MINO_HEIGHT; y++)
        for(int x=0; x<MINO_WIDTH; x++) {
          FIELD[minoY+y][minoX+x] |= MINO_SHAPES[MINO_BUF][minoAngle][y][x];
          int buf;
          switch(MINO_BUF) {
            case 0:  buf = MINO_TYPE_I;    break;
            case 1:  buf = MINO_TYPE_O;    break;
            case 2:  buf = MINO_TYPE_S;    break;
            case 3:  buf = MINO_TYPE_Z;    break;
            case 4:  buf = MINO_TYPE_J;    break;
            case 5:  buf = MINO_TYPE_L;    break;
            case 6:  buf = MINO_TYPE_T;    break;
            default: buf = MINO_TYPE_NONE; break;
          }
          if( MINO_SHAPES[MINO_BUF][minoAngle][y][x] )
            COLORS[minoY+y][minoX+x].MINO_TYPE = buf;
        }

      for(int y=0; y<FIELD_HEIGHT-1; y++) {
        bool lineFill = true;
        for(int x=1; x<FIELD_WIDTH-1; x++) {
          if( !FIELD[y][x] ) lineFill = false;
        }
        if( lineFill ) {
          for(int x=y; 0<x; x--) {
            memcpy(FIELD[x], FIELD[x-1], FIELD_WIDTH);
            for(int _x=1; _x<FIELD_WIDTH-1; _x++)
              COLORS[x][_x].MINO_TYPE = COLORS[x-1][_x].MINO_TYPE;
          }
        }
      }

      for(int x=1; x<FIELD_WIDTH-2; x++)
        if( FIELD[0][x] || FIELD[1][x] ) gameover();

      nextMino(MINO_TYPE_RAND, MINO_TYPE_RAND2, &minoCnt, &minoAngle, &minoX, &minoY);
    } else {
      minoY++;
    }

    display(minoX, minoY, MINO_BUF, minoAngle);

  }

  return 0;
}
