/** Note
 * service-manager
 *
 * Service Mediator Class입니다.
 * UI, Sound, Log, Random 기능을 위한 Service Mediator Pattern
 * UI를 참조해야할 곳이 GameState 9개 정도 되는데, 추후에 각기 다른 UI를 등록할 수 있도록 합니다.
 *
 */

#ifndef CLI_TETRIS_SERVICE_MANAGER_H_
#define CLI_TETRIS_SERVICE_MANAGER_H_

#include "ui-handler.h"
#include "timer-handler.h"
#include "random-generate-handler.h"

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

    // random number generator
   private:
    static random::RandomValueHandler* random_value_handler_;

   public:
    static random::RandomValueHandler* getRandomValueHandler();
    static void provideRandomValueHandler(random::RandomValueHandler* random_value_handler);
    static void releaseRandomValueHandler();
};
}  // namespace cli_tetris

#endif  // CLI_TETRIS_SERVICE_MANAGER_H_
