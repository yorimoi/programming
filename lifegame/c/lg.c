#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define loop for(;;)

typedef int bool;
#define true  1
#define false 0

#define FIELD_WIDTH  32
#define FIELD_HEIGHT 24

bool cells[2][FIELD_HEIGHT][FIELD_WIDTH];
int  current;

char getch()
{
  char buf=0;
  struct termios old={0};
  fflush(stdout);
  if(tcgetattr(0, &old)<0)
    perror("tcsetattr()");
  old.c_lflag&=~(ICANON|ECHO);
  old.c_cc[VMIN]=1;
  old.c_cc[VTIME]=0;
  if(tcsetattr(0, TCSANOW, &old)<0)
    perror("tcsetattr ICANON");
  if(read(0,&buf,1)<0)
    perror("read()");
  if(tcsetattr(0, TCSADRAIN, &old)<0)
    perror ("tcsetattr ~ICANON");
  return buf;
}

int getAdjacentLivesCount(unsigned int _x, unsigned int _y)
{
  int count = 0;
  for(int y=-1; y<=1; y++)
  {
    for(int x=-1; x<=1; x++)
    {
      if( 0==x && 0==y ) continue;
      unsigned int x2 = (_x + x) % FIELD_WIDTH;
      unsigned int y2 = (_y + y) % FIELD_HEIGHT;
      //if( FIELD_WIDTH <= x2 || FIELD_HEIGHT <= y2 ) continue;
      count += cells[current][y2][x2];
    }
  }

  return count;
}

int main()
{
  unsigned int cursorX = 0;
  unsigned int cursorY = 0;

  printf("\033[H\033[2J\033[?25l");
  setvbuf(stdout, NULL, _IOFBF, FIELD_WIDTH * FIELD_HEIGHT * 3);

  loop {
    for(unsigned int y=0; y<FIELD_HEIGHT; y++)
    {
      for(unsigned int x=0; x<FIELD_WIDTH; x++)
      {
        if( x==cursorX && y==cursorY )
          printf(" @");
        else
          printf(" %s", cells[current][y][x]?"\033[38;5;047m#\033[0m":".");
      }
      printf("\n");
    }
    fflush(stdout);

    switch( getch() )
    {
      case 'k':
      case 'w': if(cursorY > 0)               cursorY--; break;
      case 'j':
      case 's': if(cursorY < FIELD_HEIGHT -1) cursorY++; break;
      case 'h':
      case 'a': if(cursorX > 0)               cursorX--; break;
      case 'l':
      case 'd': if(cursorX < FIELD_WIDTH  -1) cursorX++; break;
      case 'q':
                printf("\033[?25h");
                exit(0);
      case ' ': cells[current][cursorY][cursorX] ^= true; break;
      case 'c':
            for(unsigned int y=0; y<FIELD_HEIGHT; y++)
              for(unsigned int x=0; x<FIELD_WIDTH; x++)
                cells[current][y][x] = false;
            break;
      case 'n':
            for(unsigned int y=0; y<FIELD_HEIGHT; y++)
            {
              for(unsigned int x=0; x<FIELD_WIDTH; x++)
              {
                int n = getAdjacentLivesCount(x, y);
                bool next = cells[current][y][x];
                if( cells[current][y][x] )
                {
                  if( n<=1 || n>=4 )
                    next = false;
                }
                else
                {
                  if( n==3 )
                    next = true;
                }
                cells[current^1][y][x] = next;
              }
            }
            current ^= 1;
            break;
    }
    printf("\033[H");
  }

  return 0;
}
