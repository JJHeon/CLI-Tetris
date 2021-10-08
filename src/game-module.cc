#include <cstddef>

#include "game-module.h"
#include "ui.h"
#include "user-data.h"

namespace cli_tetris {
/* GameState Class ===================================================================================== */

GameState::GameState(GameManager& supervisor, UserData* user_player = nullptr, Ui* ui = nullptr)
    : supervisor_(supervisor) {}

GameState::~GameState() {
    if (user_player_ != nullptr) delete user_player_;
    if (ui_ != nullptr) delete ui_;
}

bool GameState::CheckUserPlayer() const {
    if (user_player_ != nullptr) return true;
    return false;
}

/* GameState - StartState Class ===================================================================================== */

StartState::StartState(GameManager& supervisor)
    : GameState(supervisor) {}

void StartState::MoveStateHandler(StateCode where) {
    supervisor_.ChangeSelcet(where);
}

//TODO: 예외처리 필요합니다. 프로그램 정지를 위한.
void StartState::Initialize() {
}

InputProcessResult StartState::InputProcess() {
}

void StartState::UpdateProcess() {
}

void StartState::RenderProcess() {
}

/* GameManager Class ===================================================================================== */
GameManager::GameManager(
    int select_state = StateCode::kStart,
    StartState* start = nullptr,
    EndState* end = nullptr,
    MenuState* menu = nullptr,
    TemperaryStopState* temperary_stop = nullptr,
    SoloPlayState* solo_play = nullptr,
    DuoPlayState* duo_play = nullptr,
    MultiPlayState* multi_play = nullptr,
    SettingState* setting = nullptr,
    CreditState* credit = nullptr)
    : select_state_(select_state), game_state_{start, end, menu, temperary_stop, solo_play, duo_play, multi_play, setting, credit} {}

//TODO: exception condtion's needed
GameManager::~GameManager() {
    for (int i = 0; i != (sizeof(game_state_) / sizeof(GameState*)); ++i) {
        if (game_state_[i] != nullptr) delete game_state_[i];
    }
}

void GameManager::ChangeSelcet(StateCode where) {
    select_state_ = static_cast<int>(where);
}

bool GameManager::CheckGameState() const {
    for (int i = 0; i != (sizeof(game_state_) / sizeof(GameState*)); ++i) {
        if (game_state_[i] == nullptr) return false;
    }

    return true;
}

void GameManager::Run() {
    //시작전 Data 초기화
    for (int i = 0; i != (sizeof(game_state_) / sizeof(GameState*)); ++i) {
        game_state_[i]->Initialize();
    }

    while (true) {
        InputProcessResult n = kNothing;
        if ((n = game_state_[select_state_]->InputProcess()) == InputProcessResult::kNothing) {
            game_state_[select_state_]->UpdateProcess();
            game_state_[select_state_]->RenderProcess();
        } else {
            switch (n) {
                case kChangeState:
                    game_state_[select_state_]->Initialize();
                    break;
                case kExit:
                    break;

                default:
                    break;
            }
        }
    }
}

}  //namespace cli_tetris