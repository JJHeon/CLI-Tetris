#include "service-manager.h"

namespace cli_tetris {

Ui* Locator::getUi() {
    return ui_service_;
}
void Locator::provideUi(Ui* ui_service) {
    ui_service_ = ui_service;
}

}  //namespace cli_tetris