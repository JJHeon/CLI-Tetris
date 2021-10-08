#ifndef CLI_TETRIS_GAME_MODULE_H_
#define CLI_TETRIS_GAME_MODULE_H_

#include "user-data.h"

namespace cli_tetris {

/* GameState Class ===================================================================================== */

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
   private:
   protected:
    UserData* user_player_;    //게임을 실행한 User
    GameManager* supervisor_;  //GameState를 실행한(관리하는) Manager

   protected:
    GameState();
    GameState(GameManager* supervisor, UserData* user_player = nullptr);

    bool CheckUserPlayer() const;
    bool CheckSupervisor() const;

   protected:
    virtual void MoveStateHandler() = 0;

   public:
    virtual ~GameState();
    virtual void Initialize(GameManager* supervisor) = 0;
    virtual InputProcessResult InputProcess() = 0;
    virtual void UpdateProcess() = 0;
    virtual void RenderProcess() = 0;
};

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

/* 게임의 시작단계에서 화면 불러오기 및 UserData loading을 담당합니다. */
class StartState : public GameState {
   private:
    void LoadPreviousUserData();  //TODO: File system 관련 예외처리 필요.

   protected:
    void MoveStateHandler() override;

   public:
    StartState(GameManager* supervisor);

    void Initialize(GameManager* supervisor) override;
    InputProcessResult InputProcess() override;
    void UpdateProcess() override;
    void RenderProcess() override;
};
class EndState : public GameState {
};
class MenuState : public GameState {
};
class TemperaryStopState : public GameState {
};
class SoloPlayState : public GameState {
};
class DuoPlayState : public GameState {
};
class MultiPlayState : public GameState {
};
class SettingState : public GameState {
};
class CreditState : public GameState {
};
/* GameManager Class ===================================================================================== */

/** GameManager는 GameState를 초기화하고 실행할 형태를 제공하는 Class입니다. 
 *  Derviced된 GameState를 관리합니다.
 *  빠른 State 전환을 위해서 사전에 생성된 state들을 select_state_가 가르키는 형식으로 구성되있습니다.
 *  main()에서 단 하나만 선언되며 CheckGameState() 이후 Run() 됩니다.
 *  Run() 시작 단계에서 game_state_에 nullptr가 존재하면 안됩니다. 
 */
class GameManager final {
   private:
    GameState* game_state_[9];
    int select_state_;

   public:
    GameManager(int select_state = StateCode::kStart,
                StartState* start = nullptr,
                EndState* end = nullptr,
                MenuState* menu = nullptr,
                TemperaryStopState* temperary_stop = nullptr,
                SoloPlayState* solo_play = nullptr,
                DuoPlayState* duo_play = nullptr,
                MultiPlayState* multi_play = nullptr,
                SettingState* setting = nullptr,
                CreditState* credit = nullptr);
    ~GameManager();

    void ChangeSelcet();
    void UpdateAll(UserData& user_data);
    bool CheckGameState() const;
    void Run();
};

}  //namespace cli_tetris

#endif  //CLI_TETRIS_GAME_MODULE_H_