#include "printg.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum {
    HEX_SMALL,
    HEX_LARGE,
} HexFlags;

typedef struct {
    char* str;
    int size;
    int cur;
} String;

void size_check(String* str);
void i2a(String* str, int num, int length);
void ui2a(String* str, unsigned int num, int length);
void c2a(String* str, char ch, int length);
void s2a(String* str, char* ch, int length);
void f2a(String* str, double num, int length);
void x2a(String* str, unsigned int num, int length, HexFlags hf);
int is_num(char ch);
int numlen(int num);

int vprintg(int fd, const char* fmt, va_list ap) {
    int length = 0;
    char *c = (char *) fmt;
    String str = { NULL, 8, 0 };
    str.str = (char *) malloc(str.size * sizeof(char));

    if (str.str == NULL) {
        exit(1);
    }

    while (*c != '\0') {
        if (*c == '%') {
            while (is_num(c[1])) {
                length = length * 10 + c[1] - '0';
                ++c;
            }

            switch (c[1]) {
                case 'd':
                    i2a(&str, va_arg(ap, int), length);
                    c += 2;
                    break;
                case 'u':
                    ui2a(&str, va_arg(ap, unsigned int), length);
                    c += 2;
                    break;
                case 'x':
                    x2a(&str, va_arg(ap, unsigned int), length, HEX_SMALL);
                    c += 2;
                    break;
                case 'X':
                    x2a(&str, va_arg(ap, unsigned int), length, HEX_LARGE);
                    c += 2;
                    break;
                case 'c':
                    c2a(&str, va_arg(ap, int), length);
                    c += 2;
                    break;
                case 's':
                    s2a(&str, va_arg(ap, char*), length);
                    c += 2;
                    break;
                case 'f':
                    f2a(&str, va_arg(ap, double), length);
                    c += 2;
                    break;
                case '%':
                    c2a(&str, '%', 1);
                    c += 2;
                    break;

                default:
                    c2a(&str, '%', 1);
                    i2a(&str, length, 0);
                    c2a(&str, c[1], 1);
                    c += 2;
                    break;
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

void i2a(String* str, int num, int length) {
    char buf[10];
    int buf_cur = 0;
    int is_minus = 0;

    if (num < 0) {
        is_minus = 1;
        num = -num;
        --length;
    }

    while (1) {
        buf[buf_cur++] = num % 10 + '0';
        num /= 10;
        --length;
        if (num == 0) {
            break;
        }
    }

    for (int i = 0; i < length; ++i) {
        size_check(str);
        str->str[(str->cur)++] = ' ';
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

void ui2a(String* str, unsigned int num, int length) {
    char buf[10];
    int buf_cur = 0;

    while (1) {
        buf[buf_cur++] = num % 10 + '0';
        num /= 10;
        --length;
        if (num == 0) {
            break;
        }
    }

    for (int i = 0; i < length; ++i) {
        size_check(str);
        str->str[(str->cur)++] = ' ';
    }

    while (0 < buf_cur--) {
        size_check(str);
        str->str[(str->cur)++] = buf[buf_cur];
    }

    return;
}

void c2a(String* str, char ch, int length) {
    for (int i = 0; i < length-1; ++i) {
        size_check(str);
        str->str[(str->cur)++] = ' ';
    }
    size_check(str);
    str->str[(str->cur)++] = ch;

    return;
}

void s2a(String* str, char* ch, int length) {
    char *c = ch;
    while (*c++) {
        --length;
    }
    for (int i = 0; i < length; ++i) {
        size_check(str);
        str->str[(str->cur)++] = ' ';
    }
    while (*ch) {
        size_check(str);
        str->str[(str->cur)++] = *ch;
        ++ch;
    }

    return;
}

void f2a(String* str, double num, int length) {
    int whole  = (int)num;
    double tmp = (num - whole) * 1000000;
    unsigned long frac = (unsigned long)tmp;
    double diff = tmp - frac;
    char buf[8];

    for (int i = numlen(whole)+1+6; i < length; ++i) {
        size_check(str);
        str->str[(str->cur)++] = ' ';
    }

    i2a(str, whole, 0);
    c2a(str, '.', 1);

    if (0.5 < diff) {
        ++frac;
        if (1000000 <= frac) {
            frac = 0;
            ++whole;
        }
    } else if (diff < 0.5) {
        // Do nothing
    } else if ((frac == 0U) || (frac & 1U)) {
        ++frac;
    }

    for (int i = 0; i < 6; ++i) {
        buf[i] = frac % 10 + '0';
        frac /= 10;
    }
    for (int i = 5; 0 <= i; --i) {
        size_check(str);
        str->str[(str->cur)++] = buf[i];
    }

    return;
}

void x2a(String* str, unsigned int num, int length, HexFlags hf) {
    char buf[10];
    int buf_cur = 0;

    while (1) {
        buf[buf_cur++] = num % 16;
        num /= 16;
        --length;
        if (num == 0) {
            break;
        }
    }

    for (int i = 0; i < length; ++i) {
        size_check(str);
        str->str[(str->cur)++] = ' ';
    }

    while (0 < buf_cur--) {
        size_check(str);
        if (buf[buf_cur] <= 9) {
            str->str[(str->cur)++] = buf[buf_cur] + '0';
        } else if (hf == HEX_LARGE) {
            str->str[(str->cur)++] = buf[buf_cur] - 10 + 'A';
        } else {
            str->str[(str->cur)++] = buf[buf_cur] - 10 + 'a';
        }
    }

    return;
}

int is_num(char ch) {
    return '0' <= ch && ch <= '9';
}

int numlen(int num) {
    int len = 1;

    if (num == 0) {
        return 1;
    }

    if (num < 0) {
        ++len;
        num = -num;
    }

    while (num /= 10) {
        ++len;
    }

    return len;
}


// tests
void assert(int expect, int actual) {
    if (expect == actual) {
        printg("[ \033[32mOK\033[0m ]\n");
    } else {
        printg("[ \033[31mNG\033[0m ]  expect %d, but got %d\n", expect, actual);
    }
}

void is_num_test() {
    printg("> is_num_test()\n");
    assert(1, is_num('0'));
    assert(1, is_num('9'));
    assert(0, is_num('a'));
    assert(0, is_num('/'));
    assert(0, is_num(':'));
}

void numlen_test() {
    printg("> numlen_test()\n");
    assert(1, numlen(0));
    assert(1, numlen(1));
    assert(1, numlen(9));
    assert(2, numlen(10));
    assert(2, numlen(-1));
    assert(3, numlen(100));
}

void function_tests() {
    is_num_test();
    numlen_test();
}

