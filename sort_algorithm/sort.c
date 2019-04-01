#include "sort.h"

void init()
{
  srand((unsigned int)time(NULL));

  for(int i=0; i<ARRAY_MAX-1; i++)
    array[i] = i;

  for(int i=0; i<ARRAY_MAX; i++) {
    int j    = rand() % ARRAY_MAX;
    int tmp  = array[i];
    array[i] = array[j];
    array[j] = tmp;
  }

  printf("\033[2J[\033[H\033[?25l");

  for(int i=0; i<ARRAY_MAX; i++) {
    for(int j=0; j<array[i]; j++)
      printf("\033[48;5;255m  ");
    printf("\n");
  }

  printf("\033[0m\npress enter\n");
  getchar();
}

void end()
{
  printf("\033[H");
  for(int i=0; i<ARRAY_MAX; i++) {
    for(int j=0; j<array[i]; j++)
      printf("\033[48;5;255m  ");
    printf("\n");
  }

  printf("\033[0m\033[?25h");
}
