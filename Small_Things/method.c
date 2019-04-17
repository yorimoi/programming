#include <stdio.h>

typedef struct {
  int val;
  int (*tasu3)();
} method;
method num;

int add3() { return num.val + 3; }

int main() {
  num.val = 5;
  num.tasu3 = add3;
  printf("%d\n", num.tasu3());
}
