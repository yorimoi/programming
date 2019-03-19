#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <termios.h>
#include <time.h>

typedef int bool;
#define true  1
#define false 0

unsigned int a=9,b=9,c=10;
unsigned int *FIELD_HEIGHT = &a;
unsigned int *FIELD_WIDTH  = &b;
unsigned int *MINE_COUNT   = &c;

typedef struct {
  bool tile;
  bool mine;
  bool flag;
  bool ques;
} CELL;
CELL **cells, *base_cells;

char getch()
{
  char buf=0;
  struct termios old={0};
  fflush(stdout);
  if(tcgetattr(0, &old)<0)
    perror("tcsetattr()");
  old.c_lflag&=~(ICANON|ECHO);
  old.c_cc[VMIN]=1;
  old.c_cc[VTIME]=0;
  if(tcsetattr(0, TCSANOW, &old)<0)
    perror("tcsetattr ICANON");
  if(read(0,&buf,1)<0)
    perror("read()");
  if(tcsetattr(0, TCSADRAIN, &old)<0)
    perror ("tcsetattr ~ICANON");
  return buf;
}

int getAdjacentBombsCount(unsigned int _x, unsigned int _y)
{
  int n = 0;
  for(int y=-1; y<=1; y++)
    for(int x=-1; x<=1; x++)
    {
      if(0==x && 0==y)
        continue;
      unsigned int x2 = _x + x;
      unsigned int y2 = _y + y;

      if( x2 >= *FIELD_WIDTH || y2 >= *FIELD_HEIGHT )
        continue;

      if(cells[y2][x2].mine) n++;
    }
  return n;
}

int getAdjacentFlagsCount(unsigned int _x, unsigned int _y)
{
  int n = 0;
  for(int y=-1; y<=1; y++)
    for(int x=-1; x<=1; x++)
    {
      if(0==x && 0==y)
        continue;
      unsigned int x2 = _x + x;
      unsigned int y2 = _y + y;

      if( x2 >= *FIELD_WIDTH || y2 >= *FIELD_HEIGHT )
        continue;

      if(cells[y2][x2].flag) n++;
    }
  return n;
}

void autoExpansion(unsigned int _x, unsigned int _y)
{
  if( cells[_y][_x].mine
      || !cells[_y][_x].tile || cells[_y][_x].flag
      || _x >= *FIELD_WIDTH  || _y >= *FIELD_HEIGHT
  )
    return;
  cells[_y][_x].tile = false;

  if( getAdjacentBombsCount(_x, _y) > 0 ) return;

  for(int y=-1; y<=1; y++)
  {
    for(int x=-1; x<=1; x++)
    {
      if( x==0 && y==0 ) continue;
      unsigned int x2 = _x + x;
      unsigned int y2 = _y + y;
      if( x2 >= *FIELD_WIDTH || y2 >= *FIELD_HEIGHT )
        continue;
      autoExpansion(x2, y2);
    }
  }
}

void firstArrangement(bool *_first, int _x, int _y, time_t *_t)
{
  srand((unsigned int)time(NULL));
  unsigned int n = 0;
  do {
    int x=rand() % *FIELD_WIDTH;
    int y=rand() % *FIELD_HEIGHT;
    {
      if( x>=_x-1 && x<=_x+1 && y>=_y-1 && y<=_y+1 )
        continue;
    }

    if( !cells[y][x].mine )
    {
      cells[y][x].mine = true;
      n++;
    }
  } while( n < *MINE_COUNT );
  *_first = false;
  *_t = time(NULL);
}

void checkClear(bool *_clear)
{
  *_clear = true;
  for(unsigned int y=0; y<*FIELD_HEIGHT; y++)
    for(unsigned int x=0; x<*FIELD_WIDTH; x++)
      if(!cells[y][x].mine && cells[y][x].tile )
        *_clear = false;
  if( *_clear )
  {
    printf("%c", 0x07);
  }
}

