#include <stdio.h>
#include <termios.h>

#define FIELD_WIDTH  13
#define FIELD_HEIGHT 6

enum {
  TYPE_WALL,
  TYPE_OBSTACLE,
  TYPE_AISLE_BEFORE,
  TYPE_AISLE_AFTER
};

char AA[][3+1] =
{
  "●", // TYPE_WALL
  "×", // TYPE_OBSTACLE
  "■", // TYPE_AISLE_BEFORE
  "※", // TYPE_AISLE_AFTER
};

int field[FIELD_HEIGHT][FIELD_WIDTH] =
{
  {0,0,0,0,0,0,2,0,0,0,0,0,0,},
  {0,2,2,2,2,2,2,2,2,2,2,2,0,},
  {0,2,2,2,2,2,1,2,2,2,1,2,0,},
  {0,2,2,2,2,2,2,2,2,2,2,2,0,},
  {0,1,2,2,2,2,2,2,2,1,2,2,0,},
  {0,0,0,0,0,0,2,0,0,0,0,0,0,},
};

int player_x = 6;
int player_y = 5;

int getch()
{
  char ch;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  ch = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return ch;
}

void display()
{
  printf("\033[H");
  for(int y=0; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++) {
      if(x==player_x && y==player_y)
        printf("＠");
      else
        printf("%s", AA[field[y][x]]);
    }
    printf("\n");
  }
}

int main()
{
  int quit   = 0;
  int goal_x = 6;
  int goal_y = 0;

  printf("\033[2J\033[H");

  while(!quit) {
    display();

    int px = player_x;
    int py = player_y;
    switch(getch()) {
      case 'h': px--; break;
      case 'j': py++; break;
      case 'k': py--; break;
      case 'l': px++; break;
      case 'q':
        quit = 1;
        continue;
    }
    switch(field[py][px]) {
      case TYPE_AISLE_BEFORE:
        player_x = px;
        player_y = py;
        field[py][px] = TYPE_AISLE_AFTER;
        break;
      case TYPE_AISLE_AFTER:
        player_x = px;
        player_y = py;
        display();
        printf("  GAME OVER!\a\n");
        quit = 1;
        break;
      case TYPE_WALL:
      case TYPE_OBSTACLE:
        // Do nothing
        break;
    }

    if(px==goal_x && py==goal_y) {
      int goal = 1;
      for(int y=1; y<FIELD_HEIGHT-1; y++)
        for(int x=1; x<FIELD_WIDTH-1; x++)
          if(field[y][x] == TYPE_AISLE_BEFORE)
            goal = 0;
      if(goal) {
        display();
        printf("  GAME CLEAR!\a\n");
        quit = 1;
      }
    }
  }
}
