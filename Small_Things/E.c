/* 【東大入試】高学歴ガチ数学バトル！超早解き対決！【激ムズ】 */
/* https://youtu.be/dZL_eYv-T8Q */

/* 1==2<3 */
#include <stdio.h>

#define LOOP 1000000

int main()
{
  unsigned int i;
  for(int j=0; j<LOOP; j++) {
    i = 1;
    while(1) {
      //if(i%3 == 1)if(i%5 == 1)if(i%7 == 4)if(i%11 == 6) break; // 1
      if((i%3==1)&&(i%5==1)&&(i%7==4)&&(i%11==6)) break;       // 2
      //if((i%11==6)&&(i%7==4)&&(i%5==1)&&(i%3==1)) break;       // 3

      i++;
    }
  }
  printf("%d回ループ, %d\n", LOOP, i);
}
