#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>

#define FIELD_WIDTH  31
#define FIELD_HEIGHT 15

int getch()
{
  char c=0;
  struct termios old={0};
  fflush(stdout);
  if(tcgetattr(0, &old)<0)
    perror("tcsetattr()");
  old.c_lflag&=~(ICANON|ECHO);
  old.c_cc[VMIN]=1;
  old.c_cc[VTIME]=0;
  if(tcsetattr(0, TCSANOW, &old)<0)
    perror("tcsetattr ICANON");
  c = getchar();
  if(tcsetattr(0, TCSADRAIN, &old)<0)
    perror ("tcsetattr ~ICANON");
  return c;
}

/* 棒倒し法 */
void boutaoshi(int cell[][FIELD_WIDTH])
{
  for(int y=0; y<FIELD_HEIGHT; y++)
  {
    for(int x=0; x<FIELD_WIDTH; x++)
    {
      if( 0==y || FIELD_HEIGHT-1==y || 0==x || FIELD_WIDTH-1==x
          || (!(x%2)&&!(y%2)) )
        cell[y][x] = 1;
    }
  }

  for(int y=2; y<FIELD_HEIGHT-1; y+=2)
  {
    for(int x=2; x<FIELD_WIDTH-1; x+=2)
    {
      int break_flag = 0;
      do {
        switch(2==y?rand()%4:rand()%3) {
          /* Right */
          case 0: if(!cell[y][x+1]){cell[y][x+1]=1;break_flag=1;} break;
          /* Down */
          case 1: if(!cell[y+1][x]){cell[y+1][x]=1;break_flag=1;} break;
          /* Left */
          case 2: if(!cell[y][x-1]){cell[y][x-1]=1;break_flag=1;} break;
          /* Up */
          case 3: if(!cell[y-1][x]){cell[y-1][x]=1;break_flag=1;} break;
        }
        if(break_flag) break;
      } while(1);
    }
  }
}

/* 穴掘り法 */
void dig(int cell[][FIELD_WIDTH], int x, int y)
{
  u_int8_t b, dir;
  while(1) {
    int break_flag = 0;
    dir = rand() % 4;
    switch(dir) {
      /* Up */
      case 0:
        if(cell[y-2][x] && cell[y-1][x])
        {
          cell[--y][x] = 0;
          break_flag = 1;
        }
        b |= 1;
        break;
      /* Right */
      case 1:
        if(cell[y][x+2] && cell[y][x+1])
        {
          cell[y][++x] = 0;
          break_flag = 1;
        }
        b |= 2;
        break;
      /* Down */
      case 2:
        if(cell[y+2][x] && cell[y+1][x])
        {
          cell[++y][x] = 0;
          break_flag = 1;
        }
        b |= 4;
        break;
      /* Left */
      case 3:
        if(cell[y][x-2] && cell[y][x-1])
        {
          cell[y][--x] = 0;
          break_flag = 1;
        }
        b |= 8;
        break;
    }
    if(b==15) return;
    if(break_flag) break;
    printf("[%d]", b);
  }

  {
    printf("\033[2J\033[H");
    for(int y=0; y<FIELD_HEIGHT; y++)
    {
      for(int x=0; x<FIELD_WIDTH; x++)
      {
        printf(cell[y][x]?" #":"  ");
      }
      printf("\n");
    }
    getch();
  }

  dig(cell, x, y);
}
void anahori(int cell[][FIELD_WIDTH])
{
  for(int y=0; y<FIELD_HEIGHT; y++)
  {
    for(int x=0; x<FIELD_WIDTH; x++)
    {
      if( 0==y || FIELD_HEIGHT-1==y || 0==x || FIELD_WIDTH-1==x )
        cell[y][x] = 0;
      else
        cell[y][x] = 1;
    }
  }
  int x = (rand()%(FIELD_WIDTH-4)) + 2;
  int y = (rand()%(FIELD_HEIGHT-4)) + 2;
  x%2 ? : x--;
  y%2 ? : y--;
  cell[y][x] = 0;
  int break_flag = 0;
  while(!break_flag)
    switch(rand()%4) {
      case 0: if(y-1>1) {              y--; break_flag = 1;} break;
      case 1: if(x+1<FIELD_WIDTH-2) {  x++; break_flag = 1;} break;
      case 2: if(y+1<FIELD_HEIGHT-2) { y++; break_flag = 1;} break;
      case 3: if(x-1>1) {              x--; break_flag = 1;} break;
    }
  cell[y][x] = 0;
  dig(cell, x, y);
}

int main()
{
  struct pos {
    int x;
    int y;
  };
  struct pos start, goal;
  start.x = 1;
  start.y = 1;
  goal.x  = FIELD_WIDTH-2;
  goal.y  = FIELD_HEIGHT-2;
  int cursor_x = start.x;
  int cursor_y = start.y;

  srand((unsigned int)time(NULL));

  int cell[FIELD_HEIGHT][FIELD_WIDTH] = {};
  int footprint[FIELD_HEIGHT][FIELD_WIDTH] = {};
  boutaoshi(cell);
  //cursor_x = cursor_y = start.x  = start.y  = 2;
  //anahori(cell);


  printf("\033[2J");
  setvbuf(stdout, NULL, _IOFBF, FIELD_WIDTH * FIELD_HEIGHT * 2);

  while(1)
  {
    printf("\033[H");

    for(int y=0; y<FIELD_HEIGHT; y++)
    {
      for(int x=0; x<FIELD_WIDTH; x++)
      {
        //if(footprint[y][x])
          //printf("\033[48;5;001m");
        if(x==cursor_x && y==cursor_y)
          printf(" @");
        else if(1==cell[y][x])
          printf(" #");
        else if(start.y==y && start.x==x)
          printf(" \033[38;5;004mS");
        else if(goal.y==y && goal.x==x)
          printf(" \033[38;5;002mG");
        else if(footprint[y][x])
          printf(" .");
        else
          printf("  ");
        printf("\033[0m");
      }
      printf("\n");
    }

    fflush(stdout);

    switch(getch())
    {
      case 'h': if(!cell[cursor_y][cursor_x-1]) cursor_x--; break;
      case 'j': if(!cell[cursor_y+1][cursor_x]) cursor_y++; break;
      case 'k': if(!cell[cursor_y-1][cursor_x]) cursor_y--; break;
      case 'l': if(!cell[cursor_y][cursor_x+1]) cursor_x++; break;
      case 'q': exit(0);
      default: break;
    }
    footprint[cursor_y][cursor_x] = 1;
    if(goal.y==cursor_y && goal.x==cursor_x)
    {
      printf("\aCongratulations!\n");
      break;
    }
  }

  return 0;
}
