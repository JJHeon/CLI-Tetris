#include <cstddef>

#include "game-module.h"

namespace cli_tetris {
/* GameState Class ===================================================================================== */
GameState::GameState()
    : user_player_(nullptr), supervisor_(nullptr) {}

GameState::GameState(GameManager* supervisor, UserData* user_player = nullptr)
    : supervisor_(supervisor) {}

GameState::~GameState() {
    if (user_player_ != nullptr) delete user_player_;
    if (supervisor_ != nullptr) delete supervisor_;
}

bool GameState::CheckUserPlayer() const {
    if (user_player_ != nullptr) return true;
    return false;
}

bool GameState::CheckSupervisor() const {
    if (supervisor_ != nullptr) return true;
    return false;
}

StartState::StartState(GameManager* supervisor)
    : GameState(supervisor) {}

void StartState::MoveStateHandler(){

}

void StartState::Initialize(GameManager* supervisor) {
}

InputProcessResult StartState::InputProcess(){

}

void StartState::UpdateProcess(){

}

void StartState::RenderProcess(){
    
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

bool GameManager::CheckGameState() const {
    for (int i = 0; i != (sizeof(game_state_) / sizeof(GameState*)); ++i) {
        if (game_state_[i] == nullptr) return false;
    }

    return true;
}

void GameManager::Run() {
    //시작전 Data 초기화
    for (int i = 0; i != (sizeof(game_state_) / sizeof(GameState*)); ++i) {
        game_state_[i]->Initialize(this);
    }

    while (true) {
        InputProcessResult n = kNothing;
        if ((n = game_state_[select_state_]->InputProcess()) == InputProcessResult::kNothing) {
            game_state_[select_state_]->UpdateProcess();
            game_state_[select_state_]->RenderProcess();
        } else {
            switch (n) {
                case kChangeState:
                    game_state_[select_state_]->Initialize(this);
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