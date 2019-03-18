#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
  uint8_t buf[4096];
  ssize_t nread;
  int fd  = 0;
  int ret = 0;

  for(int i=1; i<argc; i++) {
    if(argc > 1) {
      fd = open(argv[i], O_RDONLY);
      if(fd < 0) {
        ret = errno;
        perror("open");
        continue;
      }
    }

    while((nread = read(fd, buf, sizeof(buf))) > 0)
      write(1, buf, nread);
  }

  return ret;
}