int checkField()
{
  if( (*FIELD_WIDTH * *FIELD_HEIGHT - 9) < *MINE_COUNT ) return 22;
  if( *FIELD_WIDTH * *FIELD_HEIGHT <= 9 )                return 22;
  if(  1 > *MINE_COUNT   || *MINE_COUNT>>16 )            return 22;
  if(  3 > *FIELD_WIDTH  || *FIELD_WIDTH>>8 )            return 22;
  if(  3 > *FIELD_HEIGHT || *FIELD_HEIGHT>>8 )           return 22;
  return 0;
}

void help(char * argv[]) {
	printf(
			"Simplistic Terminal Minesweeper\n"
      "default: -w 9 -h 9 -m 10\n"
			"Usage: %s [-w width] [-h height] [-m mines] [--help]\n"
			"  -w,--width    \033[3m<width>\033[0m\n"
			"  -h,--height   \033[3m<height>\033[0m\n"
			"  -m,--mine     \033[3m<mines>\033[0m\n"
			"  --help        \033[3mShow this help message.\033[0m\n"
      "Commands:\n"
      "  Move cursor:  Arrow keys, wasd or hjkl\n"
      "  Mark flag:    'f' key\n"
      "  Mark ?:       '?' key\n"
      "  Exit:         'q' key\n"
      "  Open tiles:   Other keys\n",
			argv[0]);
}

