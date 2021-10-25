#ifndef CLI_TETRIS_USER_DATA_H_
#define CLI_TETRIS_USER_DATA_H_

#include <vector>
#include <string>

#include "object-defined.h"

namespace cli_tetris {

using GameLevel = enum GameLevel {
    kEasy,
    kNormal,
    kHard
};

class UserData {
   private:
    std::string name_;
    GameLevel game_level_;
    std::vector<Object> block_list_;

   public:
    UserData(std::string&& name, GameLevel level = GameLevel::kEasy);
    ~UserData();

    void AddObject(std::vector<Object>* object);
};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_USER_DATA_H_