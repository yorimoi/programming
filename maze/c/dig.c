#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>

#define FIELD_WIDTH  31
#define FIELD_HEIGHT 15

#define AISLE 0
#define WALL  1
#define FOOT  1

int CELL[FIELD_HEIGHT][FIELD_WIDTH];
int cursor_x = 2;
int cursor_y = 2;

int getch()
{
  char c=0;
  struct termios o={0};
  tcgetattr(0, &o);
  o.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &o);
  c = getchar();
  tcsetattr(0, TCSADRAIN, &o);
  return c;
}

void dig(int x, int y)
{
  int b = 0;
  while(1)
  {
    switch(rand()%4)
    {
      // Up
      case 0:
        if(CELL[y-2][x]){
          CELL[y-1][x] = CELL[y-2][x] = AISLE;
          dig(x, y-2);
        }
        else
          b|=1; break;
      // Right
      case 1:
        if(CELL[y][x+2]){
          CELL[y][x+1] = CELL[y][x+2] = AISLE;
          dig(x+2, y);
        }
        else b|=2; break;
      // Down
      case 2:
        if(CELL[y+2][x]){
          CELL[y+1][x] = CELL[y+2][x] = AISLE;
          dig(x, y+2);
        }
        else b|=4; break;
      // Left
      case 3:
        if(CELL[y][x-2]){
          CELL[y][x-1] = CELL[y][x-2] = AISLE;
          dig(x-2, y);
        }
        else b|=8; break;
    }
    if(15==b) return;
  }
}

void init_maze()
{
  srand((unsigned int)time(NULL));
  int x = rand()%(FIELD_WIDTH-4) + 2;
  int y = rand()%(FIELD_HEIGHT-4) + 2;
  if(x%2) x--;
  if(y%2) y--;
  dig(x, y);
}

int main()
{
  int quit_flag = 0;
  int footprint[FIELD_HEIGHT][FIELD_WIDTH] = {0};
  struct pos {
    int x;
    int y;
  };
  struct pos start, goal;
  start.x = start.y = 2;
  goal.x = FIELD_WIDTH-3;
  goal.y = FIELD_HEIGHT-3;

  for(int y=1; y<FIELD_HEIGHT-1; y++)
    for(int x=1; x<FIELD_WIDTH-1; x++)
      CELL[y][x] = WALL;

  init_maze();

  printf("\033[2J\033[?25l");
  setvbuf(stdout, NULL, _IOFBF, FIELD_WIDTH * FIELD_HEIGHT * 2);

  while(!quit_flag)
  {
    printf("\033[H");
    for(int y=0; y<FIELD_HEIGHT; y++) {
      for(int x=0; x<FIELD_WIDTH; x++) {
        if(y==cursor_y && x==cursor_x)
          printf(" @");
        else if(start.y==y && start.x==x)
          printf(" S");
        else if(goal.y==y && goal.x==x)
          printf(" G");
        else if(footprint[y][x])
          printf(" .");
        else if(CELL[y][x])
          printf(" #");
        else
          printf("  ");
      }
      printf("\n");
    }
    fflush(stdout);

    switch(getch())
    {
      case 'h': if(!CELL[cursor_y][cursor_x-1]) cursor_x--; break;
      case 'j': if(!CELL[cursor_y+1][cursor_x]) cursor_y++; break;
      case 'k': if(!CELL[cursor_y-1][cursor_x]) cursor_y--; break;
      case 'l': if(!CELL[cursor_y][cursor_x+1]) cursor_x++; break;
      case 'q': quit_flag=1;
    }
    footprint[cursor_y][cursor_x] = FOOT;
    if(cursor_y==goal.y && cursor_x==goal.x)
    {
      printf("Congratulations!\a\n");
      quit_flag = 1;
    }
  }

  printf("\033[?25h");

  return 0;
}
