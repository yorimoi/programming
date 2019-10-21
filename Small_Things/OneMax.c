
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GENOM_LENGTH 20          // 遺伝子情報の長さ
#define GENOM_LIST_MAX 20        // 遺伝子集団の大きさ
#define SLECT_GENOM 20           // 遺伝子選択数
#define INDIVIDUAL_MUTATION 0.1  // 個体突然変異確立
#define GENOM_MUTATION 0.1       // 遺伝子突然変異確立
#define GENERATION_MAX 100       // Max世代数

int list[GENOM_LIST_MAX][GENOM_LENGTH];

void create_genom(int length)
{
  for(int i=0; i<length; i++)
    for(int j=0; j<GENOM_LENGTH; j++)
      if(rand()%2)
        list[i][j] = 1;
}

float evaluation(int* list)
{
  float total = 0.0f;
  for(int i=0; i<GENOM_LENGTH-1; i++)
    total += *list++;
  return total / GENOM_LENGTH;
}

int main()
{
  srand((unsigned int)time(NULL));

  create_genom(GENOM_LIST_MAX);

  for(int i=0; i<GENOM_LIST_MAX; i++) {
    printf("%03.2f,", evaluation(&list[i][0]));
  }
}
