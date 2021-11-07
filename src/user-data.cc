#include "user-data.h"

namespace cli_tetris {

UserData::UserData(std::string&& name, GameLevel level)
    : name_(std::move(name)), saved_game_level_(level) {
}

UserData::~UserData() {}

void UserData::SetName(const std::string& name) {
    this->name_ = name;
}

void UserData::SaveGameLevel(const GameLevel level) {
    this->saved_game_level_ = level;
}

}  // namespace cli_tetris