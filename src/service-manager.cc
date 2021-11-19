#include "service-manager.h"

#include "ui-handler.h"
#include "timer-handler.h"

namespace cli_tetris {
/* UiHandler  ===================================================================================== */

UiHandler* Locator::ui_handler_ = nullptr;

UiHandler* Locator::getUiHandler() {
    return ui_handler_;
}
void Locator::provideUiHandler(UiHandler* ui_handler) {
    ui_handler_ = ui_handler;
}

void Locator::releaseUiHandler() {
    delete ui_handler_;
}
/* TimerHandler ===================================================================================== */

timer::TimerHandler* Locator::timer_handler_ = nullptr;

timer::TimerHandler* Locator::getTimerHandler() {
    return timer_handler_;
}
void Locator::provideTimerHandler(timer::TimerHandler* timer_handler) {
    timer_handler_ = timer_handler;
}
void Locator::releaseTimerHandler() {
    delete timer_handler_;
}

/* RandomValueHandler ===================================================================================== */
random::RandomValueHandler* Locator::random_value_handler_ = nullptr;

random::RandomValueHandler* Locator::getRandomValueHandler() {
    return random_value_handler_;
}
void Locator::provideRandomValueHandler(random::RandomValueHandler* random_value_handler) {
    random_value_handler_ = random_value_handler;
}
void Locator::releaseRandomValueHandler() {
    delete random_value_handler_;
}

}  // namespace cli_tetris