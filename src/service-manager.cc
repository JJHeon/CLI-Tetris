#include "service-manager.h"

#include "ui.h"
#include "timer-handler.h"

namespace cli_tetris {

Ui* Locator::ui_service_ = nullptr;

Ui* Locator::getUi() {
    return ui_service_;
}
void Locator::provideUi(Ui* ui_service) {
    ui_service_ = ui_service;
}

void Locator::releaseUi() {
    delete ui_service_;
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