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
constexpr int    MAX_GENERATION      = 100;  // 繰り返す世代数


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
        memcpy(this->genom_list, genom_list, sizeof(int) * GENOM_LENGTH);
    }

    void set_evaluation(double evaluation) {this->evaluation = evaluation; }

    // sort時に使用
    bool operator<(const genom& another) const {
        return evaluation < another.evaluation;
    }
    bool operator>(const genom& another) const {
        return evaluation > another.evaluation;
    }
};


/** 
 * @brief      引数で指定された桁のランダムな遺伝子情報を生成
 * 
 * @param[in]  遺伝子情報の長さ
 * @return     生成したインスタンス
 */
genom create_genom(int length) {
    std::random_device rnd;
    int *genome_list = new int[length];
    for(int i=0; i<length; i++)
        genome_list[i] = rnd() % 2;
    return genom(genome_list, 0);
}


/**
 * @brief      評価関数
 *             0.00 ~ 1.00 で評価
 * 
 * @param[in]  評価を行うgenomClass
 * @return     評価値
 */
double evaluation(genom& ga) {
    double genom_total = 0.0;
    for(int *i=ga.get_Genom(), _j=0; _j<GENOM_LENGTH; _j++)
        genom_total += *i++;
    return genom_total / GENOM_LENGTH;
}


/**
 * @brief      選択関数
 * 
 * @param[in]  エリートを格納するgenomClass,
 * @param[in]  評価元のgenomClass,
 * @param[in]  選択するエリート数
 * @return     <none>
 */
void select(std::vector<genom>& elite, std::vector<genom>& ga, int elite_len) {
    std::vector<genom> sort_ary(ga);
    // 現行世代個体集団を評価の高い順にソート
    std::sort(sort_ary.begin(), sort_ary.end(), std::greater<genom>());

    // 一定の上位を格納
    for(int i=0; i<elite_len; i++)
        elite.push_back(sort_ary.at(i));
}


/**
 * @brief      交叉関数 => 二点交叉
 * 
 * @param[in]  交叉結果を格納するgenomClass,
 * @param[in]  一つ目の個体,
 * @param[in]  二つ目の個体
 * @return     <none>
 */
void crossover(std::vector<genom>& genom_list,
                    genom& ga_first, genom& ga_second) {
    std::random_device rnd;
    int cross_first, cross_second;
    int *lists_first, *lists_second;
    int *progeny_first  = new int[GENOM_LENGTH];
    int *progeny_second = new int[GENOM_LENGTH];

    // 入れ替える二点の範囲をランダムに設定
    cross_first  = rnd() % GENOM_LENGTH;
    cross_second = cross_first + rnd() % (GENOM_LENGTH - cross_first);

    // 遺伝子を取り出す
    lists_first  = ga_first.get_Genom();
    lists_second = ga_second.get_Genom();
    memcpy(progeny_first,  lists_first,  sizeof(int) * GENOM_LENGTH);
    memcpy(progeny_second, lists_second, sizeof(int) * GENOM_LENGTH);

    // 交叉させる
    for(int i=cross_first; i<=cross_second; i++) {
        progeny_first[i]  = lists_second[i];
        progeny_second[i] = lists_first[i];
    }

    // インスタンスを生成して子孫を格納
    genom_list.push_back(genom(progeny_first, 0));
    genom_list.push_back(genom(progeny_second, 0));
}


/**
 * @brief      世代交代処理
 * 
 * @param[in]  次世代集団
 * @param[in]  現世代集団
 * @param[in]  エリート集団
 * @param[in]  子孫集団
 * @return     <none>
 */
