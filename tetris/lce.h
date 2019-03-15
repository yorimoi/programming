///* Require ----------
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
/* ------------------ */

#define loop for(;;)

typedef int bool;
#define true  1
#define false 0

#define clearScreen() printf("\033[H\033[2J\033[?25l");
#define cursorOn()    printf("\033[?25h");
#define cursorOff()   printf("\033[?25l");

#define resetColor()  printf("\033[0m");
#define BLACK         "\033[38;5;0m"
#define RED           "\033[38;5;1m"
#define GREEN         "\033[38;5;2m"
#define YELLOW        "\033[38;5;3m"
#define BLUE          "\033[38;5;4m"
#define MAGENTA       "\033[38;5;5m"
#define CYAN          "\033[38;5;6m"
#define WHITE         "\033[38;5;7m"

struct termios oldt, newt;
extern struct termios oldt, newt;
int t_init();

int t_kbhit();
int kbhit();

int t_getch();
int getch();

void cursorPos(int _n, int _m);

void t_reset();
void reset();
