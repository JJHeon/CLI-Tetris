#ifndef CLI_TETRIS_UI_H_
#define CLI_TETRIS_UI_H_

#include "device-controller.h"

namespace cli_tetris {

/** UI class
 *  ConsoleDevice를 반드시 정의를 유도
 *  Menu, 각 Part drawing등 다양한 drawing method 집합
 */
class Ui {
   private:
    ConsoleDevice* device_;

   public:
    Ui(ConsoleDevice* device);
    ~Ui();
};

}  // namespace cli_tetris

#endif  //CLI_TETRIS_UI_H_