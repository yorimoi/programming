
#include "sort.h"

int main()
{
  init();

  // Selection
  int start = 0, max, max_i;
  while(1) {
    max = -1;
    for(int i=start; i<ARRAY_MAX; i++) {
      if(max < array[i]) {
        max_i = i;
        max   = array[i];
      }
      printf("\033[H");
      for(int y=0; y<ARRAY_MAX; y++) {
        for(int x=0; x<array[y]; x++)
          printf(y==i?"\033[41m  ":y==max_i?"\033[44m  ":"\033[48;5;255m  ");
        for(int x2=array[y]; x2<ARRAY_MAX-1; x2++)
          printf("\033[0m  ");
        printf("\033[0m\n");
      }
    }
    int tmp      = array[start];
    array[start] = array[max_i];
    array[max_i] = tmp;
    if(++start==ARRAY_MAX)
      break;
  }

  end();
}

