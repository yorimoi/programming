#include <cstdio>

using namespace std;

int main() {
    int w, h;
    scanf("%d%d", &w, &h);

    int **map = new int * [h];
    for(int i=0; i<h; i++)
        map[i] = new int[w];

    /* 秋田 */

    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++)
            printf(map[y][x] ? "#" : " ");
        printf("\n");
    }


    return 0;
}
