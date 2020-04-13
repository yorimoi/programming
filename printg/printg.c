#include "printg.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    char* str;
    int size;
    int cur;
} String;

void size_check(String* str);
void toint(String* str, int num);
void touint(String* str, unsigned int num);
void tochar(String* str, char ch);
void tostr(String* str, char* ch);

int vprintg(int fd, const char* fmt, va_list ap) {
    char *c = (char *) fmt;
    String str = { NULL, 8, 0 };
    str.str = (char *) malloc(str.size * sizeof(char));

    if (str.str == NULL) {
        exit(1);
    }

    while (*c != '\0') {
        if (*c == '%') {
            switch (c[1]) {
                case 'd':
                    toint(&str, va_arg(ap, int));
                    c += 2;
                    break;
                case 'u':
                    touint(&str, va_arg(ap, unsigned int));
                    c += 2;
                    break;
                case 'c':
                    tochar(&str, va_arg(ap, int));
                    c += 2;
                    break;
                case 's':
                    tostr(&str, va_arg(ap, char*));
                    c += 2;
                    break;

                default: break;
            }
        } else {
            size_check(&str);
            str.str[str.cur++] = *c++;
        }
    }

    str.str[str.cur] = '\0';
    write(fd, str.str, str.cur);

    free(str.str);

    return str.cur;
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

void size_check(String *str) {
    if (str->size <= (str->cur)+1) {
        char *str_new = realloc(str->str, str->size * 2 * sizeof(char));
        if (str_new == NULL) {
            free(str->str);
            exit(1);
        }

        str->str = str_new;
        str->size *= 2;
    }
}

void toint(String* str, int num) {
    if (num == 0) {
        size_check(str);
        str->str[(str->cur)++] = '0';
        return;
    }

    char buf[10];
    int buf_cur = 0;
    int is_minus = 0;

    if (num < 0) {
        is_minus = 1;
        num = -num;
    }

    while (num != 0) {
        buf[buf_cur++] = num % 10 + '0';
        num /= 10;
    }

    if (is_minus) {
        size_check(str);
        str->str[(str->cur)++] = '-';
    }
    while (0 < buf_cur--) {
        size_check(str);
        str->str[(str->cur)++] = buf[buf_cur];
    }

    return;
}

void touint(String* str, unsigned int num) {
    if (num == 0) {
        size_check(str);
        str->str[(str->cur)++] = '0';
        return;
    }

    char buf[10];
    int buf_cur = 0;

    while (num != 0) {
        buf[buf_cur++] = num % 10 + '0';
        num /= 10;
    }

    while (0 < buf_cur--) {
        size_check(str);
        str->str[(str->cur)++] = buf[buf_cur];
    }

    return;
}

void tochar(String* str, char ch) {
    size_check(str);
    str->str[(str->cur)++] = ch;

    return;
}

void tostr(String* str, char* ch) {
    while (*ch) {
        size_check(str);
        str->str[(str->cur)++] = *ch;
        ++ch;
    }

    return;
}

