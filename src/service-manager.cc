#include "service-manager.h"

#include "ui.h"

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

}  // namespace cli_tetris