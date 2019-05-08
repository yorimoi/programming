#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum {
    DT_SIZE = 3000,
};

enum {
    OD_PLUS,  // '+' +ポインタが指す値をインクリメント
    OD_MINUS, // '-' -ポインタが指す値をデクリメント
    OD_GT,    // '>' >ポインタをインクリメント
    OD_LT,    // '<' <ポインタをデクリメント
    OD_SBL,   // '[' [ポインタが指す値が0なら, 対応する]の直後までジャンプ
    OD_SBR,   // ']' ]ポインタが指す値が0でないなら, 対応する[にジャンプ
    OD_DOT,   // '.' .ポインタが指す値を出力 (ascii)
    OD_COMMA, // ',' 1バイトを入力してポインタが指す値に代入
    OD_MAX,
};

// 循環リスト
typedef struct Data {
    struct Data *next;
    struct Data *prev;
    uint8_t data;
} Data;

typedef struct Order {
    char order;
    int vs;  // [,] の対の要素番号
} Order;

typedef struct Stack {
    int ary[DT_SIZE];
    unsigned int top;  // 末尾の要素番号
} Stack;

void push(Stack *s, int i) {
    s->ary[s->top] = i;
    s->top++;
}

int pop(Stack *s) {
    if(s->top > 0)
        s->top--;
    int o = s->ary[s->top];
    return o;
}

void store(Order *o, char *c) {
    int i = 0;
    Stack s;
    while(*c) {
        if(strchr("+-><[].,", *c)) {
            o[i].order = *c;
            if(*c == '[')
                push(&s, i);
            else if(*c == ']') {
                o[i].vs = pop(&s);
                o[o[i].vs].vs = i;
            }
            i++;
        }
        c++;
    }
}

void analysis(Order *o, Data *d) {
    Order *op = o;
    Data  *dp = d;
    while(op->order) {
        switch(op->order) {
            case '+': dp->data++; break;
            case '-': dp->data--; break;
            case '>': dp = dp->next; break;
            case '<': dp = dp->prev; break;
            case '[': if(dp->data==0) op = (o+op->vs);   break;
            case ']': if(dp->data!=0) op = (o+op->vs-1); break;
            case '.': /*return dp->data;*/printf("%c", dp->data); break;
            case ',': scanf("%c", &dp->data); scanf("%*c"); break;
        }
        op++;
    }
}

int main(int argc, char **argv) {

    if(argc < 2) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    Data d[DT_SIZE] = {0};

    for(int i=1; i<DT_SIZE-1; i++) {
        d[i].next = &d[i+1];
        d[i].prev = &d[i-1];
    }
    d[DT_SIZE-1].next = d;
    d[DT_SIZE-1].prev = &d[DT_SIZE-2];
    d[0].next = &d[1];
    d[0].prev = &d[DT_SIZE-1];

    Order *o = (Order *)malloc(DT_SIZE);
    store(o, argv[1]);

    analysis(o, d);
    printf("\n");

    return 0;
}
