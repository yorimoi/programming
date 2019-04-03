/* 3歳娘「パパ、関数をカリー化して？」 */
/* https://qiita.com/jzmstrjp/items/99cc1c8ebcfc703b1410 */

#include <stdio.h>

typedef int(*funcp)(int);
int add_1;

int add(int hikisu2) {
  return add_1 + hikisu2;
}

funcp tashizann(int hikisu1) {
  add_1 = hikisu1;
  return add;
}

int main()
{
  const int kekka   = tashizann(3)(5); // 普通のカリー化
  const funcp tasu3 = tashizann(3);    // 部分適用
  const int kekka2  = tasu3(5);

  printf("tashizann(3)(5): %d\n", kekka);
  printf("tasu3(5): %d\n", kekka2);
}
