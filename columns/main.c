#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>

#define FIELD_WIDTH       8
#define FIELD_HEIGHT      17
#define COLUMN_MAX        3
#define COLUMNS_DEFAULT_X 4
#define COLUMNS_DEFAULT_Y 1
#define JEWEL_MAX         6

enum {
  CELL_NONE,
  CELL_WALL,
  CELL_JEWEL0,
  CELL_JEWEL1,
  CELL_JEWEL2,
  CELL_JEWEL3,
  CELL_JEWEL4,
  CELL_JEWEL5,
  CELL_MAX
};

char cell_AA[][5+3+1] =
{
  "  ",         // CELL_NONE
  "\033[37m #", // CELL_WALL
  "\033[31m〇", // CELL_JEWEL0
  "\033[32m〇", // CELL_JEWEL1
  "\033[33m〇", // CELL_JEWEL2
  "\033[34m〇", // CELL_JEWEL3
  "\033[35m〇", // CELL_JEWEL4
  "\033[36m〇", // CELL_JEWEL5
};

int  field_cells[FIELD_HEIGHT][FIELD_WIDTH];
int  display_buffer[FIELD_HEIGHT][FIELD_WIDTH];
int  columns[COLUMN_MAX];
int  columns_x, columns_y;
int  pass;

void reset_columns()
{
  columns_x = COLUMNS_DEFAULT_X;
  columns_y = COLUMNS_DEFAULT_Y;
  for(int i=0; i<COLUMN_MAX; i++)
    columns[i] = CELL_JEWEL0 + rand() % JEWEL_MAX;
}

int getch() /*{{{*/
{
  char c;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  c = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return c;
}//}}}
int kbhit()/*{{{*/
{
  int c, of;
  struct termios ot, nt;
  tcgetattr(0, &ot);
  nt = ot;
  nt.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &nt);
  of = fcntl(0, F_GETFL, 0);
  fcntl(0, F_SETFL, of | O_NONBLOCK);
  c = getchar();
  tcsetattr(0, TCSANOW, &ot);
  fcntl(0, F_SETFL, of);
  if (c != EOF) {
    ungetc(c, stdin);
    return 1;
  }
  return 0;
}//}}}

void display()
{
  printf("\033[H");
  memcpy(display_buffer, field_cells, sizeof field_cells);

  if(!pass)
    for(int i=0; i<COLUMN_MAX; i++)
      display_buffer[columns_y+i][columns_x] = columns[i];

  for(int y=2; y<FIELD_HEIGHT; y++) {
    for(int x=0; x<FIELD_WIDTH; x++)
      printf("%s", cell_AA[display_buffer[y][x]]);
    printf("\n");
  }
}

int columns_collision_detection(int _columns_x, int _columns_y)
{
  return field_cells[_columns_y+COLUMN_MAX-1][_columns_x] != CELL_NONE;
}

int erase()
{
  int erased = 0;
  for(int y=1; y<FIELD_HEIGHT-1; y++) {
    for(int x=1; x<FIELD_WIDTH-1; x++) {
      const int direction_max = 4;
      int directions[][2] = {
        { 1, 0},
        { 0, 1},
        { 1, 1},
        { 1,-1},
      };
      for(int i=0; i<direction_max; i++) {
        int connect_cnt = 0;
        int cell = field_cells[y][x];
        for(int y2=y, x2=x; field_cells[y2][x2] == cell;
            x2 += directions[i][0], y2 += directions[i][1])
          connect_cnt++;
        if(connect_cnt >= 3) {
          erased = 1;
          for(int y2=y, x2=x; field_cells[y2][x2] == cell;
              x2 += directions[i][0], y2 += directions[i][1])
            field_cells[y2][x2] = CELL_NONE;
        }
      }
    }
  }
  return erased;
}

void quit()
{
  printf("\033[?25h");
  exit(0);
}

int main()
{
  srand((unsigned int)time(NULL));
  time_t t = 0;
  for(int y=0; y<FIELD_HEIGHT; y++)
    field_cells[y][0] = field_cells[y][FIELD_WIDTH-1] = CELL_WALL;
  for(int x=1; x<FIELD_WIDTH-1; x++)
    field_cells[0][x] = field_cells[FIELD_HEIGHT-1][x] = CELL_WALL;
  reset_columns();

  printf("\033[2J\033[H\033[?25l");

  display();

  while(1) {
    if(time(NULL) != t) {
      t = time(NULL);

      if(pass) {
        int fall = 0;
        for(int y=FIELD_HEIGHT-2; y>=1; y--)
          for(int x=1; x<FIELD_WIDTH-1; x++) {
            if((field_cells[y+1][x] == CELL_NONE)
                && (field_cells[y][x] != CELL_NONE)) {
              fall = 1;
              field_cells[y+1][x] = field_cells[y][x];
              field_cells[y][x] = CELL_NONE;
            }
          }
        if(!fall) {
          pass = 0;
          reset_columns();
        }
      }
      else if(columns_collision_detection(columns_x, columns_y+1)) {
        for(int i=0; i<COLUMN_MAX; i++)
          field_cells[columns_y+i][columns_x] = columns[i];
        for(int y=1; y<3; y++)
          for(int x=1; x<FIELD_WIDTH-1; x++)
            if(field_cells[y][x] != CELL_NONE) {
              printf("GAME OVER!\n");
              getch();
              quit();
            }
        if(erase())
          pass = 1;
      } else {
        columns_y++;
      }

      display();
    }

    if(kbhit()) {
      if(pass) {
        if('q'==getch())
          quit();
      } else {
        switch(getch()) {
          case 'q': quit(); break;
          case 'h':
            if(!columns_collision_detection(columns_x-1, columns_y))
              columns_x--;
            break;
          case 'j':
            if(!columns_collision_detection(columns_x, columns_y+1))
              columns_y++;
            break;
          //case 'k': columns_y--; break;
          case 'l':
            if(!columns_collision_detection(columns_x+1, columns_y))
              columns_x++;
            break;
          case ' ':
            {
              int tmp[COLUMN_MAX];
              memcpy(tmp, columns, sizeof columns);
              for(int i=0; i<COLUMN_MAX; i++)
                columns[(i+1)%COLUMN_MAX] = tmp[i];
            }
            break;
        }
        display();
      }
    }
  }
}
