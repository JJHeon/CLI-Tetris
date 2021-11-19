#include "random-generate-handler.h"

#include <cmath>

namespace cli_tetris::random {
RandomValueHandler::RandomValueHandler(
    const int& uniform_distribution_start,
    const int& uniform_distribution_end,
    const int& normal_distribution_1_mean,
    const int& normal_distribution_1_diviation,
    const int& normal_distribution_2_mean,
    const int& normal_distribution_2_diviation,
    const int& normal_distribution_3_mean,
    const int& normal_distribution_3_diviation,
    const int& normal_distribution_4_mean,
    const int& normal_distribution_4_diviation,
    const int& normal_distribution_5_mean,
    const int& normal_distribution_5_diviation,
    const int& normal_distribution_6_mean,
    const int& normal_distribution_6_diviation,
    const int& normal_distribution_7_mean,
    const int& normal_distribution_7_diviation,
    const int& normal_distribution_8_mean,
    const int& normal_distribution_8_diviation,
    const int& normal_distribution_9_mean,
    const int& normal_distribution_9_diviation,
    const int& normal_distribution_10_mean,
    const int& normal_distribution_10_diviation,
    const int& normal_distribution_11_mean,
    const int& normal_distribution_11_diviation)
    : generator_(random_device_()),
      uniform_distribution_(uniform_distribution_start, uniform_distribution_end),
      normal_distribution_1_(normal_distribution_1_mean, normal_distribution_1_diviation),
      normal_distribution_2_(normal_distribution_2_mean, normal_distribution_2_diviation),
      normal_distribution_3_(normal_distribution_3_mean, normal_distribution_3_diviation),
      normal_distribution_4_(normal_distribution_4_mean, normal_distribution_4_diviation),
      normal_distribution_5_(normal_distribution_5_mean, normal_distribution_5_diviation),
      normal_distribution_6_(normal_distribution_6_mean, normal_distribution_6_diviation),
      normal_distribution_7_(normal_distribution_7_mean, normal_distribution_7_diviation),
      normal_distribution_8_(normal_distribution_8_mean, normal_distribution_8_diviation),
      normal_distribution_9_(normal_distribution_9_mean, normal_distribution_9_diviation),
      normal_distribution_10_(normal_distribution_10_mean, normal_distribution_10_diviation),
      normal_distribution_11_(normal_distribution_11_mean, normal_distribution_11_diviation) {}

int RandomValueHandler::getUniformRandomNumber() {
    return uniform_distribution_(generator_);
}
int RandomValueHandler::getNormalRandomNumberVer1() {
    return std::round(normal_distribution_1_(generator_));
}
int RandomValueHandler::getNormalRandomNumberVer2() {
    return std::round(normal_distribution_2_(generator_));
}
int RandomValueHandler::getNormalRandomNumberVer3() {
    return std::round(normal_distribution_3_(generator_));
}
int RandomValueHandler::getNormalRandomNumberVer4() {
    return std::round(normal_distribution_4_(generator_));
}
int RandomValueHandler::getNormalRandomNumberVer5() {
    return std::round(normal_distribution_5_(generator_));
}
int RandomValueHandler::getNormalRandomNumberVer6() {
    return std::round(normal_distribution_6_(generator_));
}
int RandomValueHandler::getNormalRandomNumberVer7() {
    return std::round(normal_distribution_7_(generator_));
}
int RandomValueHandler::getNormalRandomNumberVer8() {
    return std::round(normal_distribution_8_(generator_));
}
int RandomValueHandler::getNormalRandomNumberVer9() {
    return std::round(normal_distribution_9_(generator_));
}
int RandomValueHandler::getNormalRandomNumberVer10() {
    return std::round(normal_distribution_10_(generator_));
}
int RandomValueHandler::getNormalRandomNumberVer11() {
    return std::round(normal_distribution_11_(generator_));
}

}  // namespace cli_tetris::random
