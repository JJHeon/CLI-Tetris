#ifndef CLI_TETRIS_GAME_MODULE_H_
#define CLI_TETRIS_GAME_MODULE_H_

#include <memory>
#include <chrono>

#include "user-data.h"
#include "ui-handler.h"
#include "timer-handler.h"
#include "random-generate-handler.h"
// #include "input-layer.h"

namespace cli_tetris {
using namespace timer;

class GameManager;

/* GameState Class ===================================================================================== */

/* GameState의 식별자입니다. */
using StateCode = enum StateCode {
    kStart = 0,
    kEnd,
    kMenu,
    kSoloPlay,
    kTemperaryStop,
    kDuoPlay,
    kMultiPlay,
    kSetting,
    kCredit
};

/** GameState::Process() 의 return 값을 식별하기 위한 enum입니다. */
using ProcessResult = enum ProcessResult {
    kNothing = -3,
    kChangeState = -2,
    kOut = -1,
    kExit = 0
};

/** GameState Class는 게임의 전체 Logic을 책임집니다.
 *  Derived Class들은 각 세부플레이의 형태를 상세하게 정의합니다.
 *  InputProcess() 내부에서 MoveStateHandler를 호출합니다. 호출 후, kChangeState를 retrun해 GameManager에게 State를 변경하라고 알립니다.
 *  MoveStateHandler는 GameManager supervisor를 이용해서 GameManager의 select_state_를 변경합니다.
 *  MoveStateHandler는 GameState 내부에서만 동작해야합니다.
 *  GameManager로 부터 Handler와 UserData를 받습니다.
 */
class GameState {
   protected:
    GameManager& supervisor_;  // GameState를 실행한(관리하는) Manager, 일반적으로 MoveState를 위해 사용합니다.
    UserData& player_;         //게임을 실행한 User
    UiHandler& ui_;
    TimerHandler& timer_;

   protected:
    GameState(GameManager& supervisor, UserData& user_player, UiHandler& ui, TimerHandler& timer);

   protected:
    virtual void MoveStateHandler(StateCode where) = 0;

   public:
    virtual ~GameState();

    /**
     *  최초 한번만 실행되는 State Value 설정 method
     *  생성자와 분리해서 정의한 이유는, 게임 설정에 따라 변수값이 가변적으로 적용될 수 있도록
     *  코드 부분에서 필수적인 요소들과 게임에 영향을 끼치는 value를 분리하기 위함입니다.
     */
    virtual void Initialize() = 0;

    // virtual ProcessResult InputProcess() = 0;
    // virtual ProcessResult UpdateProcess(std::chrono::duration<int64_t, std::nano> diff) = 0;
    virtual ProcessResult UpdateProcess() = 0;
    virtual void RenderProcess() = 0;
    virtual void EnterProcess() = 0;
    virtual void FinishProcess() = 0;
};
/* GameState - StartState Class ===================================================================================== */
/** 게임의 시작단계에서 Device component 등록 및 UserData loading을 담당합니다. */
class StartState : public GameState {
   private:
    int ready_milliseconds_;                    // StartState 대기 시간
    std::vector<TimerAccessor> accessor_list_;  // accessor list

   protected:
    std::vector<std::unique_ptr<Object>> ui_object_list_;  // Ui list

   protected:
    void MoveStateHandler(StateCode where) override;

   public:
    StartState(GameManager& supervisor, UserData& user_player, UiHandler& ui, TimerHandler& tiemr);

    void Initialize() override;
    // ProcessResult InputProcess() override;
    // ProcessResult UpdateProcess(std::chrono::duration<int64_t, std::nano> diff) override;
    ProcessResult UpdateProcess() override;
    void RenderProcess() override;
    void EnterProcess() override;
    void FinishProcess() override;
};
/* GameState - EndState Class ===================================================================================== */
class EndState : public GameState {
   private:
    int ready_milliseconds_;                    // EndState 대기 시간
    std::vector<TimerAccessor> accessor_list_;  // accessor list
   protected:
    std::vector<std::unique_ptr<Object>> ui_object_list_;  // Ui list

   protected:
    void MoveStateHandler(StateCode where) override;

   public:
    EndState(GameManager& supervisor, UserData& user_player, UiHandler& ui, TimerHandler& tiemr);

    void Initialize() override;
    // ProcessResult InputProcess() override;
    // ProcessResult UpdateProcess(std::chrono::duration<int64_t, std::nano> diff) override;
    ProcessResult UpdateProcess() override;
    void RenderProcess() override;
    void EnterProcess() override;
    void FinishProcess() override;
};
/* GameState - MenuState Class ===================================================================================== */
class MenuState : public GameState {
   protected:
    std::vector<std::unique_ptr<Object>> ui_object_list_;  // Ui list
                                                           // CommandQueue input_buffers_;
    using MenuItem = enum MenuItem {
        kKeepPlaying = 0,
        kCreateNewPlay = 1,
        kLoadPreviousPlay = 2,
        kPlayTogether = 3,
        kSeeBoard = 4,
        kExitPlay = 5
    };
    int current_select_;
    MenuUI* menu_accessor_;

   protected:
    void MoveStateHandler(StateCode where) override;

   public:
    MenuState(GameManager& supervisor, UserData& user_player, UiHandler& ui, TimerHandler& tiemr);

    void Initialize() override;
    // ProcessResult InputProcess() override;
    ProcessResult UpdateProcess() override;
    void RenderProcess() override;
    void EnterProcess() override;
    void FinishProcess() override;
};

/* GameState - SoloPlayState Class ===================================================================================== */

class SoloPlayState : public GameState {
   private:
    std::vector<TimerAccessor> accessor_list_;  // accessor list
    random::RandomValueHandler* random_generator_;

