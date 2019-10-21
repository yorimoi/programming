/*http://www1.cts.ne.jp/~clab/hsample/Sort/Sort9.html */

#include "sort.h"

void draw(int pivot)
{
  printf("\033[H");
  for(int y=0; y<ARRAY_MAX; y++) {
    for(int x=0; x<array[y]; x++)
      printf(y==pivot?"\033[41m  ":"\033[48;5;255m  ");
    for(int x2=array[y]; x2<ARRAY_MAX-1; x2++)
      printf("\033[0m  ");
    printf("\033[0m\n");
  }
}

void quick(int left, int right) {
  int i, j, pivot;
  i = left;
  j = right;
  pivot = array[(left + right) / 2];
  while(1) {
    while(array[i] < pivot) i++;
    while(pivot < array[j]) j--;
    if(i >= j)
      break;
    int tmp  = array[i];
    array[i] = array[j];
    array[j] = tmp;
    i++;
    j--;
  }
  draw(pivot);
  if(left < i-1)
    quick(left, i-1);
  if(j+1 < right)
    quick(j+1, right);
}

int main()
{
  init();

  // Quick
  quick(0, ARRAY_MAX-1);

  end();
}

