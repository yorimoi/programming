/**
 * モンティホール問題
 * 暇なときに最適化したい
*/
#include <iostream>
#include <random>

int pow_(int x, int y) {
    if(y == 0) return 1;
    int z = x;
    for(; y>1; y--)
        z *= x;
    return z;
}

int main() {

    std::random_device rnd;

    int correct = 0;               // 正解の合計
    constexpr int trymax = 100;    // 試行回数
    constexpr int choice_max = 3;  // 選択肢の最大

    for(int _i=0; _i<trymax; _i++) {
        // 正解の決定
        int answer = pow_(2, rnd() % choice_max);
        // 予想の決定
        int anticipation = pow_(2, rnd() % choice_max);
        // ヤギを見せる
        int goat = (pow_(2, choice_max)-1) ^ (answer | anticipation);
        if(goat == 3)      goat = rnd()%2 ? 1 : 2;
        else if(goat == 5) goat = rnd()%2 ? 1 : 4;
        else if(goat == 6) goat = rnd()%2 ? 2 : 4;
        // ドアの変更
        anticipation = (pow_(2, choice_max)-1) ^ (anticipation | goat);

        if(anticipation == answer)
            correct++;
    }

    std::cout << "正解率: " << (double)correct / trymax << "%" << std::endl;

    return 0;
}
