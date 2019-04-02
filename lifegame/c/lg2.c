#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FIELD_WIDTH  100
#define FIELD_HEIGHT 32


int field[FIELD_HEIGHT][FIELD_WIDTH];
int field_buffer[FIELD_HEIGHT][FIELD_WIDTH];

int check_neighbor(int _x, int _y)
{
  int cnt = 0;
  for(int y=-1; y<=1; y++)
    for(int x=-1; x<=1; x++)
      if(field[(_y+y)%FIELD_HEIGHT][(_x+x)%FIELD_WIDTH])
        cnt++;
  return cnt;
}

int main()
{
  srand((unsigned int)time(NULL));

  for(int y=0; y<FIELD_HEIGHT; y++)
    for(int x=0; x<FIELD_WIDTH; x++)
      if(rand()%2) field[y][x] = 1;

  printf("\033[2J");

  while(1) {
    printf("\033[H");
    for(int y=0; y<FIELD_HEIGHT; y++) {
      for(int x=0; x<FIELD_WIDTH; x++) {
        if(field[y][x])
          printf("\033[32m#\033[0m");
        else
          printf(".");
      }
      printf("\n");
    }

    for(int y=0; y<FIELD_HEIGHT; y++)
      for(int x=0; x<FIELD_WIDTH; x++) {
        if(field[y][x])
          switch(check_neighbor(x, y)) {
            case 2:
            case 3:
              // Do nothing
              break;
            default:
              field_buffer[y][x] = 0;
              break;
          }
        else
          if(check_neighbor(x, y) == 3)
            field_buffer[y][x] = 1;
      }
    for(int y=0; y<FIELD_HEIGHT; y++)
      for(int x=0; x<FIELD_WIDTH; x++)
        field[y][x] = field_buffer[y][x];
  }
}
