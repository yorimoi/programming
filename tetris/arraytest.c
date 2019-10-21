#include <stdio.h>

void plus1(int _ary[5]) {
  for(int i=0; i<5; i++)
    _ary[i] = i;
}

int main() {
  int ary[5] = {};
  plus1(ary);
  for(int i=0; i<5; i++)
    printf("%d,", ary[i]);
}
