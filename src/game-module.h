#ifndef CLI_TETRIS_GAME_MODULE_H_
#define CLI_TETRIS_GAME_MODULE_H_

#include "user-data.h"

namespace cli_tetris {

/* User Data Class ===================================================================================== */
// using GameState = enum GameState {
//     kMenu = 0,
//     kPlaySolo,
//     kPlayDuo,
//     kPlayMulti,
//     kCreditPage,
//     kAchievement,
//     kSettings
// };

class GameCore {
   private:
   public:
    virtual ~GameCore();
    virtual void MoveStateHandler() = 0;
    virtual void InputProcess(UserData& user_data) = 0;
    virtual void UpdateProcess(UserData& user_data) = 0;
    virtual void RenderProcess(UserData& user_data) = 0;
};

class GameTetris {
   private:
    GameCore* game_core;
    UserData* user_list_[5];
};

}  //namespace cli_tetris

#endif  //CLI_TETRIS_GAME_MODULE_H_