
#include "sort.h"

int main()
{
  init();

  // Gnome
  while(1) {
    int swap = 0;
    for(int i=0; i<ARRAY_MAX-1; i++) {
      if(array[i] < array[i+1]) {
        swap = 1;
        while(i>=0) {
          int tmp    = array[i];
          array[i]   = array[i+1];
          array[i+1] = tmp;
          printf("\033[H");
          for(int y=0; y<ARRAY_MAX; y++) {
            for(int x=0; x<array[y]; x++)
              printf(y==i?"\033[41m  ":"\033[48;5;255m  ");
            for(int x2=array[y]; x2<ARRAY_MAX-1; x2++)
              printf("\033[0m  ");
            printf("\033[0m\n");
          }
          if(array[i-1] < array[i])
            i--;
          else
            break;
        }
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

    if(!swap)
      break;
  }

  end();
}

