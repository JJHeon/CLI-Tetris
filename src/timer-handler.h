/** Note
 * class for linux timer handling
 * -- 설명 --
 * TimerAccessor class :
 * TimerAccessor class는 key로 사용되어집니다.
 * TimerHnadler의 CreateTimer Method 를 통해 생성됩니다.
 * Timer가 동작중인지, 끝난 상태인지, 파괴된 상태인지를 확인할 수 있는 method를 제공합니다.
 *
 * TimerData class :
 * TimerHandler가 내부에서 timer_t를 wrrap해서 사용하는 class입니다.
 * 사용자는 이 class를 사용할 이유가 없습니다.
 *
 * TimerHandler class :
 * TimerAccessor를 이용해 Timer를 관리합니다. map을 이용해 내부에서 TimerData class를 관리합니다.
 * Timer의 생성, 시작, 중지를 수행할 수 있습니다.
 *
 * -- 변경 이력 -- (21.12.05 이전 기록 없음)
 * 21.12.05 TimerData class 위치 TimerHandler class 내부로 변경
 */

#ifndef CLI_TETRIS_TIMER_HANDLER_H_
#define CLI_TETRIS_TIMER_HANDLER_H_

#include <map>
#include <ctime>
#include <memory>

namespace cli_tetris::timer {

/* TimerAccessor Class ===================================================================================== */
/**
 * TimerHandler를 위한 key 로 사용됩니다.
 * TimerHandler의 CreateTimer method를 통해 생성됩니다.
 */
class TimerAccessor {
    friend class TimerHandler;

   private:
    const unsigned int id_;          // Handler로부터 할당받은 id
    bool* const is_running_;         // Timer 동작 여부
    std::weak_ptr<bool> is_allive_;  // Accessor의 수명 확인

   private:
    TimerAccessor(int id, bool* is_running_address);

   public:
    ~TimerAccessor();
    TimerAccessor(const TimerAccessor& obj);  //복사 생성자
    TimerAccessor(TimerAccessor&& obj) noexcept;
    TimerAccessor& operator=(const TimerAccessor& obj) = delete;  //일반 대입 생성자는 존재할 수 없다, 이 객체는 유일해야 하므로, 키를 복제할 수 있지만, 양도하거나 변경할 수 없다.
    TimerAccessor& operator=(TimerAccessor&& obj) = delete;       //대입 생성자는 존재할 수 없다.

    bool IsRunning() const;
    bool IsAlive() const;

    static bool WaitingTimer(const TimerAccessor& accessor);
};

inline bool TimerAccessor::WaitingTimer(const TimerAccessor& accessor) {
    if (accessor.IsAlive() && !accessor.IsRunning())
        return true;
    else
        return false;
}

/* TimerHandler Class ===================================================================================== */
class TimerHandler {
   private:
    class TimerData {
       public:
        std::shared_ptr<bool> accessor_allive_;
        timer_t timer_;

        TimerData(std::shared_ptr<bool> accessor_allive);
        TimerData(const TimerData& obj);
        TimerData(TimerData&& obj) noexcept;
        TimerData& operator=(const TimerData& obj);
    };

    static bool is_initialize_;            // 생성은 단 한개만 허용
    std::map<int, TimerData> timer_list_;  // TimderData의 id, 그에 해당하는 timer id

   public:
    TimerHandler();
    ~TimerHandler();
    TimerAccessor CreateTimer();
    void DeleteTimer(TimerAccessor& accessor);
    void SetTimer(TimerAccessor& accessor, const int& sec, const int& nanosec);
    void StopTimer(TimerAccessor& accessor);
    void DisableTimer(TimerAccessor& accessor);
    void EnableTimer(TimerAccessor& accessor);
};

}  // namespace cli_tetris::timer

#endif  // CLI_TETRIS_CUSTOM_TIMER_H_
