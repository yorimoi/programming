/* 【東大入試】高学歴ガチ数学バトル！超早解き対決！【激ムズ】 */
/* https://youtu.be/dZL_eYv-T8Q */
#include <stdio.h>

int main()
{
  unsigned int i = 1;
  while(1) {
    if(i%3 == 1)
      if(i%5 == 1)
        if(i%7 == 4)
          if(i%11 == 6)
            break;
    i++;
  }
  printf("[%d]\n", i);
}
