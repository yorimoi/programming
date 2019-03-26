#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#define COURSE_WIDTH  224
#define COURSE_HEIGHT  15

#define SCREEN_WIDTH  16

enum {
  PART_NONE,
  PART_GROUND,
  PART_HARD_BLOCK,
  PART_SOFT_BLOCK,
  PART_PIPE_UPPER_LEFT,
  PART_PIPE_UPPER_RIGHT,
  PART_PIPE_LOWER_LEFT,
  PART_PIPE_LOWER_RIGHT,
  PART_QUESTION0,
  PART_QUESTION1,
  PART_QUESTION2,
  //PART_QUESTION_USED,
  PART_CLOUD_UPPER_LEFT,
  PART_CLOUD_UPPER_CENTER,
  PART_CLOUD_UPPER_RIGHT,
  PART_CLOUD_LOWER_LEFT,
  PART_CLOUD_LOWER_CENTER,
  PART_CLOUD_LOWER_RIGHT,
  PART_TREE_LEFT,
  PART_TREE_CENTER,
  PART_TREE_RIGHT,
  PART_MOUNTAIN_TOP,
  PART_MOUNTAIN_LEFT,
  PART_MOUNTAIN_CENTER0,
  PART_MOUNTAIN_RIGHT,
  PART_MOUNTAIN_PLANE,
  PART_MOUNTAIN_CNETER1,
  PART_GOAL_TOP,
  PART_GOAL_POLE,
  PART_FORT_TOP,
  PART_FORT_LEFT,
  PART_FORT_WALL,
  PART_FORT_RIGHT,
  PART_FORT_MIDDLE,
  PART_FORT_GATE_UPPER,
  PART_FORT_GATE_LOWER,
  PART_MAX,
};

char part_names[][2+1] =
{
  "  ",
  "b0",
  "b1",
  "b2",
  "p0",
  "p1",
  "p2",
  "p3",
  "q0",
  "q1",
  "q2",
  "c0",
  "c1",
  "c2",
  "c3",
  "c4",
  "c5",
  "t0",
  "t1",
  "t2",
  "m0",
  "m1",
  "m2",
  "m3",
  "m4",
  "m5",
  "g0",
  "g1",
  "f0",
  "f1",
  "f2",
  "f3",
  "f4",
  "f5",
  "f6"
};

char part_AA[][36+4+1] =
{
  "\033[38;2;000;000;000;48;2;080;128;255m  ", // PART_NONE
  "\033[38;2;000;000;000;48;2;192;112;000m##", // PART_GROUND
  "\033[38;2;192;112;000;48;2;255;200;184m■", // PART_HARD_BLOCK
  "\033[38;2;000;000;000;48;2;192;112;000m♯", // PART_SOFT_BLOCK
  "\033[38;2;000;000;000;48;2;072;168;016m[ ", // PART_PIPE_UPPER_LEFT
  "\033[38;2;000;000;000;48;2;072;168;016m ]", // PART_PIPE_UPPER_RIGHT
  "\033[38;2;152;232;000;48;2;072;168;016m| ", // PART_PIPE_LOWER_LEFT
  "\033[38;2;152;232;000;48;2;072;168;016m |", // PART_PIPE_LOWER_RIGHT
  "\033[38;2;000;000;000;48;2;255;160;000m？", // PART_QUESTION0
  "\033[38;2;000;000;000;48;2;255;160;000m？", // PART_QUESTION1
  "\033[38;2;000;000;000;48;2;255;160;000m？", // PART_QUESTION2
//"\033[38;2;000;000;000;48;2;255;160;000m  ", // PART_QUESTION_USED
  "\033[38;2;096;200;255;48;2;255;255;255m% ", // PART_CLOUD_UPPER_LEFT
  "\033[38;2;096;160;255;48;2;255;255;255m％", // PART_CLOUD_UPPER_CENTER
  "\033[38;2;096;200;255;48;2;255;255;255m %", // PART_CLOUD_UPPER_RIGHT
  "\033[38;2;096;200;255;48;2;255;255;255m% ", // PART_CLOUD_LOWER_LEFT
  "\033[38;2;096;160;255;48;2;255;255;255m％", // PART_CLOUD_LOWER_CENTER
  "\033[38;2;096;200;255;48;2;255;255;255m %", // PART_CLOUD_LOWER_RIGHT
  "\033[38;2;000;000;000;48;2;152;232;000m  ", // PART_TREE_LEFT
  "\033[38;2;000;000;000;48;2;152;232;000m %", // PART_TREE_CENTER
  "\033[38;2;000;000;000;48;2;152;232;000m  ", // PART_TREE_RIGHT
  "\033[38;2;000;000;000;48;2;072;168;016m  ", // PART_MOUNTAIN_TOP
  "\033[38;2;000;000;000;48;2;072;168;016m  ", // PART_MOUNTAIN_LEFT
  "\033[38;2;000;000;000;48;2;072;168;016m %", // PART_MOUNTAIN_CENTER0
  "\033[38;2;000;000;000;48;2;072;168;016m  ", // PART_MOUNTAIN_RIGHT
  "\033[38;2;000;000;000;48;2;072;168;016m  ", // PART_MOUNTAIN_PLANE
  "\033[38;2;000;000;000;48;2;072;168;016m% ", // PART_MOUNTAIN_CNETER1
  "\033[38;2;072;168;016;48;2;080;128;255m●", // PART_GOAL_TOP
  "\033[38;2;152;232;000;48;2;080;128;255m｜", // PART_GOAL_POLE
  "\033[38;2;000;000;000;48;2;192;112;000mΠ", // PART_FORT_TOP
  "\033[38;2;000;000;000;48;2;192;112;000m#▮", // PART_FORT_LEFT
  "\033[38;2;000;000;000;48;2;192;112;000m♯", // PART_FORT_WALL
  "\033[38;2;000;000;000;48;2;192;112;000m▮#", // PART_FORT_RIGHT
  "\033[38;2;000;000;000;48;2;192;112;000m##", // PART_FORT_MIDDLE
  "\033[38;2;192;112;000;48;2;000;000;000m⌒", // PART_FORT_GATE_UPPER
  "\033[38;2;000;000;000;48;2;000;000;000m  ", // PART_FORT_GATE_LOWER
};

