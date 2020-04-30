#include <stdio.h>
#include <stdlib.h>

void encode_block(unsigned char *in, int len) {
    static const char base64_table[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    unsigned char out[5];

    out[0] = base64_table[(int)(in[0] >> 2)];
    out[1] = base64_table[(int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4))];
    out[2] = 1 < len ? base64_table[(int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6))] : '=';
    out[3] = 2 < len ? base64_table[(int)(in[2] & 0x3f)] : '=';
    out[4] = 0;

    printf("%s", out);
}

void encode(char *input) {
    unsigned char in[3];
    unsigned char *c = (unsigned char*) input;
    int len = 0;

    while (1) {
        len = 0;
        for (int i = 0; i < 3; ++i) {
            if (*c) {
                in[i] = *c;
                ++c;
                ++len;
            } else {
                in[i] = 0;
            }
        }
        if (0 < len) {
            encode_block(in, len);
        } else {
            return;
        }
    }
}

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Invalid argument\n");
        exit(1);
    }

    char *input = argv[1];
    printf("Input: %s\n", input);

    encode(input);

    printf("\n");

    return 0;
}

