#include "ui.h"

#include "device-controller.h"

namespace cli_tetris {

Ui::Ui(ConsoleDevice* device)
    : device_(device) {}

Ui::~Ui() {
    delete device_;
}

}  //namespace cli_tetris
