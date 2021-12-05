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
    GameLevel saved_game_level_;

   public:
    UserData(std::string&& name, GameLevel level = GameLevel::kEasy);
    ~UserData();

    void SetName(const std::string& name);
    void SaveGameLevel(const GameLevel level);
};

class GameData {
   private:
    GameLevel current_game_level_;
    int block_speed_;
    std::vector<object::GraphicObject> blocks;

   public:
    void AddObject(std::vector<object::GraphicObject>* object);
};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_USER_DATA_H_