    bool start_standby_flag_;  // timer와 함께할 시작 대기 Flag

    TetrisBoardUI* tetris_board_;
    TopBoardUI* top_board_;
    ScoreBoardUI* score_board_;
    NextTetrisBoardUI* next_tetris_board_;
    LevelBoardUI* level_board_;
    InformBoardUI* inform_board_;

    std::array<std::array<int, 41>, 21>* block_board_;  // From TetrisBoardUI

    TetrisBlock* block_;  //내려오는 block.
    std::array<YX, 16> privious_block_shapes_; //내려오기 전에 저장한 위치.

   private:
    bool IsBlockAlive() const;

   protected:
    std::vector<std::unique_ptr<Object>> ui_object_list_;  // Ui list

   protected:
    void MoveStateHandler(StateCode where) override;

   public:
    SoloPlayState(GameManager& supervisor, UserData& user_player, UiHandler& ui, TimerHandler& tiemr);
    ~SoloPlayState();

    void Initialize() override;

    ProcessResult UpdateProcess() override;
    void RenderProcess() override;
    void EnterProcess() override;
    void FinishProcess() override;
};

/* TODO: 미구현, 추후 구현 예정
// class TemperaryStopState : public GameState {
//    protected:
//     void MoveStateHandler(StateCode where) override;

//    public:
//     TemperaryStopState(GameManager& supervisor, UserData& user_player, Ui& ui);
//     void Initialize() override;
//     InputProcessResult InputProcess() override;
//     void UpdateProcess() override;
//     void RenderProcess() override;
// };
// class DuoPlayState : public GameState {
//    protected:
//     void MoveStateHandler(StateCode where) override;

//    public:
//     DuoPlayState(GameManager& supervisor, UserData& user_player, Ui& ui);
//     void Initialize() override;
//     InputProcessResult InputProcess() override;
//     void UpdateProcess() override;
//     void RenderProcess() override;
// };
// class MultiPlayState : public GameState {
//    protected:
//     void MoveStateHandler(StateCode where) override;

//    public:
//     MultiPlayState(GameManager& supervisor, UserData& user_player, Ui& ui);
//     void Initialize() override;
//     InputProcessResult InputProcess() override;
//     void UpdateProcess() override;
//     void RenderProcess() override;
// };
// class SettingState : public GameState {
//    protected:
//     void MoveStateHandler(StateCode where) override;

//    public:
//     SettingState(GameManager& supervisor, UserData& user_player, Ui& ui);
//     void Initialize() override;
//     InputProcessResult InputProcess() override;
//     void UpdateProcess() override;
//     void RenderProcess() override;
// };
// class CreditState : public GameState {
//    protected:
//     void MoveStateHandler(StateCode where) override;

//    public:
//     CreditState(GameManager& supervisor, UserData& user_player, Ui& ui);
//     void Initialize() override;
//     InputProcessResult InputProcess() override;
//     void UpdateProcess() override;
//     void RenderProcess() override;
// };
/* GameManager Class ===================================================================================== */

/** GameManager는 GameState를 초기화하고 실행할 형태를 제공하는 Class입니다.
 *  Derviced된 GameState를 관리합니다.
 *  빠른 State 전환을 위해서 사전에 생성된 state들을 select_state_가 가르키는 형식으로 구성되있습니다.
 *  main()에서 단 하나의 객체만 생성되며 CheckGameState() 이후 Run() 됩니다.
 *  Run() 시작 단계에서 game_state_에 nullptr가 존재하면 안됩니다.
 *  Initialize는 반드시 의도적으로 최초 한번은 실행되어야 합니다. (virtual method라서 생성자에서 호출할 수 없었습니다.)
 *  등록된 GameState들은 오직 GameManager의 허락된(public 된) 기능만을 사용할 수 있습니다.

 */
class GameManager final {
   private:
    std::array<std::unique_ptr<GameState>, 9> game_state_;
    int select_state_;

   private:
    // GameManager는 driver 정보를 관리합니다.
    UiHandler* const ui_handler_;
    timer::TimerHandler* const timer_handler_;
    // GameManager는 접속한 Player의 정보를 관리합니다.
    std::unique_ptr<UserData> player_;

    /* 게임 실행에 필요한 console screen의 size입니다. */
    // constexpr static YX game_size_ = {.y = 46, .column = 160};
    constexpr static YX game_size_ = YX(46, 160);
    /* UI Object for frame time */
   public:  // TODO: Test
            // private: //Origianl
            // std::unique_ptr<FramePerSecondUI> frame_time_object_;
   public:
    GameManager(UiHandler* ui_driver, timer::TimerHandler* timer_handler, int select_state = StateCode::kStart);
    ~GameManager();

    // Game Run
    void ChangeSelcet(StateCode where);
    void Initialize();
    void Run();

    // State
    bool CheckGameState() const;
    // Check
    bool CheckScreenSize(YX& screen_size);
    // Load
    void LoadPreviousUserData();  // TODO: File system 관련 예외처리 필요.

    static YX getNeededScreenSize();

    // TestCode
    friend void GameManagerTestTimer(GameManager& G, std::chrono::duration<int64_t, std::nano> diff, std::chrono::time_point<std::chrono::high_resolution_clock> present, std::chrono::time_point<std::chrono::high_resolution_clock> past);
};

// TestCode
void GameManagerTestCode(void);
void GameManagerTestThreadManager(void);
void GameManagerTestThread(int* start_t);
void GameManagerTestTimer(GameManager& G, std::chrono::duration<int64_t, std::nano> diff, std::chrono::time_point<std::chrono::high_resolution_clock> present, std::chrono::time_point<std::chrono::high_resolution_clock> past);
}  // namespace cli_tetris

#endif  // CLI_TETRIS_GAME_MODULE_H_
