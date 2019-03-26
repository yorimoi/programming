/* https://dic.nicovideo.jp/a/%E5%80%89%E5%BA%AB%E7%95%AA */
#include <stdio.h>
#include <string.h>
#include <termios.h>

#define FIELD_WIDTH  8
#define FIELD_HEIGHT 7

enum {
  NONE,
  WALL,
  BAGGAGE,
  GOAL,
  BAGGAGE_ON_THE_GOAL
};

char AA[][5+3+1] =
{
  "\033[0m  ",  // NONE
  "\033[0m■",  // WALL
  "\033[33m○", // BAGGAGE
  "\033[31m＋", // GOAL
  "\033[31m◎", // BAGGAGE_ON_THE_GOAL
};

int field[FIELD_HEIGHT][FIELD_WIDTH] =
{
  {1,1,1,1,1,1,1,1},
  {1,0,0,1,1,1,3,1},
  {1,0,2,0,0,1,3,1},
  {1,0,2,0,0,0,3,1},
  {1,1,2,0,0,2,3,1},
  {1,1,0,0,0,1,1,1},
  {1,1,1,1,1,1,1,1},
};
int field_buffer[FIELD_HEIGHT][FIELD_WIDTH];

int player_x, player_y;

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
        printf("\033[0m♀");
      else
        printf("%s", AA[field_buffer[y][x]]);
    }
    printf("\n");
  }
}

int main()
{
  player_x = player_y = 1;
  memcpy(field_buffer, field, sizeof field);

  printf("\033[2J\033[H");

  while(1) {
    display();

    int px = player_x;
    int py = player_y;
    switch(getch()) {
      case 'r':
        printf("\a");
        memcpy(field_buffer, field, sizeof field);
        player_x = player_y = 1;
        continue;
      case 'h': px--; break;
      case 'j': py++; break;
      case 'k': py--; break;
      case 'l': px++; break;
      case 'q': return 0;
    }
    if(field[py][px] != WALL) {
      if((field_buffer[py][px] == BAGGAGE)
          || field_buffer[py][px] == BAGGAGE_ON_THE_GOAL) {
        int bx = px + -1 * (player_x - px);
        int by = py + -1 * (player_y - py);
        if((field[by][bx] != WALL)
            && (field_buffer[by][bx] != BAGGAGE)
            && (field_buffer[by][bx] != BAGGAGE_ON_THE_GOAL)) {
          field_buffer[by][bx] = field_buffer[py][px];
          if(field[py][px] == GOAL)
            field_buffer[py][px] = GOAL;
          else
            field_buffer[py][px] = NONE;
          player_x = px;
          player_y = py;
        }
        if(field[by][bx] == GOAL)
          field_buffer[by][bx] = BAGGAGE_ON_THE_GOAL;

        int finish = 0;
        for(int y=0; y<FIELD_HEIGHT; y++)
          for(int x=0; x<FIELD_WIDTH; x++)
            if((field[y][x] == GOAL)
                && (field_buffer[y][x] == BAGGAGE_ON_THE_GOAL))
              finish++;
        if(finish == 4) {
          display();
          printf("  GAME CLEAR!!\a\n");
          return 0;
        }
      } else {
        player_x = px;
        player_y = py;
      }
    }
  }
}
