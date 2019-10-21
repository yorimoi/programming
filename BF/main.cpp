#include <cstdio>
#include <cstdint>
#include <stack>
#include <list>

using namespace std;

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    constexpr int DT_SIZE = 3000;
    list<uint8_t> d(DT_SIZE, 0);
    char *o = argv[1];

    list<uint8_t>::iterator dp = d.begin();
    stack<char*> l, r;
    for(char *op=o; *op; op++) {
        switch(*op) {
            case '+': (*dp)++; break;
            case '-': (*dp)--; break;
            case '>': dp++; break;
            case '<': dp--; break;
            case '[': l.push(op); if(*dp==0){ op=r.top();   r.pop();} break;
            case ']': r.push(op); if(*dp!=0){ op=l.top()-1; l.pop();} break;
            case '.': printf("%c", *dp);  break;
            case ',': *dp = fgetc(stdin); break;
        }
    }
}