void next_generation_gene_create(std::vector<genom>& next_gen_group,
                        std::vector<genom>& ga, std::vector<genom>& ga_elite,
                        std::vector<genom>& ga_progeny) {
    std::copy(ga.begin(), ga.end(), next_gen_group.begin());
    // 現世代を評価の低い順にソート
    std::sort(next_gen_group.begin(), next_gen_group.end());

    // 追加するエリート集団と子孫集団の合計分を取り除く
    next_gen_group.erase(next_gen_group.begin(),
            next_gen_group.begin() + ga_elite.size() + ga_progeny.size());

    // エリート集団と子孫集団を次世代集団へ追加
    next_gen_group.insert(next_gen_group.end(),
                          ga_elite.begin(), ga_elite.end());
    next_gen_group.insert(next_gen_group.end(),
                          ga_progeny.begin(), ga_progeny.end());
}


/**
 * @brief  突然変異関数
 * 
 * @param[in]  次世代集団
 * @param[in]  個体突然変異確率
 * @param[in]  遺伝子突然変異確率
 * @return     <none>
 */
void mutation(std::vector<genom>& ga,
                double individual_mutaion, double genom_mutation) {
    std::random_device rnd;
    for(auto &i: ga) {
        // 個体に対して一定の確率で突然変異が起きる
        if(individual_mutaion > (rnd()%100 / 100.0)) {
            // 個体の遺伝子情報一つ一つに対して突然変異がおこる
            for(int j=0; j<GENOM_LENGTH; j++) {
                // 0 と 1 を入れ替える
                if(genom_mutation > (rnd()%100 / 100.0))
                    *(i.get_Genom()+j) ^= 1;
            }
        }
    }
}


int main(void) {

    // 第1世代の生成
    std::vector<genom> current_gen_group;// (MAX_GENOM_LIST);
    for(int i=0; i<MAX_GENOM_LIST; i++)
        current_gen_group.push_back(create_genom(GENOM_LENGTH));

    for(int g=1; g<MAX_GENERATION+1; g++) {
        // 現行世代個体集団の遺伝子を評価し, 代入する
        for(int i=0; i<MAX_GENOM_LIST; i++)
            current_gen_group[i].set_evaluation(evaluation(current_gen_group[i]));

        // エリート個体を選択
        std::vector<genom> elite_genes;
        select(elite_genes, current_gen_group, SELECT_GENOM);

        // エリート遺伝子を交叉させ, 格納
        std::vector<genom> progeny_gene;
        for(int i=1; i<SELECT_GENOM; i++)
            crossover(progeny_gene, elite_genes[i-1], elite_genes[i]);

        // 次世代個体集団を現行世代、エリート集団、子孫集団から作成
        std::vector<genom> next_gen_group (MAX_GENOM_LIST);
        next_generation_gene_create(next_gen_group, current_gen_group,
                                    elite_genes, progeny_gene);

        // 次世代個体集団全ての個体に突然変異を施す
        mutation(next_gen_group, INDIVIDUAL_MUTATION, GENOM_MUTATION);

        // 各個体適用度を配列化
        std::vector<double> fits;
        for(auto i: current_gen_group)
            fits.push_back(i.get_evaluation());
        
        // 進化結果を評価
        double min = *std::min_element(fits.begin(), fits.end());
        double max = *std::max_element(fits.begin(), fits.end());
        double avg = std::accumulate(fits.begin(), fits.end(), 0.0)/fits.size();

        // 現行世代の進化結果を出力
        std::cout << "-----第" << g << "世代の結果-----\n";
        std::cout << "  Min:" << min << "\n";
        std::cout << "  Max:" << max << "\n";
        std::cout << "  Avg:" << avg << std::endl;

        if(g < MAX_GENERATION)
            // 現行世代と次世代を入れ替え
            std::copy(next_gen_group.begin(), next_gen_group.end(),
                      current_gen_group.begin());
        else {
            std::cout << "最終エリート個体:[";
            for(int i=0; i<GENOM_LENGTH-1; i++)
                std::cout << *(elite_genes[0].get_Genom()+i);
            std::cout << *(elite_genes[0].get_Genom()+GENOM_LENGTH-1) << "]\n";
        }
    }

    return 0;
}
