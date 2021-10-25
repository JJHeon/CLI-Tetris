#include "user-data.h"

namespace cli_tetris {

UserData::UserData(std::string&& name, GameLevel level)
    : name_(std::move(name)), game_level_(level) {
}

}  // namespace cli_tetris