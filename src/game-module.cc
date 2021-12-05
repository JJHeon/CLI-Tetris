#include "game-module.h"

#include <cstddef>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

#include "ui-handler.h"
#include "user-data.h"
#include "timer-handler.h"
#include "service-manager.h"
#include "object-defined.h"
#include "random-generate-handler.h"

extern "C" {
#include <ncurses.h>
#include <unistd.h>  // for development
}

namespace cli_tetris {

using namespace object;

/* GameState Class ===================================================================================== */

GameState::GameState(GameManager& supervisor, UserData& user_player)
    : supervisor_(supervisor), player_(user_player) {}

GameState::~GameState() {}

/* GameState - StartState Class ===================================================================================== */

StartState::StartState(GameManager& supervisor, UserData& user_player)
    : GameState(supervisor, user_player) {
}

void StartState::MoveStateHandler(StateCode where) {
    this->FinishProcess();
    supervisor_.ChangeSelcet(where);
}

void StartState::Initialize() {
    // Get Handlers
    ui_handler_ = Locator::getUiHandler();
    timer_handler_ = Locator::getTimerHandler();

    // Get Timer accessor
    accessor_list_.push_back(timer_handler_->CreateTimer());
}

void StartState::EnterProcess() {
    ui_handler_->ClearScreen();

    // Timer 10초 설정.
    timer_handler_->SetTimer(accessor_list_.at(0), 1, 0);

    // Drawing할 Ui object 등록
    ui_object_list_.push_back(std::make_unique<StandbyPage>(ui_handler_->getCurrentScreenSize()));

    // 최초에 한번 Draw 합니다.
    this->RenderProcess();
}
ProcessResult StartState::UpdateProcess() {
    // timer 설정값 현재 10초 만큼 대기 후, MenuState로 이동.

    if (TimerAccessor::WaitingTimer(accessor_list_.at(0))) {
        MoveStateHandler(StateCode::kMenu);
        return ProcessResult::kChangeState;
    }

    return ProcessResult::kNothing;
}

void StartState::RenderProcess() {
    for (auto itr = ui_object_list_.begin(); itr != ui_object_list_.end(); ++itr) {
        if (!(*itr)->IsChanged()) continue;

        ui_handler_->Draw((*itr).get());
    }
}

void StartState::FinishProcess() {
    // 할당한 Object를 모두 해제합니다.
    ui_object_list_.erase(ui_object_list_.begin(), ui_object_list_.end());
}

/* GameState - EndState Class ===================================================================================== */

EndState::EndState(GameManager& supervisor, UserData& user_player)
    : GameState(supervisor, user_player) {}

void EndState::MoveStateHandler(StateCode where) {
    this->FinishProcess();
    supervisor_.ChangeSelcet(where);
}

void EndState::Initialize() {
    // Get Handlers
    ui_handler_ = Locator::getUiHandler();
    timer_handler_ = Locator::getTimerHandler();

    // Get Timer accessor
    accessor_list_.push_back(timer_handler_->CreateTimer());
}

void EndState::EnterProcess() {
    ui_handler_->ClearScreen();

    // Timer 5초 설정.
    timer_handler_->SetTimer(accessor_list_.at(0), 5, 0);

    // Drawing할 Ui object 등록
    ui_object_list_.push_back(std::make_unique<ExitPage>(ui_handler_->getCurrentScreenSize()));

    // 최초에 한번 Draw 합니다.
    this->RenderProcess();
}
ProcessResult EndState::UpdateProcess() {
    // timer 설정값 현재 5초 만큼 대기 후, Exit. Game 종료.
    if (accessor_list_.at(0).IsAlive() && !accessor_list_.at(0).IsRunning()) {
        return ProcessResult::kExit;
    }

    return ProcessResult::kNothing;
}
void EndState::RenderProcess() {
    for (auto itr = ui_object_list_.begin(); itr != ui_object_list_.end(); ++itr) {
        if (!(*itr)->IsChanged()) continue;

        ui_handler_->Draw((*itr).get());
    }
}

void EndState::FinishProcess() {
    // 할당한 Object를 모두 해제합니다.
    ui_object_list_.erase(ui_object_list_.begin(), ui_object_list_.end());
}

/* GameState - MenuState Class ===================================================================================== */

MenuState::MenuState(GameManager& supervisor, UserData& user_player)
    : GameState(supervisor, user_player), current_select_(kKeepPlaying) {}

void MenuState::MoveStateHandler(StateCode where) {
    this->FinishProcess();
    supervisor_.ChangeSelcet(where);
}

void MenuState::Initialize() {
    // Get Handlers
    ui_handler_ = Locator::getUiHandler();
}

void MenuState::EnterProcess() {
    ui_handler_->ClearScreen();

    // Drawing할 Ui object 등록
    ui_object_list_.push_back(std::make_unique<FrameObject46X160>(ui_handler_->getCurrentScreenSize()));
    ui_object_list_.push_back(std::make_unique<MenuObject>(ui_handler_->getCurrentScreenSize()));

    // 최초에 한번 Draw 합니다.
    this->RenderProcess();

    current_select_ = 0;
    auto object_ptr = ui_object_list_.at(1).get();
    menu_accessor_ = dynamic_cast<MenuObject*>(object_ptr);
}
ProcessResult MenuState::UpdateProcess() {
    // ncurse Input
    auto menu_ptr = menu_accessor_->GetMenuAccessor();
    int input = ui_handler_->getInput();
    switch (input) {
        case KEY_DOWN:
            ui_handler_->ControlMenuDriver(menu_ptr, MenuRequest::DOWN);
            if (current_select_ != MenuItem::kExitPlay) current_select_++;
            menu_accessor_->UpdateState();

            break;
        case KEY_UP:
            ui_handler_->ControlMenuDriver(menu_ptr, MenuRequest::UP);
            if (current_select_ != MenuItem::kKeepPlaying) current_select_--;
            menu_accessor_->UpdateState();
            break;
        case 10:  // Enter
        {
            switch (current_select_) {
                case MenuItem::kKeepPlaying:

                    MoveStateHandler(StateCode::kSoloPlay);
                    return ProcessResult::kChangeState;
                    break;
                case MenuItem::kCreateNewPlay:
                    break;
                case MenuItem::kLoadPreviousPlay:
                    break;
                case MenuItem::kPlayTogether:
                    break;
                case MenuItem::kSeeBoard:
                    break;
                case MenuItem::kExitPlay:
                    this->FinishProcess();
                    return ProcessResult::kExit;
                    break;
                default:
                    break;
            }
        } break;
        default:
            break;
    }

    return ProcessResult::kNothing;
}
void MenuState::RenderProcess() {
    for (auto itr = ui_object_list_.begin(); itr != ui_object_list_.end(); ++itr) {
        if (!(*itr)->IsChanged()) continue;

        ui_handler_->Draw((*itr).get());
    }
}

void MenuState::FinishProcess() {
    //  할당한 Object를 모두 해제합니다.
    ui_object_list_.erase(ui_object_list_.begin(), ui_object_list_.end());
}

/* GameState - SoloPlayState Class ===================================================================================== */
SoloPlayState::SoloPlayState(GameManager& supervisor, UserData& user_player)
    : GameState(supervisor, user_player) {
    board_object_ptr_ = nullptr;
}
SoloPlayState::~SoloPlayState() {
}

void SoloPlayState::MoveStateHandler(StateCode where) {
    this->FinishProcess();
    supervisor_.ChangeSelcet(where);
}

void SoloPlayState::Initialize() {
    // Get Handlers
    ui_handler_ = Locator::getUiHandler();
    timer_handler_ = Locator::getTimerHandler();
    random_generator_ = Locator::getRandomValueHandler();

    // Get Timer accessor
    accessor_list_.push_back(timer_handler_->CreateTimer());
    accessor_list_.push_back(timer_handler_->CreateTimer());
}

void SoloPlayState::EnterProcess() {
    ui_handler_->ClearScreen();

    // 시작 대기 Timer 5초 설정.
    timer_handler_->SetTimer(accessor_list_.at(0), 5, 0);

    YX game_screen_size = GameManager::getNeededScreenSize();
    YX current_screen_size = ui_handler_->getCurrentScreenSize();
    int relative_start_pos_y = ((current_screen_size.y - game_screen_size.y) / 2) + 1;
    int relative_start_pos_x = ((current_screen_size.x - game_screen_size.x) / 2) + 1;

    // Drawing할 Ui object 등록
    ui_object_list_.push_back(std::make_unique<FrameObject46X160>(ui_handler_->getCurrentScreenSize()));
    ui_object_list_.push_back(std::make_unique<TetrisBoard>(ui_handler_->getCurrentScreenSize(), YX(relative_start_pos_y + 1, relative_start_pos_x + 1)));
    ui_object_list_.push_back(std::make_unique<TopBoard>(ui_handler_->getCurrentScreenSize(), YX(relative_start_pos_y + 1, relative_start_pos_x + 47)));
    ui_object_list_.push_back(std::make_unique<ScoreBoard>(ui_handler_->getCurrentScreenSize(), YX(relative_start_pos_y + 7, relative_start_pos_x + 47)));
    ui_object_list_.push_back(std::make_unique<NextTetrisBoard>(ui_handler_->getCurrentScreenSize(), YX(relative_start_pos_y + 13, relative_start_pos_x + 47)));
    ui_object_list_.push_back(std::make_unique<LevelBoard>(ui_handler_->getCurrentScreenSize(), YX(relative_start_pos_y + 27, relative_start_pos_x + 47)));
    ui_object_list_.push_back(std::make_unique<InformBoard>(ui_handler_->getCurrentScreenSize(), YX(relative_start_pos_y + 33, relative_start_pos_x + 47)));

    // Get TetrisBoard Pointer
    board_object_ptr_ = dynamic_cast<TetrisBoard*>(ui_object_list_[1].get());
    // Connect UI and Engine
    board_object_ptr_->ConnectBoard(user_tetris_engine_.getTetrisBoard());

    //    최초에 한번 Draw 합니다.
    this->RenderProcess();
}
ProcessResult SoloPlayState::UpdateProcess() {
    return ProcessResult::kNothing;
}
void SoloPlayState::RenderProcess() {
    for (auto itr = ui_object_list_.begin(); itr != ui_object_list_.end(); ++itr) {
        if (!(*itr)->IsChanged()) continue;

        ui_handler_->Draw((*itr).get());
    }
}

void SoloPlayState::FinishProcess() {
    // 할당한 Object를 모두 해제합니다.
    ui_object_list_.erase(ui_object_list_.begin(), ui_object_list_.end());

    board_object_ptr_ = nullptr;
}

/* GameManager Class ===================================================================================== */
constexpr YX GameManager::game_size_;
GameManager::GameManager(StateCode select_state)
    : select_state_(select_state) {
}

// TODO: exception condtion's needed
GameManager::~GameManager() {
    for (auto i = game_state_.begin(); i != game_state_.end(); ++i) i->release();
}

void GameManager::ChangeSelcet(StateCode where) {
    select_state_ = where;
}

bool GameManager::CheckGameState() const {
    // for (int i = 0; i < game_state_.size(); ++i) { //Original
    for (int i = 0; i < 4; ++i) {  // TODO: 개발용 임시
        if (game_state_[i] == nullptr) return false;
    }

    return true;
}

bool GameManager::CheckScreenSize(YX& screen_size) {
    if (screen_size.y < game_size_.y || screen_size.x < game_size_.x)
        return false;
    else
        return true;
}

YX GameManager::getNeededScreenSize() {
    return GameManager::game_size_;
}

void GameManager::LoadPreviousUserData() {
}

void GameManager::Initialize() {
    // Get Handlers
    ui_handler_ = Locator::getUiHandler();

    // GameState Initalizing
    for (auto i = game_state_.begin(); i != game_state_.end(); ++i) (*i) = nullptr;

    // GameState는 GameManager가 소유합니다.
    game_state_[0] = std::make_unique<StartState>(*this, *(player_.get()));
    game_state_[1] = std::make_unique<EndState>(*this, *(player_.get()));
    game_state_[2] = std::make_unique<MenuState>(*this, *(player_.get()));
    game_state_[3] = std::make_unique<SoloPlayState>(*this, *(player_.get()));

    // 생성 Check
    if (!CheckGameState()) throw std::runtime_error(std::string("E002 : GameState isn't Loaded"));

    // 각 GameState Initializing
    // for (int i = 0; i < sizeof(game_state_) / sizeof(std::unique_ptr<GameState>); ++i) //Original
    for (int i = 0; i != 4; ++i)  // TODO: 임시.
        game_state_[i]->Initialize();

    /** 화면 크기 Check, 게임 실행에 필요한 크기는 다음과 같습니다.
     *  y : 46
     *  x : 160
     */
    YX screen_size;
    screen_size = ui_handler_->getCurrentScreenSize();
    if (!CheckScreenSize(screen_size)) throw std::runtime_error(std::string("E003 : Terminal 크기 부족"));

    /*TODO: Sound 등록 필요 */
}

void GameManager::Run() {
    game_state_.at(static_cast<int>(select_state_))->EnterProcess();

    while (true) {
        ProcessResult n = ProcessResult::kNothing;

        if ((n = game_state_.at(static_cast<int>(select_state_))->UpdateProcess()) == ProcessResult::kNothing) {
            game_state_.at(static_cast<int>(select_state_))->RenderProcess();
        }

        switch (n) {
            case ProcessResult::kNothing:
                // Normal Excution
                break;
            case ProcessResult::kChangeState:
                game_state_.at(static_cast<int>(select_state_))->EnterProcess();
                break;
            case ProcessResult::kOut:
                // Not error, but just away
                break;
            case ProcessResult::kExit:
                return;  // finish(정상 종료)
                break;

            default:
                throw std::runtime_error(std::string("E004 : 알 수 없는 입력"));
                break;
        }
    }
}

/* TestCode =========================================================================================== */
namespace GameModuleTest {
void GameManagerTestCode(void) {
    attrset(A_ITALIC);
    mvprintw(5, 0, "attrset ITALIC");
    refresh();
    sleep(5);
    attrset(A_NORMAL);
}

void GameManagerTestThreadManager(void) {
    int start_t1 = 6;
    int start_t2 = 14;
    std::thread t(GameManagerTestThread, &start_t1);
    std::thread t2(GameManagerTestThread, &start_t2);

    t.detach();
    t2.detach();
}

void GameManagerTestThread(int* start_t) {
    for (int i = *start_t; i != (*start_t) + 4; ++i) {
        mvprintw(i, 0, "%d gogo", i);
        refresh();
        sleep(3);
    }
}
void GameManagerTestTimer(GameManager& G, std::chrono::duration<int64_t, std::nano> diff, std::chrono::time_point<std::chrono::high_resolution_clock> present, std::chrono::time_point<std::chrono::high_resolution_clock> past) {
    static int64_t t = 5000;
    static WINDOW* win_ = newwin(10, 50, 30, 2);

    int64_t k1 = std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();
    int64_t k2 = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
    int64_t k3 = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    int64_t k4 = std::chrono::duration_cast<std::chrono::seconds>(diff).count();
    std::time_t time1 = std::chrono::high_resolution_clock::to_time_t(present);
    std::time_t time2 = std::chrono::high_resolution_clock::to_time_t(past);
    char* date1 = ctime(&time1);
    char* date2 = ctime(&time2);
    clear();
    mvwprintw(win_, 0, 0, "%d", k1);
    mvwprintw(win_, 1, 0, "%d", k2);
    mvwprintw(win_, 2, 0, "%d", k3);
    mvwprintw(win_, 3, 0, "%d", k4);
    mvwprintw(win_, 4, 0, "%s", date1);
    mvwprintw(win_, 5, 0, "%s", date2);
    mvwprintw(win_, 6, 0, "%lld", static_cast<int64_t>(time1));
    mvwprintw(win_, 7, 0, "%lld", static_cast<int64_t>(time2));

    wrefresh(win_);
    usleep(100);
    bool flag = false;
    if (!flag) {
        mvwprintw(win_, 9, 0, "%lld", k4);
        wrefresh(win_);
        flag = true;
    }
    // if (k < 0) return;
    // t -= k;
    // if (t < 0) {
    //     //   시간경과 표시 for develop
    //     // G.frame_time_object_->UpdateCurrentTime(&present);
    //     // if (G.frame_time_object_->IsChanged()) G.ui_->Draw(G.frame_time_object_.get());
    //     t = 5000;
    // }

    // mvwprintw(win_, 0, 0, "%ld %5ld", k, t);
    // wrefresh(win_);
    // usleep(1);
}
}  // namespace GameModuleTest
}  // namespace cli_tetris