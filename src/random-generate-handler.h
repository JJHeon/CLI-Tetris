/** Note
 * random-generate-handler
 *
 * standard Cpp의 random 을 사용하기 쉽게 warrap한 class 입니다.
 * 생성 단계에서 사용을 위한 초기화를 모두 끝내고 get-Method를 통해 Random value를 바로 얻습니다.
 *
 */

#ifndef CLI_TETRIS_RANDOM_GENERATE_HANDLER_H_
#define CLI_TETRIS_RANDOM_GENERATE_HANDLER_H_

#include <random>

namespace cli_tetris::random {
class RandomValueHandler final {
   private:
    std::random_device random_device_;
    std::mt19937 generator_;
    std::uniform_int_distribution<int> uniform_distribution_1_;
    std::uniform_int_distribution<int> uniform_distribution_2_;
    // std::normal_distribution<double> normal_distribution_1_;
    // std::normal_distribution<double> normal_distribution_2_;
    // std::normal_distribution<double> normal_distribution_3_;
    // std::normal_distribution<double> normal_distribution_4_;
    // std::normal_distribution<double> normal_distribution_5_;
    // std::normal_distribution<double> normal_distribution_6_;
    // std::normal_distribution<double> normal_distribution_7_;
    // std::normal_distribution<double> normal_distribution_8_;
    // std::normal_distribution<double> normal_distribution_9_;
    // std::normal_distribution<double> normal_distribution_10_;
    // std::normal_distribution<double> normal_distribution_11_;

   public:
    RandomValueHandler(
        const int& uniform_distribution_1_start,
        const int& uniform_distribution_1_end,
        const int& uniform_distribution_2_start,
        const int& uniform_distribution_2_end);
    // RandomValueHandler(
    //     const int& uniform_distribution_start,
    //     const int& uniform_distribution_end,
    //     const int& normal_distribution_1_mean,
    //     const int& normal_distribution_1_diviation,
    //     const int& normal_distribution_2_mean,
    //     const int& normal_distribution_2_diviation,
    //     const int& normal_distribution_3_mean,
    //     const int& normal_distribution_3_diviation,
    //     const int& normal_distribution_4_mean,
    //     const int& normal_distribution_4_diviation,
    //     const int& normal_distribution_5_mean,
    //     const int& normal_distribution_5_diviation,
    //     const int& normal_distribution_6_mean,
    //     const int& normal_distribution_6_diviation,
    //     const int& normal_distribution_7_mean,
    //     const int& normal_distribution_7_diviation,
    //     const int& normal_distribution_8_mean,
    //     const int& normal_distribution_8_diviation,
    //     const int& normal_distribution_9_mean,
    //     const int& normal_distribution_9_diviation,
    //     const int& normal_distribution_10_mean,
    //     const int& normal_distribution_10_diviation,
    //     const int& normal_distribution_11_mean,
    //     const int& normal_distribution_11_diviation);
    ~RandomValueHandler() = default;

    int getUniform1RandomNumber();
    int getUniform2RandomNumber();
    // int getNormalRandomNumberVer1();
    // int getNormalRandomNumberVer2();
    // int getNormalRandomNumberVer3();
    // int getNormalRandomNumberVer4();
    // int getNormalRandomNumberVer5();
    // int getNormalRandomNumberVer6();
    // int getNormalRandomNumberVer7();
    // int getNormalRandomNumberVer8();
    // int getNormalRandomNumberVer9();
    // int getNormalRandomNumberVer10();
    // int getNormalRandomNumberVer11();
};

}  // namespace cli_tetris::random

#endif  // CLI_TETRIS_RANDOM_GENERATE_HANDLER_H_
