#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_MAX 32

int  array[ARRAY_MAX];

void init();
void end();

extern int  array[];
extern void init();
extern void end();
