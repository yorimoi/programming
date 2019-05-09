#include <cstdio>
#include <cstdint>
#include <cstring>
#include <vector>
#include <stack>
#include <list>

struct Order {
    char order;
    int vs;  // [,] の対の要素番号

    Order(char o, int v) { order = o; vs = v; }
    Order(int v) { vs = v; }
};

void store(std::vector<Order>& o, char *c) {
    int i = 0;
    std::stack<int> s;
    while(*c) {
        if(strchr("+-><[].,", *c)) {
            o.push_back(Order(*c, 0));
            if(*c == '[')
                s.push(i);
            else if(*c == ']') {
                o.insert(o.begin()+i, Order(s.top()));
                o.insert(o.begin()+s.top(), Order(i));
                s.pop();
            }
            i++;
        }
        c++;
    }
}

void analysis(std::vector<Order>& o, std::list<uint8_t>& d) {
    std::vector<Order>::iterator op = o.begin();
    std::list<uint8_t>::iterator dp = d.begin();
    while(op->order) {
        switch(op->order) {
            case '+': (*dp)++; break;
            case '-': (*dp)--; break;
            case '>': dp++; break;
            case '<': dp--; break;
            case '[': if(*dp == 0) op += op->vs;   break;
            case ']': if(*dp != 0) op -= op->vs-1; break;
            case '.': printf("%c", *dp);  break;
            case ',': *dp = fgetc(stdin); break;
        }
        op++;
    }
}

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    constexpr int DT_SIZE = 3000;
    std::list<uint8_t> d(DT_SIZE, 0);
    std::vector<Order> o;

    store(o, argv[1]);
    analysis(o, d);

    return 0;
}
