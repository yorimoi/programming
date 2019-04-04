/* For me */
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
  if(argc < 2) {
    printf("Invalid argument\n");
    return 1;
  }

  char *on  = "on";
  char *off = "off";

  if(strcmp(on, argv[1])==0)
    printf("\033[?25h");
  else if(strcmp(off, argv[1])==0)
    printf("\033[?25l");
  else {
    printf("Invalid argument\n");
    return 1;
  }

  return 0;
}
