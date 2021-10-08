#include "device-controller.h"

#include <ncurses.h>

namespace cli_tetris {

ConsoleDevice::ConsoleDevice()
    : is_initialized(true) {
    Initialize();
}
ConsoleDevice::~ConsoleDevice() {
    ReturnDevice();
}

void ConsoleDevice::Initialize() {
    is_initialized = true;
    initscr();
    refresh();
    noecho();
}

void ConsoleDevice::ReturnDevice() {
    is_initialized = false;
    endwin();
}

}  // namespace cli_tetris