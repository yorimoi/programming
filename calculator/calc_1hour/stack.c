int stack[100];
int stack_cur;

void push(int val) {
    stack[stack_cur++] = val;
}

int pop() {
    if (0 < stack_cur) {
        return stack[--stack_cur];
    } else {
        return 0;
    }
}

