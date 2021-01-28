#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define FILE_SIZE 1024

int main(void) {
    int fd;
    char *map;
    long page_size, map_size;
    char *filename = "mmap_input_file.txt";

    fd = open(filename, O_RDONLY);

    if (fd < 0) {
        printf("error: can't open file: %s\n", filename);
        return 1;
    }

    page_size = sysconf(_SC_PAGE_SIZE);
    map_size  = (FILE_SIZE / page_size + 1) * page_size;

    map = (char*) mmap(NULL, map_size, PROT_READ, MAP_SHARED, fd, 0);

    if (map == MAP_FAILED) {
        printf("error: mmap failed\n");
        return 1;
    }

    printf("%s\n", map);

    close(fd);
    munmap(map, map_size);

    return 0;
}

