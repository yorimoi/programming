#include <stdio.h>
#include <termios.h>

#define FIELD_WIDTH  8
#define FIELD_HEIGHT 8

enum {
  STONE_NONE,
  STONE_BLACK,
  STONE_WHITE,
  STONE_MAX
};

int BOARD[FIELD_HEIGHT][FIELD_WIDTH];
int NOW_TURN  = STONE_WHITE;
int CNT       = 4;

int getch()
{
  char c=0;
  struct termios old={0};
  tcgetattr(0, &old);
  old.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &old);
  c = getchar();
  tcsetattr(0, TCSADRAIN, &old);
  return c;
}

void upset(int _x, int _y)
{
  int opponent = STONE_BLACK==NOW_TURN ? STONE_WHITE : STONE_BLACK;
  for(int y=-1; y<2; y++)
    for(int x=-1; x<2; x++) {
      int x2        = _x + x;
      int y2        = _y + y;
      int c         = 0;
      int turn_over = 0;

      while(1) {
        if(x2<0 || x2>FIELD_WIDTH-1 || y2<0 || y2>FIELD_HEIGHT-1)
          break;
        if(opponent==BOARD[y2][x2]) {
          c++;
        } else {
          if(NOW_TURN==BOARD[y2][x2])
            turn_over = 1;
          break;
        }
        x2+=x;
        y2+=y;
      }
      if(turn_over) {
        for(int _=c; _>0; _--) {
          x2-=x;
          y2-=y;
          BOARD[y2][x2] = NOW_TURN;
        }
      }
    }
}

int main()
{
  BOARD[3][3] = BOARD[4][4] = STONE_WHITE;
  BOARD[3][4] = BOARD[4][3] = STONE_BLACK;
  int cursor_x  = 0;
  int cursor_y  = 0;
  int quit_flag = 0;

  printf("\033[2J\033[?25l\033[42m");

  while(!quit_flag)
  {
    printf("\033[H");
    printf("\033[30m +-----------------+ \n");
    for(int y=0; y<FIELD_HEIGHT; y++) {
      printf("\033[30m |");
      for(int x=0; x<FIELD_WIDTH; x++) {
        if(cursor_x==x && cursor_y==y)
          printf("\033[31m @");
        else if(STONE_WHITE==BOARD[y][x])
          printf("\033[37m O");
        else if(STONE_BLACK==BOARD[y][x])
          printf("\033[30m X");
        else
          printf("\033[30m -");
      }
      printf("\033[30m | \n");
    }
    printf("\033[30m +-----------------+ \n");
    printf(" \033[30mTURN: ");
    printf("%s]           \n",
        NOW_TURN==STONE_WHITE?"\033[37m[O":"\033[30m[X");

    if(FIELD_WIDTH*FIELD_HEIGHT == CNT)
    {
      int w=0, b=0;
      printf("\033[H");
      printf("\033[30m +-----------------+ \n");
      for(int y=0; y<FIELD_HEIGHT; y++) {
        printf("\033[30m |");
        for(int x=0; x<FIELD_WIDTH; x++) {
          if(STONE_WHITE==BOARD[y][x]) {
            printf("\033[37m O");
            w++;
          } else {
            printf("\033[30m X");
            b++;
          }
        }
        printf("\033[30m | \n");
      }
      printf("\033[30m +-----------------+ \n");
      w>b ? printf("\033[37m   [ WHITE  WIN! ]   \n") :
        w<b ? printf("\033[30m   [ BLACK  WIN! ]   \n") :
          printf("\033[31m     [ DRAW... ]     \n");
      printf("    \033[37mwhite   \033[30mblack    \n"
             "      \033[37m%02d      \033[30m%02d     \n", w, b);
      quit_flag = 1;
      continue;
    }

    switch(getch())
    {
      case 'h': if(cursor_x>0)              cursor_x--; break;
      case 'j': if(cursor_y<FIELD_HEIGHT-1) cursor_y++; break;
      case 'k': if(cursor_y>0)              cursor_y--; break;
      case 'l': if(cursor_x<FIELD_WIDTH-1)  cursor_x++; break;
      case 'q': quit_flag = 1; break;
      case ' ':
                if(STONE_NONE==BOARD[cursor_y][cursor_x])
                {
                  BOARD[cursor_y][cursor_x] = NOW_TURN;
                  upset(cursor_x, cursor_y);
                  NOW_TURN = STONE_BLACK==NOW_TURN ? STONE_WHITE : STONE_BLACK;
                  CNT++;
                }
      default: break;
    }
  }

  printf("\033[?25h\033[0m");

  return 0;
}
