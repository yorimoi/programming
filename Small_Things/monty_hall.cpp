/**
 * モンティホール問題
 * 暇なときに最適化したい
*/
#include <iostream>
#include <random>

enum Choice {
    A,
    B,
    C,
    CHOICE_MAX,
};

int _pow(int x, int y) {
    if(y == 0) return 1;
    int z = x;
    for(; y>1; y--)
        z *= x;
    return z;
}

int main() {

    std::random_device rnd;

    int correct = 0;             // 正解の合計
    constexpr int trymax = 100;  // 試行回数

    for(int i=0; i<trymax; i++) {
        // 正解の決定
        int answer = _pow(2, rnd() % CHOICE_MAX);
        // 予想の決定
        int anticipation = _pow(2, rnd() % CHOICE_MAX);
        // ヤギを見せる
        int goat = (_pow(2, CHOICE_MAX)-1) ^ (answer | anticipation);
        if(goat == 3)      goat = rnd()%2 ? 1 : 2;
        else if(goat == 5) goat = rnd()%2 ? 1 : 4;
        else if(goat == 6) goat = rnd()%2 ? 2 : 4;
        // ドアの変更
        anticipation = (_pow(2, CHOICE_MAX)-1) ^ (anticipation | goat);

        if(anticipation == answer)
            correct++;
    }

    std::cout << "正解率: " << (double)correct / trymax << "%" << std::endl;

    return 0;
}
