#include "ui.h"

#include <ncurses.h>

#include "object-defined.h"

namespace cli_tetris {

Ui::Ui()
    : is_initialized(false) {
    this->Initialize();
}

Ui::~Ui() {
    if (is_initialized) this->End();
}

void Ui::Initialize() {
    is_initialized = true;
    initscr();
    refresh();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
}

void Ui::End() {
    is_initialized = false;
    endwin();
}

LineColumn Ui::getScreenMaxSize() {
    LineColumn n = {0, 0};
    getmaxyx(stdscr, n.line, n.column);

    return n;
}

}  // namespace cli_tetris
