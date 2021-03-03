#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define FILE_SIZE 1024

int main(void) {
    int fd;
    char *map;
    char c = 0;
    long page_size, map_size;
    char *filename = "mmap_output_file.txt";

    fd = open(filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd < 0) {
        printf("error: can't open file: %s\n", filename);
        return 1;
    }

    page_size = sysconf(_SC_PAGE_SIZE);
    map_size  = (FILE_SIZE / page_size + 1) * page_size;

    lseek(fd, map_size, SEEK_SET);
    write(fd, &c, sizeof(char));
    lseek(fd, 0, SEEK_SET);

    map = (char*) mmap(NULL, map_size, PROT_WRITE, MAP_SHARED, fd, 0);

    if (map == MAP_FAILED) {
        printf("error: mmap failed\n");
        return 1;
    }

    strcat(map, "This is test file for using mmap.\n");
    strcat(map, "mmap, is useful for read/write file.\n");

    msync(map, map_size, 0);

    close(fd);
    munmap(map, map_size);

    return 0;
}

