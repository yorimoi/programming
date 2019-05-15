#include <cstdio>
#include <cstdint>
#include <stack>
#include <list>
#include <fstream>
#include <string>

using namespace std;

char xtoc(int x) {
    if(x <= 9)
        return x + '0';
    else if(10 <= x && x <= 16)
        return x + 'A' - 10;
    else
        return 0;
}

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    char *filename = argv[1];

    ifstream ifs;
    ifs.open(filename, ios::binary | ios::in);

    if(ifs.fail()) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return 1;
    }

    int DT_SIZE = 3000;
    list<uint8_t> d(DT_SIZE, 0);

    string o;
    char byte;

    ifs.seekg(0, ios_base::end);
    int filesize = ifs.tellg();
    ifs.seekg(4, ios_base::beg);

    while(ifs.tellg() < filesize - 128) {
        ifs.read(&byte, 1);
        o += xtoc((byte & 0b11110000) >> 4);
        o += xtoc(byte & 0b00001111);
    }

    list<uint8_t>::iterator dp = d.begin();
    stack<string::iterator> l, r;

    for(string::iterator oi=o.begin(); oi != o.end(); ++oi) {
        switch(*oi) {
            case '0':
            case '8': (*dp)++; break;
            case '1':
            case '9': (*dp)--; break;
            case '2':
            case 'A': dp++; break;
            case '3':
            case 'B': dp--; break;
            case '4':
            case 'C': l.push(oi); if(*dp==0){ oi=r.top();   r.pop();} break;
            case '5':
            case 'D': r.push(oi); if(*dp!=0){ oi=l.top()-1; l.pop();} break;
            case '6':
            case 'E': printf("%c", *dp);  break;
            case '7':
            case 'F': *dp = fgetc(stdin); break;
        }
    }

    ifs.close();
}

