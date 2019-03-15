#include "lce.h"

int t_init()
{
  if( tcgetattr(STDIN_FILENO, &oldt) < 0 )
    return -1;
  newt = oldt;
  newt.c_lflag &= ~(ICANON|ECHO);
  newt.c_cc[VMIN]=1;
  newt.c_cc[VTIME]=0;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  return 0;
}

int t_kbhit()
{
  int _c, oldf;
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  _c = getchar();
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if (_c != EOF) {
    ungetc(_c, stdin);
    return 1;
  }
  return 0;
}

int kbhit()
{
  struct termios oldt, newt;
  int _c, oldf;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  _c = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if (_c != EOF) {
    ungetc(_c, stdin);
    return 1;
  }
  return 0;
}

int t_getch()
{
  char _c=0;
  fflush(stdout);
  if(read(0,&_c,1)<0)
    perror("read()");
  return _c;
}

int getch()
{
  char _c=0;
  struct termios old={0};
  fflush(stdout);
  if(tcgetattr(0, &old)<0)
    perror("tcsetattr()");
  old.c_lflag&=~(ICANON|ECHO);
  old.c_cc[VMIN]=1;
  old.c_cc[VTIME]=0;
  if(tcsetattr(0, TCSANOW, &old)<0)
    perror("tcsetattr ICANON");
  if(read(0,&_c,1)<0)
    perror("read()");
  if(tcsetattr(0, TCSADRAIN, &old)<0)
    perror ("tcsetattr ~ICANON");
  return _c;
}

void cursorPos(int _n, int _m)
{
  printf("\033[%d;%dH", _n + 1, _m * 2 + 1);
}

void t_reset()
{
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  //clearScreen();
  resetColor();
  cursorOn();
  //printf("\n");
}

void reset()
{
  //clearScreen();
  resetColor();
  cursorOn();
  //printf("\n");
}