int main(int argc, char **argv)
{
  {
	  static struct option longopts[] = {
	  	{"width",  required_argument, 0, 'w'},
	  	{"height", required_argument, 0, 'h'},
	  	{"mine",   required_argument, 0, 'm'},
	  	{"help",   optional_argument, 0, 'H'},
	  	{0,0,0,0}
	  };
    int opt, idx;
    while( (opt=getopt_long(argc, argv, "w:h:m:", longopts, &idx)) != -1 )
    {
      switch(opt)
      {
        case 'w':
          *FIELD_WIDTH  = atoi(optarg);
          break;
        case 'h':
          *FIELD_HEIGHT = atoi(optarg);
          break;
        case 'm':
          *MINE_COUNT   = atoi(optarg);
          break;
        case 'H':
            help(argv);
            exit(0);
          break;
        default:
          help(argv);
          exit(1);
      }
    }
  }

  if( checkField() != 0 )
  {
    fprintf(stderr, "Invalid arguments\n");
    exit(22);
  }

  cells = malloc(sizeof(CELL *) * *FIELD_HEIGHT);
  base_cells = malloc(sizeof(CELL) * *FIELD_HEIGHT * *FIELD_WIDTH);
  for(unsigned int i=0; i<*FIELD_HEIGHT; i++)
    cells[i] = base_cells + i * *FIELD_WIDTH;

  unsigned int cursorX   = 0;
  unsigned int cursorY   = 0;
  unsigned int flag_cnt  = 0;
  time_t t               = 0;
  bool first             = true;
  bool explotion         = false;
  bool clear             = false;

  for(unsigned int y=0; y < *FIELD_HEIGHT; y++)
  {
    for(unsigned int x=0; x < *FIELD_WIDTH; x++)
    {
      cells[y][x].tile = true;
      cells[y][x].mine = false;
      cells[y][x].ques = false;
      cells[y][x].flag = false;
    }
  }

  printf("\033[H\033[2J\033[?25l");
  setvbuf(stdout, NULL, _IOFBF, *FIELD_WIDTH * *FIELD_HEIGHT * 10);

  while(1)
  {
    printf(" Remaining Mines: %03d  \n", *MINE_COUNT - flag_cnt);
    for(unsigned int y=0; y < *FIELD_HEIGHT; y++)
    {
      for(unsigned int x=0; x < *FIELD_WIDTH; x++)
        if(cursorX == x && cursorY == y)
          printf(" @");
        else if(cells[y][x].flag)
          printf(" \033[31;47mP\033[0m");
        else if(cells[y][x].ques)
          printf(" \033[37m?\033[0m");
        else if(cells[y][x].tile)
          printf(" \033[37m#\033[0m");
        else if(cells[y][x].mine)
          printf(" *");
        else
        {
          int n = getAdjacentBombsCount(x, y);
          if(n==0)
            printf("  ");
          else
          {
            printf(" \033[3%dm%d\033[0m", n, n);
          }
        }
      printf("\n");
    }
    fflush(stdout);

    if( explotion || clear )
    {
      if( clear )
      {
        time_t t2 = time(NULL);
        printf(" Congratulations!");
        printf(" [%d s]\n", (int)(t2 - t));
      }
      else
        printf("press any key...\n");
      if( getch() == 0x1b ) {getch();getch();}
      printf("\033[?25h");
      break;
    }

    switch(getch())
    {
      case '\033':
        {
          getch();
          switch(getch())
          {
            case 'A': if(cursorY > 0)                cursorY--; break;
            case 'B': if(cursorY < *FIELD_HEIGHT -1) cursorY++; break;
            case 'D': if(cursorX > 0)                cursorX--; break;
            case 'C': if(cursorX < *FIELD_WIDTH  -1) cursorX++; break;
          }
        }
        break;
      case 'k':
      case 'w': if(cursorY > 0)                cursorY--; break;
      case 'j':
      case 's': if(cursorY < *FIELD_HEIGHT -1) cursorY++; break;
      case 'h':
      case 'a': if(cursorX > 0)                cursorX--; break;
      case 'l':
      case 'd': if(cursorX < *FIELD_WIDTH  -1) cursorX++; break;
      case 'f': if( !first && cells[cursorY][cursorX].tile )
                {
                  if( !cells[cursorY][cursorX].flag )
                  {
                    cells[cursorY][cursorX].flag = true;
                    flag_cnt++;
                  }
                  else
                  {
                    cells[cursorY][cursorX].flag = false;
                    flag_cnt--;
                  }
                  cells[cursorY][cursorX].ques = false;
                }
                break;
      case '?': if( !first && cells[cursorY][cursorX].tile )
                {
                  cells[cursorY][cursorX].ques = !cells[cursorY][cursorX].ques;
                  if( cells[cursorY][cursorX].flag )
                  {
                    cells[cursorY][cursorX].flag = false;
                    flag_cnt--;
                  }
                }
                break;
      case 'q':
                printf("\033[?25h");
                exit(EXIT_SUCCESS);
                break;
      default:
                if( first )
                  firstArrangement(&first, cursorX, cursorY, &t);

                if( !cells[cursorY][cursorX].tile
                    && getAdjacentBombsCount(cursorX, cursorY)==0 )
                  break;

                if( !cells[cursorY][cursorX].flag
                 && !cells[cursorY][cursorX].ques )
                  cells[cursorY][cursorX].tile = false;
                else
                  break;

                if( cells[cursorY][cursorX].mine )
                {
                  explotion = true;
                  printf("%c", 0x07);
                  for(unsigned int y=0; y<*FIELD_HEIGHT; y++)
                    for(unsigned int x=0; x<*FIELD_WIDTH; x++)
                      cells[y][x].tile =
                      cells[y][x].ques =
                      cells[y][x].flag = false;
                  break;
                }

                if( (getAdjacentBombsCount(cursorX, cursorY)
                      == getAdjacentFlagsCount(cursorX, cursorY)) )
                {
                  for(int y=-1; y<=1; y++)
                  {
                    for(int x=-1; x<=1; x++)
                    {
                      if( x==0 && y==0 ) continue;
                      unsigned int x2 = cursorX + x;
                      unsigned int y2 = cursorY + y;
                      if( x2 >= *FIELD_WIDTH || y2 >= *FIELD_HEIGHT )
                        continue;
                      if( cells[y2][x2].mine && !cells[y2][x2].flag )
                      {
                        explotion = true;
                        printf("%c", 0x07);
                        for(unsigned int y=0; y<*FIELD_HEIGHT; y++)
                          for(unsigned int x=0; x<*FIELD_WIDTH; x++)
                            cells[y][x].tile =
                            cells[y][x].ques =
                            cells[y][x].flag = false;
                        break;
                      }
                      autoExpansion(x2, y2);
                    }
                  }
                }

                if( !explotion )
                  checkClear(&clear);

                break;
    }
    printf("\033[H");
  }

  return 0;
}
