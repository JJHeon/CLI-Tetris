#ifndef CLI_TETRIS_GAME_MODULE_H_
#define CLI_TETRIS_GAME_MODULE_H_

#include <memory>

#include "user-data.h"
#include "ui.h"

namespace cli_tetris {

class GameManager;

/* GameState Class ===================================================================================== */

/* GameState의 식별자입니다. */
using StateCode = enum StateCode {
    kStart = 0,
    kEnd,
    kMenu,
    kTemperaryStop,
    kSoloPlay,
    kDuoPlay,
    kMultiPlay,
    kSetting,
    kCredit
};

/** GameState::InputProcess() 의 return 값을 식별하기 위한 enum입니다. */
using InputProcessResult = enum InputProcessResult {
    kNothing = -3,
    kChangeState = -2,
    kExit
};

/** GameState Class는 게임의 전체 Logic을 책임집니다.
 *  Derived Class들은 각 세부플레이의 형태를 상세하게 정의합니다.
 *  InputProcess() 내부에서 MoveStateHandler를 호출합니다. 호출 후, kChangeState를 retrun해 GameManager에게 State를 변경하라고 알립니다.
 *  MoveStateHandler는 GameManager supervisor를 이용해서 GameManager의 select_state_를 변경합니다.
 *  MoveStateHandler는 GameState 내부에서만 동작해야합니다.
 */
class GameState {
   protected:
    GameManager& supervisor_;  // GameState를 실행한(관리하는) Manager, 일반적으로 MoveState를 위해 사용합니다.
    UserData& player_;         //게임을 실행한 User
    Ui& ui_;

   protected:
    GameState(GameManager& supervisor, UserData& user_player, Ui& ui);

   protected:
    virtual void MoveStateHandler(StateCode where) = 0;

   public:
    virtual ~GameState();
    virtual void Initialize() = 0;
    virtual InputProcessResult InputProcess() = 0;
    virtual void UpdateProcess() = 0;
    virtual void RenderProcess() = 0;
};

/** 게임의 시작단계에서 Device component 등록 및 UserData loading을 담당합니다. */
class StartState : public GameState {
   private:
   protected:
    void MoveStateHandler(StateCode where) override;

   public:
    StartState(GameManager& supervisor, UserData& user_player, Ui& ui);

    void Initialize() override;
    InputProcessResult InputProcess() override;
    void UpdateProcess() override;
    void RenderProcess() override;
};
/* TODO: 미구현, 추후 구현 예정
// class EndState : public GameState {
//    protected:
//     void MoveStateHandler(StateCode where) override;

//    public:
//     EndState(GameManager& supervisor, UserData& user_player, Ui& ui);
//     void Initialize() override;
//     InputProcessResult InputProcess() override;
//     void UpdateProcess() override;
//     void RenderProcess() override;
// };
// class MenuState : public GameState {
//    protected:
//     void MoveStateHandler(StateCode where) override;

//    public:
//     MenuState(GameManager& supervisor, UserData& user_player, Ui& ui);
//     void Initialize() override;
//     InputProcessResult InputProcess() override;
//     void UpdateProcess() override;
//     void RenderProcess() override;
// };
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
// class SoloPlayState : public GameState {
//    protected:
//     void MoveStateHandler(StateCode where) override;

//    public:
//     SoloPlayState(GameManager& supervisor, UserData& user_player, Ui& ui);
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
    std::unique_ptr<GameState> game_state_[9];
    int select_state_;

   private:
    // GameManager는 driver 정보를 관리합니다.
    Ui* const ui_;
    // GameManager는 접속한 Player의 정보를 관리합니다.
    std::unique_ptr<UserData> player_;

    /* 게임 실행에 필요한 console screen의 size입니다. */
    static constexpr LineColumn game_size_ = {.line = 46, .column = 160};

   public:
    GameManager(Ui* ui_driver, int select_state = StateCode::kStart);
    ~GameManager();

    // Game Run
    void Initialize();
    void Run();

    // State
    void ChangeSelcet(StateCode where);
    bool CheckGameState() const;

    // Check
    bool CheckScreenSize(LineColumn& screen_size);

    // Load
    void LoadPreviousUserData();  // TODO: File system 관련 예외처리 필요.
};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_GAME_MODULE_H_