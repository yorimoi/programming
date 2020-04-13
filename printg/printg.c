#include "printg.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

void size_check(char* str, int* str_size, int* cur);
void toint(char* str, int* str_size, int* cur, int num);

int vprintg(int fd, const char* fmt, va_list ap) {
    int cur = 0, str_size = 8;
    char *c = (char *) fmt;
    char *str = (char *) malloc(str_size * sizeof(char));

    if (str == NULL) {
        exit(1);
    }

    while (*c != '\0') {
        if (*c == '%') {
            switch (c[1]) {
                case 'd':
                    toint(str, &str_size, &cur, va_arg(ap, int));
                    c += 2;
                    break;

                default: break;
            }
        } else {
            size_check(str, &str_size, &cur);
            str[cur++] = *c++;
        }
    }

    str[cur] = '\0';
    write(fd, str, cur);

    free(str);

    return cur;
}

int fprintg(int fd, const char* fmt, ...) {
    int len;
    va_list ap;

    va_start(ap, fmt);
    len = vprintg(fd, fmt, ap);
    va_end(ap);

    return len;
}

int printg(const char* fmt, ...) {
    int len;
    va_list ap;

    va_start(ap, fmt);
    len = vprintg(0, fmt, ap);
    va_end(ap);

    return len;
}

void size_check(char* str, int* str_size, int* cur) {
    if (*str_size <= (*cur)+1) {
        char *str_new = realloc(str, *str_size * 2 * sizeof(char));
        if (str_new == NULL) {
            free(str);
            exit(1);
        }

        str = str_new;
        *str_size *= 2;
    }
}

void toint(char* str, int* str_size, int* cur, int num) {
    if (num == 0) {
        size_check(str, str_size, cur);
        str[(*cur)++] = '0';
        return;
    }

    char buf[10];
    int buf_cur = 0;

    while (num != 0) {
        buf[buf_cur++] = num % 10 + '0';
        num /= 10;
    }
    while (0 < buf_cur--) {
        size_check(str, str_size, cur);
        str[(*cur)++] = buf[buf_cur];
    }

    return;
}

