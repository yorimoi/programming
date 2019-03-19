#include <stdio.h>
#include <string.h>
#include <termios.h>

#define WIDTH  10
#define HEIGHT 10

int onpu[HEIGHT][WIDTH] =
{
  {0,0,0,0,1,1,1,1,0,0},
  {0,0,0,0,1,1,1,1,1,0},
  {0,0,0,0,1,1,1,1,1,1},
  {0,0,0,0,1,0,0,1,1,1},
  {0,0,0,0,1,0,0,0,1,1},
  {0,0,0,0,1,0,0,0,1,1},
  {0,0,0,0,1,0,0,0,0,0},
  {1,1,1,1,1,0,0,0,0,0},
  {1,1,1,1,1,0,0,0,0,0},
  {1,1,1,1,1,0,0,0,0,0},
};

// ToDo function
int hint_vertical[HEIGHT/2][WIDTH] =
{
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { 3, 3, 3, 3,10, 3, 3, 4, 5, 4},
};
int hint_horizontal[HEIGHT][WIDTH/2] =
{
  { 0, 0, 0, 0, 4,},
  { 0, 0, 0, 0, 5,},
  { 0, 0, 0, 0, 6,},
  { 0, 0, 0, 1, 3,},
  { 0, 0, 0, 1, 2,},
  { 0, 0, 0, 1, 2,},
  { 0, 0, 0, 0, 1,},
  { 0, 0, 0, 0, 5,},
  { 0, 0, 0, 0, 5,},
  { 0, 0, 0, 0, 5,},
};

int status[HEIGHT][WIDTH];

int cursor_x, cursor_y;

enum {
  STAT_NONE,
  STAT_DIG,
  STAT_MARK,
};

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

void draw()
{
  printf("\033[H\033[0m");

  for(int y=0; y<HEIGHT/2; y++) {
    for(int x=0; x<WIDTH/2; x++)
      printf("  ");
    for(int x=0; x<WIDTH; x++) {
      printf(x%2?"\033[48;5;008m":"\033[48;5;242m");
      if(hint_vertical[y][x])
        printf("%2d", hint_vertical[y][x]);
      else
        printf("  ");
      printf("\033[0m");
    }
    printf("\n");
  }
  for(int y=0; y<HEIGHT; y++) {
    printf(y%2?"\033[48;5;008m":"\033[48;5;242m");
    for(int x=0; x<WIDTH/2; x++) {
      if(hint_horizontal[y][x])
        printf("%2d", hint_horizontal[y][x]);
      else
        printf("  ");
    }
    for(int x=0; x<WIDTH; x++) {
      printf("\033[47;30m");
      if(x==cursor_x && y==cursor_y)
        printf("\033[31m @");
      else if(STAT_DIG==onpu[y][x])
        printf(" #");
      else if(STAT_MARK==onpu[y][x])
        printf("\033[38;5;208m X");
      else
        printf(" .");
      printf("\033[0m");
    }
    printf("\n");
  }
}

int figcmp(int correct[][WIDTH], int target[][WIDTH])
{
  for(int y=0; y<HEIGHT; y++)
    for(int x=0; x<WIDTH; x++)
      if(
          (correct[y][x] == STAT_NONE && target[y][x] == STAT_DIG)
          || (correct[y][x] == STAT_DIG && target[y][x] != STAT_DIG)
        )
        return 1;
  return 0;
};

int main()
{
  int quit = 0;
  int correct[HEIGHT][WIDTH];
  memcpy(correct, onpu, sizeof onpu);
  memset(onpu, 0, sizeof onpu);

  printf("\033[2J\033[H\033[?25l");

  while(!quit) {
    draw();
    switch(getch()) {
      case 'h': if(cursor_x>0)        cursor_x--;      break;
      case 'j': if(cursor_y<HEIGHT-1) cursor_y++;      break;
      case 'k': if(cursor_y>0)        cursor_y--;      break;
      case 'l': if(cursor_x<WIDTH-1)  cursor_x++;      break;
      case 'q': quit = 1;     break;
      case 'x':
                if(onpu[cursor_y][cursor_x]!=STAT_DIG)
                  onpu[cursor_y][cursor_x] ^= STAT_MARK;
                break;
      case ' ':
                if(onpu[cursor_y][cursor_x]!=STAT_MARK)
                  onpu[cursor_y][cursor_x] ^= STAT_DIG;
                if(!figcmp(correct, onpu)) {
                  quit = 1;
                  printf("Congaatulations!\n\a");
                }
                break;
    }
  }

  printf("\033[?25h");

  return 0;
}
