
#include "sort.h"

void draw(int n)
{
  printf("\033[H");
  for(int y=0; y<ARRAY_MAX; y++) {
    for(int x=0; x<array[y]; x++)
      printf(y==n?"\033[41m  ":"\033[48;5;255m  ");
    for(int x2=array[y]; x2<ARRAY_MAX-1; x2++)
      printf("\033[0m  ");
    printf("\033[0m\n");
  }
}

int main()
{
  init();

  // Insertion
  int i, j;
  for(j=i=1; i<ARRAY_MAX; i++, j=i) {
    draw(i);
    while((j > 0) && (array[j-1] < array[j])) {
      int tmp    = array[j];
      array[j]   = array[j-1];
      array[j-1] = tmp;
      draw(j--);
    }
  }

  end();
}

