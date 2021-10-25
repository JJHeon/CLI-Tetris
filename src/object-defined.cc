#include "object-defined.h"

#include <ncurses.h>

namespace cli_tetris {
/* Object Class ===================================================================================== */
Object::Object(int pos_y, int pos_x)
    : is_changed(false) {
    start_pos_.line = pos_y;
    start_pos_.column = pos_x;
}

Object::Object(LineColumn start_pos)
    : start_pos_(start_pos), is_changed(false) {}

LineColumn Object::getObjectPos() const {
    return start_pos_;
}

void Object::setObjectPos(int pos_y, int pos_x) {
    start_pos_.line = pos_y;
    start_pos_.column = pos_x;
}
/* StandbyUI Class ===================================================================================== */
StandbyUI::StandbyUI(LineColumn start_pos)
    : Object(start_pos) {
    win_ = newwin(size_.line, size_.column, start_pos_.line, start_pos_.column);
}

StandbyUI::~StandbyUI() {
    delwin(win_);
}

void StandbyUI::UpdatePhysics() {
    // nothing
}
void StandbyUI::UpdateRendering() {
    const char* hello_message =
        "Hi Thank you for playing my game.\n\
    This tetris is made for improving my coding skill\n\
    so if you want to look at the code. please visit www.github.com/heonjj/CLI-Tetris\n\
    I will grad if you take a look my code and comment(or criticize)\n\n\
    Anyway!, I made this almost two month.\n\
    So happily play game.";
    int hello_message_y = 10;
    int hello_message_x = size_.column / 2;

    // Draw
    box(win_, 0, 0);
    attron(A_BOLD);
    mvwprintw(win_, hello_message_y, hello_message_x, hello_message);
    attroff(A_BOLD);

    // Last excution
    wrefresh(win_);
}
}  // namespace cli_tetris