int course[COURSE_HEIGHT][COURSE_WIDTH];

int player_x, player_y;

int getch()
{
  char ch;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  ch = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return ch;
}

// Collision Detection
int cd(int px, int py)
{
  if(px<0 || px>=COURSE_WIDTH || py<0 || py>=COURSE_HEIGHT)
    return 1;

  switch(course[py][px]) {
    case PART_GROUND:
    case PART_HARD_BLOCK:
    case PART_SOFT_BLOCK:
    case PART_PIPE_UPPER_LEFT:
    case PART_PIPE_UPPER_RIGHT:
    case PART_PIPE_LOWER_LEFT:
    case PART_PIPE_LOWER_RIGHT:
    case PART_QUESTION0:
    case PART_QUESTION1:
    case PART_QUESTION2:
      return 1;
  }
  return 0;
}

void draw()
{
  printf("\033[H");

  int left = player_x - SCREEN_WIDTH/2;
  if(left < 0)
    left = 0;
  if(left > COURSE_WIDTH - SCREEN_WIDTH)
    left = COURSE_WIDTH - SCREEN_WIDTH;

  for(int y=0; y<COURSE_HEIGHT; y++) {
    for(int x=left; x<left+SCREEN_WIDTH; x++) {
      printf("%s", part_AA[course[y][x]]);
      if(x==player_x && y==player_y)
        printf("\b\b\033[31m＠");
    }
    printf("\n");
  }
}

void quit()
{
  printf("\033[?25h");
  exit(0);
}

int main()
{
  FILE *fp;
  char filename[] = "1-1.txt";

  fp = fopen(filename, "r");
  if(fp == NULL)
    printf("%s open failured!\a\n", filename);

  for(int y=0; y<COURSE_HEIGHT; y++) {
    for(int x=0; x<COURSE_WIDTH; x++) {
      char part[2+1];
      fread(part, 2, 1, fp);
      for(int p=PART_NONE; p<PART_MAX; p++) {
        if(strncmp(part, part_names[p], 2) == 0) {
          course[y][x] = p;
          break;
        }
      }
    }
    fseek(fp, 2, SEEK_CUR);
  }
  fclose(fp);

  player_x = 1;
  player_y = 12;

  printf("\033[2J\033[H\033[?25l");

  while(1) {
    draw();

    switch(getch()) {
      case 'h': if(!cd(player_x-1, player_y)) player_x--; break;
      case 'j': if(!cd(player_x, player_y+1)) player_y++; break;
      case 'k': if(!cd(player_x, player_y-1)) player_y--; break;
      case 'l': if(!cd(player_x+1, player_y)) player_x++; break;
      case 'q': quit(); break;
    }
  }
}
