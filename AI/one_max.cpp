/**
 * Re:ゼロから始める遺伝的アルゴリズム をC++で書き直してみたもの
 * https://qiita.com/Azunyan1111/items/975c67129d99de33dc21
*/

#include <iostream>
#include <cstring>
#include <random>
#include <algorithm>
#include <vector>

constexpr int    GENOM_LENGTH        = 100;  // 遺伝子情報の長さ
constexpr int    MAX_GENOM_LIST      = 100;  // 遺伝子集団の大きさ
constexpr int    SELECT_GENOM        = 20;   // 遺伝子選択数
constexpr double INDIVIDUAL_MUTATION = 0.1;  // 個体突然変異確率
constexpr double GENOM_MUTATION      = 0.1;  // 遺伝子突然変異確率
constexpr int    MAX_GENERATION      = 40;   // 繰り返す世代数

class genom {
    int genom_list[GENOM_LENGTH];
    double evaluation;

public:
    genom() {}

    genom(int *g, double e) {
        memcpy(this->genom_list, g, sizeof(int) * GENOM_LENGTH);
        this->evaluation = e;
        delete[] g;
    }

    int *get_Genom() { return this->genom_list; }

    double get_evaluation() { return this->evaluation; }

    void set_Genom(int *genom_list) {
        memcpy(this->genom_list, genom_list, sizeof(int*) *GENOM_LENGTH);
    }

    void set_evaluation(double evaluation) {this->evaluation = evaluation; }

    // sort時に使用
    bool operator<(const genom& another) const {
        return evaluation < another.evaluation;
    }
};


genom create_genom(int length) {
    std::random_device rnd;
    int *genome_list = new int[length];
    for(int i=0; i<length; i++)
        genome_list[i] = rnd() % 2;
    return genom(genome_list, 0);
}

double evaluation(genom& ga) {
    double genom_total = 0;
    for(int *i=ga.get_Genom(), _j=0; _j<GENOM_LENGTH; _j++)
        genom_total += *i++;
    return genom_total / GENOM_LENGTH;
}

std::vector<genom> select(std::vector<genom>& ga, int elite_length) {
    std::vector<genom> sort_ary(ga);
    std::sort(sort_ary.begin(), sort_ary.end());

    std::vector<genom> result;
    for(int i=0; i<elite_length; i++)
        result.push_back(sort_ary.at(i));
    return result;
}

void crossover(std::vector<genom>& genom_list, genom ga_first, genom ga_second) {
    std::random_device rnd;
    int cross_first, cross_second;
    int *lists_first, *lists_second;
    int *progeny_first  = new int[GENOM_LENGTH];
    int *progeny_second = new int[GENOM_LENGTH];

    cross_first  = rnd() % GENOM_LENGTH;
    cross_second = cross_first + rnd() % (GENOM_LENGTH - cross_first);
    lists_first  = ga_first.get_Genom();
    lists_second = ga_second.get_Genom();
    memcpy(progeny_first , lists_first,  sizeof(int) * GENOM_LENGTH);
    memcpy(progeny_second, lists_second, sizeof(int) * GENOM_LENGTH);

    for(int i=cross_first; i<cross_second; i++) {
        progeny_first[i]  = lists_second[i];
        progeny_second[i] = lists_first[i];
    }

    genom_list.push_back(genom(progeny_first, 0));
    genom_list.push_back(genom(progeny_second, 0));
}

int main(void) {

    // 第1世代の生成
    std::vector<genom> current_gen_group;// (MAX_GENOM_LIST);
    for(int i=0; i<MAX_GENOM_LIST; i++)
        current_gen_group.push_back(create_genom(GENOM_LENGTH));

    //for(int i=0; i<GENOM_LENGTH; i++)
    //    std::cout << *(current_gen_group[1].get_Genom()+i) << ","; // OK
    //std::cout << evaluation(current_gen_group[0]) << std::endl; // OK

    // 現行世代個体集団の遺伝子を評価し, 代入する
    for(int i=0; i<MAX_GENOM_LIST; i++)
        current_gen_group[i].set_evaluation(evaluation(current_gen_group[i]));

    // エリート個体を選択
    std::vector<genom> elite_genes = select(current_gen_group, SELECT_GENOM);

    // エリート遺伝子を交叉させ, 格納
    std::vector<genom> progeny_gene;
    for(int i=1; i<SELECT_GENOM; i++)
        crossover(progeny_gene, elite_genes[i-1], elite_genes[i]);

    // 次世代個体集団を現行世代、エリート集団、子孫集団から作成

    // 次世代個体集団全ての個体に突然変異を施す

    // 各個体適用度を配列化

    // 進化結果を評価

    // 現行世代の進化結果を出力

    // 現行世代と次世代を入れ替え

    return 0;
}
