#include "sort.h"

int main()
{
  init();

  // Comb
  int h = ARRAY_MAX / 1.3;
  while(1) {
    for(int i=0; i+h<=ARRAY_MAX; i++) {
      if(array[i] < array[i+h]) {
        int tmp    = array[i];
        array[i]   = array[i+h];
        array[i+h] = tmp;
      }
      printf("\033[H");
      for(int y=0; y<ARRAY_MAX; y++) {
        for(int x=0; x<array[y]; x++)
          printf(y==i?"\033[41m  ":"\033[48;5;255m  ");
        for(int x2=array[y]; x2<ARRAY_MAX-1; x2++)
          printf("\033[0m  ");
        printf("\033[0m\n");
      }
    }

    if(h==1)
      break;
    h /= 1.3;
  }

  end();
}
