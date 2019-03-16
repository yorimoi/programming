/* パズドラを小一時間で作ってみた */
/* https://youtu.be/JLQLxrgygKk */
/* そのまんまパクっただけ */
/* vim:set foldmethod=marker: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>

#define FIELD_WIDTH  8
#define FIELD_HEIGHT 8

#define BLOCK_TYPE_MAX 7

typedef int bool;
#define true  1
#define false 0

enum {
  CELL_TYPE_NONE,
  CELL_TYPE_BLOCK_0,
  CELL_TYPE_BLOCK_1,
  CELL_TYPE_BLOCK_2,
  CELL_TYPE_BLOCK_3,
  CELL_TYPE_BLOCK_4,
  CELL_TYPE_BLOCK_5,
  CELL_TYPE_BLOCK_6,
  CELL_TYPE_MAX,
};

char CELL_AA[][6+1] =
{
  "\033[37m.", // CELL_TYPE_NONE
  "\033[31m0", // CELL_TYPE_BLOCK_0
  "\033[32m1", // CELL_TYPE_BLOCK_1
  "\033[33m2", // CELL_TYPE_BLOCK_2
  "\033[34m3", // CELL_TYPE_BLOCK_3
  "\033[35m4", // CELL_TYPE_BLOCK_4
  "\033[36m5", // CELL_TYPE_BLOCK_5
  "\033[37m6", // CELL_TYPE_BLOCK_6
};

int cells[FIELD_HEIGHT][FIELD_WIDTH];
int checked[FIELD_HEIGHT][FIELD_WIDTH];

int cursor_x, cursor_y;
int selected_x=-1, selected_y=-1;

bool locked = true;

int getch() // {{{
{
  char c;
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(0, TCSANOW, &t);
  c = getchar();
  tcsetattr(0, TCSADRAIN, &t);
  return c;
} // }}}

int kbhit() // {{{
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
} // }}}

int get_connected_block_count(int _x, int _y, int _type, int _cnt)
{
  if(
      ( _x<0 || _x>=FIELD_WIDTH || _y<0 || _y>=FIELD_HEIGHT )
      || checked[_y][_x]
      || cells[_y][_x] == CELL_TYPE_NONE
      || cells[_y][_x] != _type
    )
    return _cnt;
  _cnt++;
  checked[_y][_x] = true;

  _cnt = get_connected_block_count(_x, _y-1, _type, _cnt);
  _cnt = get_connected_block_count(_x-1, _y, _type, _cnt);
  _cnt = get_connected_block_count(_x, _y+1, _type, _cnt);
  _cnt = get_connected_block_count(_x+1, _y, _type, _cnt);

  return _cnt;
}

void erase_connected_blocks(int _x, int _y, int _type)
{
  if(
      ( _x<0 || _x>=FIELD_WIDTH || _y<0 || _y>=FIELD_HEIGHT )
      || cells[_y][_x] == CELL_TYPE_NONE
      || cells[_y][_x] != _type
    )
    return;

  cells[_y][_x] = CELL_TYPE_NONE;

  erase_connected_blocks(_x, _y-1, _type);
  erase_connected_blocks(_x-1, _y, _type);
  erase_connected_blocks(_x, _y+1, _type);
  erase_connected_blocks(_x+1, _y, _type);
}

void erase_connected_blocks_all()
{
  memset(checked, 0, sizeof checked);
  for(int y=0; y<FIELD_HEIGHT; y++)
    for(int x=0; x<FIELD_WIDTH; x++)
    {
      int n = get_connected_block_count(x, y, cells[y][x], 0);
      if(n>=3)
      {
        erase_connected_blocks(x, y, cells[y][x]);
        locked = true;
      }
    }
}

void display()
{
  printf("\033[H");
  for(int y=0; y<FIELD_HEIGHT; y++)
  {
    for(int x=0; x<FIELD_WIDTH; x++)
    {
      if(x==cursor_x && y==cursor_y && !locked)
        printf(" \033[47m%s\033[0m", CELL_AA[cells[y][x]]);
      else
        printf(" %s", CELL_AA[cells[y][x]]);
    }
    printf(" %s\n", (y==selected_y)?"\033[37m←":" ");
  }
  for(int x=0; x<FIELD_WIDTH; x++)
    printf(" %s", (x==selected_x)?"\033[37m↑":" ");
  printf("\n");
}

int main()
{
  int quit_flag = 0;
  srand((unsigned int)time(NULL));

  for(int y=0; y<FIELD_HEIGHT; y++)
    for(int x=0; x<FIELD_WIDTH; x++)
      cells[y][x] = CELL_TYPE_BLOCK_0 + rand() % BLOCK_TYPE_MAX;

  time_t t = time(NULL);

  while(locked)
  {
    locked = false;
    for(int y=FIELD_HEIGHT-2; y>=0; y--)
      for(int x=0; x<FIELD_WIDTH; x++)
        if((cells[y][x] != CELL_TYPE_NONE
              && (cells[y+1][x] == CELL_TYPE_NONE)))
        {
          cells[y+1][x] = cells[y][x];
          cells[y][x]   = CELL_TYPE_NONE;
          locked = true;
        }

    for(int x=0; x<FIELD_WIDTH; x++)
      if(cells[0][x] == CELL_TYPE_NONE)
      {
        cells[0][x] = CELL_TYPE_BLOCK_0 + rand() % BLOCK_TYPE_MAX;
        locked = true;
      }

    if(!locked)
      erase_connected_blocks_all();
  }

  printf("\033[2J\033[H\033[?25l");

  while(!quit_flag)
  {
    if(t < time(NULL))
    {
      t = time(NULL);

      if(locked)
      {
        locked = false;
        for(int y=FIELD_HEIGHT-2; y>=0; y--)
          for(int x=0; x<FIELD_WIDTH; x++)
            if((cells[y][x] != CELL_TYPE_NONE
                  && (cells[y+1][x] == CELL_TYPE_NONE)))
            {
              cells[y+1][x] = cells[y][x];
              cells[y][x]   = CELL_TYPE_NONE;
              locked = true;
            }

        for(int x=0; x<FIELD_WIDTH; x++)
          if(cells[0][x] == CELL_TYPE_NONE)
          {
            cells[0][x] = CELL_TYPE_BLOCK_0 + rand() % BLOCK_TYPE_MAX;
            locked = true;
          }

        if(!locked)
          erase_connected_blocks_all();
      }

      display();
    }

    if(kbhit())
    {
      if(locked)
        getch();
      else
      {
        switch(getch())
        {
          case 'h': if(cursor_x>0)              cursor_x--; break;
          case 'j': if(cursor_y<FIELD_HEIGHT-1) cursor_y++; break;
          case 'k': if(cursor_y>0)              cursor_y--; break;
          case 'l': if(cursor_x<FIELD_WIDTH-1)  cursor_x++; break;
          case 'q': quit_flag = 1; break;
          default:
                    if(selected_x < 0)
                    {
                      selected_x = cursor_x;
                      selected_y = cursor_y;
                    }
                    else
                    {
                      int distance = abs(selected_x - cursor_x)
                                   + abs(selected_y - cursor_y);
                      if(distance == 0)
                        selected_x = selected_y = -1;
                      else if(distance == 1)
                      {
                        int tmp = cells[cursor_y][cursor_x];
                        cells[cursor_y][cursor_x] = cells[selected_y][selected_x];
                        cells[selected_y][selected_x] = tmp;

                        erase_connected_blocks_all();

                        selected_x = selected_y = -1;
                        locked = true;
                      }
                      else
                        printf("\a");
                    }
                    break;
        }
      }
      display();
    }
  }

  printf("\033[?25h");

  return 0;
}
