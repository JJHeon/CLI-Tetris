#include "game-module.h"

#include <cstddef>
#include <chrono>
#include <thread>

#include "ui.h"
#include "user-data.h"
#include "service-manager.h"

extern "C" {
#include <ncurses.h>
#include <unistd.h>  // for development
}

namespace cli_tetris {
/* GameState Class ===================================================================================== */

GameState::GameState(GameManager& supervisor, UserData& user_player, Ui& ui)
    : supervisor_(supervisor), player_(user_player), ui_(ui) {}

GameState::~GameState() {}

/* GameState - StartState Class ===================================================================================== */

StartState::StartState(GameManager& supervisor, UserData& user_player, Ui& ui)
    : GameState(supervisor, user_player, ui) {}

void StartState::MoveStateHandler(StateCode where) {
    this->FinishProcess();
    supervisor_.ChangeSelcet(where);
}

void StartState::Initialize() {
    //대기시간 설정
    ready_milliseconds = 5000;
}

ProcessResult StartState::InputProcess() {
    //아무것도 입력받지 않습니다.
    return ProcessResult::kNothing;
}

ProcessResult StartState::UpdateProcess(std::chrono::duration<int64_t, std::nano> diff) {
    auto n = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    if (n < 0) return ProcessResult::kOut;
    ready_milliseconds -= n;

    // ready_miliseconds 만큼 대기 후, MenuState로 이동.
    if (ready_milliseconds < 0) {
        MoveStateHandler(StateCode::kEnd);
        return ProcessResult::kChangeState;
    }

    return ProcessResult::kNothing;
}

void StartState::RenderProcess() {
    for (auto itr = ui_object_list_.begin(); itr != ui_object_list_.end(); ++itr) {
        if (!(*itr)->IsChanged()) continue;

        ui_.Draw((*itr).get());
    }
}
void StartState::EnterProcess() {
    ui_.ClearScreen();

    // Drawing할 Ui object 등록
    ui_object_list_.push_back(std::make_unique<StandbyUI>(0, 0));

    // 최초에 한번 Draw 합니다.
    this->RenderProcess();
}
void StartState::FinishProcess() {
    // 할당한 Object를 모두 해제합니다.
    ui_object_list_.erase(ui_object_list_.begin(), ui_object_list_.end());
}

/* GameState - EndState Class ===================================================================================== */

EndState::EndState(GameManager& supervisor, UserData& user_player, Ui& ui)
    : GameState(supervisor, user_player, ui) {}

void EndState::MoveStateHandler(StateCode where) {
    this->FinishProcess();
    supervisor_.ChangeSelcet(where);
}

void EndState::Initialize() {
    //대기시간 설정
    ready_milliseconds = 10000;
}

ProcessResult EndState::InputProcess() {
    //아무것도 입력받지 않습니다.
    return ProcessResult::kNothing;
}

ProcessResult EndState::UpdateProcess(std::chrono::duration<int64_t, std::nano> diff) {
    auto n = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    if (n < 0) return ProcessResult::kOut;
    ready_milliseconds -= n;

    // ready_miliseconds 만큼 대기 후, Exit. Game 종료.
    if (ready_milliseconds < 0) {
        return ProcessResult::kExit;
    }

    return ProcessResult::kNothing;
}

void EndState::RenderProcess() {
    for (auto itr = ui_object_list_.begin(); itr != ui_object_list_.end(); ++itr) {
        if (!(*itr)->IsChanged()) continue;

        ui_.Draw((*itr).get());
    }
}
void EndState::EnterProcess() {
    ui_.ClearScreen();

    // Drawing할 Ui object 등록
    ui_object_list_.push_back(std::make_unique<ExitUI>(0, 0));

    // 최초에 한번 Draw 합니다.
    this->RenderProcess();
}
void EndState::FinishProcess() {
    // 할당한 Object를 모두 해제합니다.
    ui_object_list_.erase(ui_object_list_.begin(), ui_object_list_.end());
}

/* GameManager Class ===================================================================================== */
GameManager::GameManager(Ui* ui_driver, int select_state)
    : ui_(ui_driver), select_state_(select_state) {
}

// TODO: exception condtion's needed
GameManager::~GameManager() {
    for (auto i = game_state_.begin(); i != game_state_.end(); ++i) i->release();
}

void GameManager::ChangeSelcet(StateCode where) {
    select_state_ = static_cast<int>(where);
}

bool GameManager::CheckGameState() const {
    // for (int i = 0; i < game_state_.size(); ++i) { //TODO: Original
    for (int i = 0; i < 2; ++i) {  // TODO: 개발용 임시, EndState까지
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
    for (auto i = game_state_.begin(); i != game_state_.end(); ++i) (*i) = nullptr;  // std::move(nullptr);
    // GameState는 GameManager가 소유합니다.
    game_state_[0] = std::make_unique<StartState>(*this, *(player_.get()), *(ui_));
    game_state_[1] = std::make_unique<EndState>(*this, *(player_.get()), *(ui_));
    // game_state_[2] = std::make_unique<MenuState>(*this, *(player_.get()), *(ui_));
    // game_state_[3] = std::make_unique<TemperaryStopState>(*this, *(player_.get()), *(ui_));
    // game_state_[4] = std::make_unique<SoloPlayState>(*this, *(player_.get()), *(ui_));
    // game_state_[5] = std::make_unique<DuoPlayState>(*this, *(player_.get()), *(ui_));
    // game_state_[6] = std::make_unique<MultiPlayState>(*this, *(player_.get()), *(ui_));
    // game_state_[7] = std::make_unique<SettingState>(*this, *(player_.get()), *(ui_));
    // game_state_[8] = std::make_unique<CreditState>(*this, *(player_.get()), *(ui_));

    // 생성 Check
    if (!CheckGameState()) throw std::runtime_error(std::string("E002 : GameState isn't Loaded"));

    // 각 GameState Initializing
    // for (int i = 0; i < sizeof(game_state_) / sizeof(std::unique_ptr<GameState>); ++i) //Original
    for (int i = 0; i != 2; ++i)  // TODO: 임시.
        game_state_[i]->Initialize();

    /** 화면 크기 Check, 게임 실행에 필요한 크기는 다음과 같습니다.
     *  Line   : 46
     *  Column : 160
     */
    LineColumn screen_size;
    screen_size = ui_->getScreenMaxSize();
    if (!CheckScreenSize(screen_size)) throw std::runtime_error(std::string("E003 : Terminal 크기 부족"));

    /*TODO: Sound 등록 필요 */

    /* FrameTime Object 등록 */
    frame_time_object_ = std::make_unique<FramePerSecondUI>(game_size_.line - 2, 1);
}

void GameManager::Run() {
    // TestCode //TODO: 나중에 지울것
    /*
    GameManagerTestCode();
    GameManagerTestThreadManager();
    int test = 0;
    while (true) {
        test++;
    }
    */

    // game_state_.at(select_state_)->EnterProcess();

    std::chrono::time_point<std::chrono::high_resolution_clock> past = std::chrono::time_point<std::chrono::high_resolution_clock>::max();

    while (true) {
        ProcessResult n = kNothing;

        std::chrono::time_point<std::chrono::high_resolution_clock> present = std::chrono::high_resolution_clock::now();
        auto diff = present - past;
        past = present;

        GameManagerTestTimer(*this, diff, present, past);  // TestCode

        if ((n = game_state_.at(select_state_)->InputProcess()) == ProcessResult::kNothing) {
            // if ((n = game_state_.at(select_state_)->UpdateProcess(diff)) == ProcessResult::kNothing) {
            //      game_state_.at(select_state_)->RenderProcess();
            // }
        }

        switch (n) {
            case kNothing:
                // Normal Excution
                break;
            case kChangeState:
                game_state_.at(select_state_)->EnterProcess();
                break;
            case kOut:
                // Not error, but just away
                break;
            case kExit:
                return;  // finish(정상 종료)
                break;

            default:
                throw std::runtime_error(std::string("E004 : 알 수 없는 입력"));
                break;
        }
    }
}

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
    mvwprintw(win_, 0, 0, "%lld", k1);
    mvwprintw(win_, 1, 0, "%lld", k2);
    mvwprintw(win_, 2, 0, "%lld", k3);
    mvwprintw(win_, 3, 0, "%lld", k4);
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

}  // namespace cli_tetris