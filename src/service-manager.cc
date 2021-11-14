#include "service-manager.h"

#include "ui-handler.h"
#include "timer-handler.h"

namespace cli_tetris {

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

}  // namespace cli_tetris