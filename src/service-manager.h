#ifndef CLI_TETRIS_SERVICE_MANAGER_H_
#define CLI_TETRIS_SERVICE_MANAGER_H_

#include "ui-handler.h"
#include "timer-handler.h"

/** UI, Sound, Log, Random 기능을 위한 Service Mediator Pattern
 *  UI를 참조해야할 곳이 GameState 9개 정도 되는데, 추후에 각기 다른 UI를 등록할 수 있도록 하고자 한다.
 *  Service Mediator Pattern을 이용해서 UI를 각 GameState에 전달한다.
 *  주로 StateState의 Initialize 단계에서 각 Ui를 등록하게 한다.
 */
namespace cli_tetris {
class Locator final {
    // ui
   private:
    static UiHandler* ui_handler_;

   public:
    static UiHandler* getUiHandler();
    static void provideUiHandler(UiHandler* ui_service);
    static void releaseUiHandler();

    // System timer
   private:
    static timer::TimerHandler* timer_handler_;

   public:
    static timer::TimerHandler* getTimerHandler();
    static void provideTimerHandler(timer::TimerHandler* timer_handler);
    static void releaseTimerHandler();
};
}  // namespace cli_tetris

#endif  // CLI_TETRIS_SERVICE_MANAGER_H_
