#ifndef CLI_TETRIS_GAME_MODULE_H_
#define CLI_TETRIS_GAME_MODULE_H_

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
    /** CAUTION: (Only) StartState에게 Privilege 부여
     *  GameManager class 설명을 참고
     */
    friend class StartState;

   protected:
    GameManager& supervisor_;  //GameState를 실행한(관리하는) Manager, 일반적으로 MoveState를 위해 사용합니다.
    UserData* user_player_;    //게임을 실행한 User
    Ui* ui_;

   protected:
    GameState(GameManager& supervisor, UserData* user_player = nullptr, Ui* ui = nullptr);

    bool CheckUserPlayer() const;
    bool CheckSupervisor() const;

   protected:
    virtual void MoveStateHandler(StateCode where) = 0;

   public:
    virtual ~GameState();
    virtual void Initialize() = 0;
    virtual InputProcessResult InputProcess() = 0;
    virtual void UpdateProcess() = 0;
    virtual void RenderProcess() = 0;
};

/** 게임의 시작단계에서 Device component 등록 및 UserData loading을 담당합니다. 
 *  StartState는 base class인 GameState에 대해 friend 권한을 가지고 있습니다.
 *  GameManager에 등록된 다른 State에 대해 ptr 또는 참조, 값을 변경할 수 있습니다. (Value Change Privilege)
*/
class StartState : public GameState {
   private:
    void LoadPreviousUserData();  //TODO: File system 관련 예외처리 필요.

    //GameManager에게 요청한 주소를 통해 Ui를 등록합니다.
    void UpdateUi();

   protected:
    void MoveStateHandler(StateCode where) override;

   public:
    StartState(GameManager& supervisor);

    void Initialize() override;
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
 *  main()에서 단 하나의 객체만 생성되며 CheckGameState() 이후 Run() 됩니다.
 *  Run() 시작 단계에서 game_state_에 nullptr가 존재하면 안됩니다. 
 *  Initialize는 반드시 의도적으로 최초 한번은 실행되어야 합니다. (virtual method라서 생성자에서 호출할 수 없었습니다.)
 *  등록된 GameState들은 오직 GameManager의 허락된(public 된) 기능만을 사용할 수 있습니다.
 *  
 *  NOTE:
 *  등록된 game_state_ 간의 데이터 이동 및 ptr 변경은 불가능합니다. (생성자를 통해 참조된 ptr를 제외하고)
 *  getGameState Method만이 다른 game_state_ instance에 access 가능합니다. 따라서 매우 주의해서 사용하세요.
 *  현재 getGameState는 초기화를 담당하는 (ONLY) StartState이 사용합니다.
 *  StartState는 base class인 GameState에 대해 friend 권한을 가지고 있습니다.
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

    void ChangeSelcet(StateCode where);

    //CAUTION: (Ony StartState can use) 해당 method는 game_state_ 간 데이터 이동 및 등록을 가능하도록 합니다. 의도되지 않은 기능이므로 주의해서 사용하세요
    GameState* getGameState(StateCode where) const;
    bool CheckGameState() const;
    void Run();
};

}  //namespace cli_tetris

#endif  //CLI_TETRIS_GAME_MODULE_H_