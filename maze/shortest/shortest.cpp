#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef vector<vector<char>> field_t;

typedef pair<int, int> point_t;

point_t operator+(const point_t &lhs, const point_t &rhs) {
    point_t p = {lhs.first + rhs.first, lhs.second + rhs.second};
    return p;
}

bool operator==(const point_t &lhs, const point_t &rhs) {
    return (lhs.first == rhs.first) && (lhs.second == rhs.second);
}

bool is_in_field(int col, int row, const point_t &point) {
    const int c = point.second;
    const int r = point.first;
    return (0 <= c && c < col) && (0 <= r && r < row);
}

int solve(int col, int row, field_t &field,
        const point_t &start, const point_t &goal) {
    vector<vector<int>> memo;
    for(int i=0; i<row; ++i) {
        vector<int> v(col, 0);
        memo.push_back(v);
    }

    const point_t points[] = {
        {-1,0}, {1,0}, {0,-1}, {0,1},
    };

    queue<point_t> q;
    q.push(start);

    while(!q.empty()) {
        point_t cur = q.front(); q.pop();
        if(cur == goal)
            return memo[cur.first][cur.second];

        for(const auto &point : points) {
            point_t next = cur + point;
            if(is_in_field(col, row, next)) {
                const char s = field[next.first][next.second];
                if(s == '0' || s == 'g') {
                    if(memo[next.first][next.second] == 0) {
                        q.push(next);
                        memo[next.first][next.second]
                            = memo[cur.first][cur.second] + 1;
                    }
                }
            }
        }
    }

    return -1;
}

int main() {

    int col, row;
    cin >> col >> row;

    point_t start, goal;
    field_t field;

    for(int i=0; i<row; ++i) {
        vector<char> v;
        for(int j=0; j<col; ++j) {
            char c;
            cin >> c;
            v.push_back(c);
            if(c == 's') {
                start.first = i;
                start.second = j;
            } else if(c == 'g') {
                goal.first = i;
                goal.second = j;
            }
        }
        field.push_back(v);
    }

    cout << solve(col, row, field, start, goal) << endl;

    return 0;
}
