#ifndef CLI_TETRIS_TIMER_HANDLER_H_
#define CLI_TETRIS_TIMER_HANDLER_H_

#include <map>
#include <ctime>
#include <memory>

namespace cli_tetris::timer {
/**
 * TimerHandler를 위한 key value로 사용됩니다.
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
};

class TimerData {
   public:
    std::shared_ptr<bool> accessor_allive_;
    timer_t timer_;

    TimerData(std::shared_ptr<bool> accessor_allive);
    TimerData(const TimerData& obj);
    TimerData(TimerData&& obj) noexcept;
    TimerData& operator=(const TimerData& obj);
};

class TimerHandler {
   private:
    static bool is_initialize_;            // 생성은 단 한개만 허용
    std::map<int, TimerData> timer_list_;  // TimderData의 id, 그에 해당하는 timer id

   public:
    TimerHandler();
    ~TimerHandler();
    TimerAccessor&& CreateTimer();
    void DeleteTimer(TimerAccessor& accessor);
    void SetTimer(TimerAccessor& accessor, const int& sec, const int& nanosec);
    void StopTimer(TimerAccessor& accessor);
};

}  // namespace cli_tetris::timer

#endif  // CLI_TETRIS_CUSTOM_TIMER_H_
