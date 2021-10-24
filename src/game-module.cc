#include <cstddef>

#include "game-module.h"

#include <chrono>

#include "ui.h"
#include "user-data.h"
#include "service-manager.h"

namespace cli_tetris {
/* GameState Class ===================================================================================== */

GameState::GameState(GameManager& supervisor, UserData& user_player, Ui& ui)
    : supervisor_(supervisor), player_(user_player), ui_(ui) {}

GameState::~GameState() {}

/* GameState - StartState Class ===================================================================================== */

StartState::StartState(GameManager& supervisor, UserData& user_player, Ui& ui)
    : GameState(supervisor, user_player, ui) {}

void StartState::MoveStateHandler(StateCode where) {
    supervisor_.ChangeSelcet(where);
}

void StartState::Initialize() {
    ready_milliseconds = 10000;
}

InputProcessResult StartState::InputProcess() {
    //아무것도 입력받지 않습니다.
    return InputProcessResult::kNothing;
}

void StartState::UpdateProcess(std::chrono::duration<int64_t, std::nano> diff) {
    auto n = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    if (n < 0) return;
    ready_milliseconds -= n;

    // ready_miliseconds 만큼 대기 후, MenuState로 이동.
    if (ready_milliseconds < 0) MoveStateHandler(StateCode::kMenu);
}

void StartState::RenderProcess() {
}

/* GameManager Class ===================================================================================== */
GameManager::GameManager(Ui* ui_driver, int select_state)
    : ui_(ui_driver), select_state_(select_state) {
}

// TODO: exception condtion's needed
GameManager::~GameManager() {
    for (int i = 0; i < sizeof(game_state_) / sizeof(std::unique_ptr<GameState>); ++i) game_state_->release();
}

void GameManager::ChangeSelcet(StateCode where) {
    select_state_ = static_cast<int>(where);
}

bool GameManager::CheckGameState() const {
    for (int i = 0; i < sizeof(game_state_) / sizeof(std::unique_ptr<GameState>); ++i) {
        if (game_state_[i] == nullptr) return false;
    }

    return true;
}

bool GameManager::CheckScreenSize(LineColumn& screen_size) {
    if (screen_size.line < game_size_.line || screen_size.column < game_size_.column)
        return false;
    else
        return true;
}

void GameManager::LoadPreviousUserData() {
}

void GameManager::Initialize() {
    // Ui driver는 생성자 단계에서 받습니다.
    if (ui_ == nullptr) throw std::runtime_error(std::string("E001 : UI Driver 없음"));

    // GameState Initalizing
    for (int i = 0; i < sizeof(game_state_) / sizeof(std::unique_ptr<GameState>); ++i) game_state_[i] = nullptr;  // std::move(nullptr);
    // GameState는 GameManager가 소유합니다.
    game_state_[0] = std::make_unique<StartState>(*this, *(player_.get()), *(ui_));
    // game_state_[1] = std::make_unique<EndState>(*this, *(player_.get()), *(ui_));
    // game_state_[2] = std::make_unique<MenuState>(*this, *(player_.get()), *(ui_));
    // game_state_[3] = std::make_unique<TemperaryStopState>(*this, *(player_.get()), *(ui_));
    // game_state_[4] = std::make_unique<SoloPlayState>(*this, *(player_.get()), *(ui_));
    // game_state_[5] = std::make_unique<DuoPlayState>(*this, *(player_.get()), *(ui_));
    // game_state_[6] = std::make_unique<MultiPlayState>(*this, *(player_.get()), *(ui_));
    // game_state_[7] = std::make_unique<SettingState>(*this, *(player_.get()), *(ui_));
    // game_state_[8] = std::make_unique<CreditState>(*this, *(player_.get()), *(ui_));

    // 생성 Check
    if (CheckGameState()) throw std::runtime_error(std::string("E002 : GameState isn't Loaded"));

    // 각 GameState Initializing
    // for (auto n : game_state_)
    for (int i = 0; i < sizeof(game_state_) / sizeof(std::unique_ptr<GameState>); ++i)
        game_state_[i]->Initialize();

    /** 화면 크기 Check, 게임 실행에 필요한 크기는 다음과 같습니다.
     *  Line   : 46
     *  Column : 160
     */
    LineColumn screen_size;
    screen_size = ui_->getScreenMaxSize();
    if (!CheckScreenSize(screen_size)) throw std::runtime_error(std::string("E003 : Terminal 크기 부족"));

    /*TODO: Sound 등록 필요 */
}

void GameManager::Run() {
    std::chrono::time_point<std::chrono::high_resolution_clock> past = std::chrono::time_point<std::chrono::high_resolution_clock>::max();
    while (true) {
        InputProcessResult n = kNothing;

        std::chrono::time_point<std::chrono::high_resolution_clock> present = std::chrono::high_resolution_clock::now();

        if ((n = game_state_[select_state_]->InputProcess()) == InputProcessResult::kNothing) {
            auto diff = present - past;
            game_state_[select_state_]->UpdateProcess(diff);
            game_state_[select_state_]->RenderProcess();
        } else {
            switch (n) {
                case kChangeState:
                    game_state_[select_state_]->Initialize();
                    break;
                case kExit:
                    return;
                    break;

                default:
                    throw std::runtime_error(std::string("E004 : 알 수 없는 입력"));
                    break;
            }
        }
        past = present;
    }
}

}  // namespace cli_tetris