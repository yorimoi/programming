/* 色々ガバガバ */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

struct Pair {
    int x;
    int y;
};

int solve(char **map, int m, int n, Pair g) {
    int vec[4][2] = {{-1,0},{0,1},{1,0},{0,-1},};
    int **checked = new int * [n];
    for(int i=0; i<n; i++)
        checked[i] = new int[m];
    memset(checked, 0, sizeof(int *) * n * m);

    // startが0,0限定
    while(1) {
        for(int h=0; h<n; h++) {
            for(int w=0; w<m; w++) {
                for(int v=0; v<4; v++) {
                    if(map[h][w] == '1') continue;
                    int x = w + vec[v][1],
                        y = h + vec[v][0];
                    if(x<0 || m<=x || y<0 || n<=y) continue;
                    if(checked[y][x] > 0 || map[y][x] == '1' || map[y][x] == 's'
                            || (checked[h][w]==0 && !(h==0 && w==0)))
                        continue;
                    checked[y][x] = checked[h][w] + 1;
                    if(checked[g.y][g.x] > 0)
                        return checked[y][x];
                }
            }
        }
    }

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++)
            printf("%2d", checked[i][j]);
        printf("\n");
    }

    for(int y=0; y<n; y++)
        for(int x=0; x<m; x++)
            if(map[y][x] == 'g') return checked[y][x];

    return -1;
}

int main(void) {
    int m, n;
    scanf("%d%d", &m, &n);
    char **map = (char **)malloc(sizeof(char *) * n);
    for(int i=0; i<n; i++)
        map[i] = (char *)malloc(sizeof(char *) * m);
    Pair goal;

    for(int h=0; h<n; h++) {
        char buf[128];
        fgets(buf, sizeof buf, stdin);
        for(int w=0; w<m; w++) {
            map[h][w] = buf[w];
            if(buf[w] == 'g') {
                goal.x = w;
                goal.y = h;
            }
        }
    }

    //printf("%d\n", solve(map, m, n, goal));

    return 0;
}
