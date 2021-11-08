#include "object-defined.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

extern "C" {
#include <ncurses.h>
}

namespace cli_tetris {
/* Object Class ===================================================================================== */
Object::Object(int pos_y, int pos_x)
    : is_changed(true) {
    start_pos_.line = pos_y;
    start_pos_.column = pos_x;
}

Object::Object(LineColumn start_pos)
    : start_pos_(start_pos), is_changed(true) {}

LineColumn Object::getObjectPos() const {
    return start_pos_;
}

void Object::setObjectPos(int pos_y, int pos_x) {
    start_pos_.line = pos_y;
    start_pos_.column = pos_x;
}

bool Object::IsChanged() const {
    return is_changed;
}

void Object::setIsChanged(bool changed) {
    is_changed = changed;
}

/* FramePerSecond Class ===================================================================================== */
FramePerSecondUI::FramePerSecondUI(LineColumn start_pos)
    : Object(start_pos) {
    // win_ = newwin(size_.line - 1, size_.column - 1, start_pos_.line, start_pos_.column);
    win_ = newwin(size_.line, size_.column, start_pos_.line, start_pos_.column);
}
FramePerSecondUI::FramePerSecondUI(int start_y, int start_x)
    : Object(start_y, start_x) {
    // win_ = newwin(size_.line -1, size_.column - 1, start_y, start_x);
    win_ = newwin(size_.line, size_.column, start_y, start_x);
}

FramePerSecondUI::~FramePerSecondUI() {
    delwin(win_);
}

void FramePerSecondUI::UpdatePhysics() {
    // nothing
}
void FramePerSecondUI::UpdateRendering() {
    std::time_t t = std::chrono::high_resolution_clock::to_time_t(present_);

    // Draw
    mvwprintw(win_, 0, 0, "Sec : %lld", static_cast<int64_t>(t));

    // Last excution
    wrefresh(win_);

    // necessary
    this->setIsChanged(false);
}
void FramePerSecondUI::UpdateCurrentTime(std::chrono::time_point<std::chrono::high_resolution_clock>* present) {
    present_ = *present;
    this->setIsChanged(true);
}

void FramePerSecondUI::UpdateDifferTime(std::chrono::duration<int64_t, std::nano>* diff) {
    diff_ = *diff;
    this->setIsChanged(true);
}

/* StandbyUI Class ===================================================================================== */
StandbyUI::StandbyUI(LineColumn start_pos)
    : Object(start_pos) {
    // win_ = newwin(size_.line - 1, size_.column - 1, start_pos_.line, start_pos_.column);
    win_ = newwin(size_.line, size_.column, start_pos_.line, start_pos_.column);
}
StandbyUI::StandbyUI(int start_y, int start_x)
    : Object(start_y, start_x) {
    // win_ = newwin(size_.line -1, size_.column - 1, start_y, start_x);
    win_ = newwin(size_.line, size_.column, start_y, start_x);
}

StandbyUI::~StandbyUI() {
    delwin(win_);
}

void StandbyUI::UpdatePhysics() {
    // nothing

    // necessary
    this->setIsChanged(true);
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

    attron(A_BOLD);
    mvwprintw(win_, hello_message_y, hello_message_x, hello_message);
    box(win_, 0, 0);
    attroff(A_BOLD);

    // Last excution
    wrefresh(win_);

    // necessary.
    this->setIsChanged(false);
}
/* ExitUI Class ===================================================================================== */
ExitUI::ExitUI(LineColumn start_pos) : StandbyUI(start_pos) {
    win_ = newwin(size_.line, size_.column, start_pos_.line, start_pos_.column);
}
ExitUI::ExitUI(int start_y, int start_x) : StandbyUI(start_y, start_x) {
    win_ = newwin(size_.line, size_.column, start_y, start_x);
}

ExitUI::~ExitUI() {
    delwin(win_);
}

void ExitUI::UpdatePhysics() {
    // nothing

    // necessary
    this->setIsChanged(true);
}

void ExitUI::UpdateRendering() {
    const char* goodbye_message =
        "It's for development page.";
    int goodbye_message_y = 10;
    int goodbye_message_x = size_.column / 2;

    // Draw
    box(win_, 0, 0);
    attron(A_BOLD);
    mvwprintw(win_, goodbye_message_y, goodbye_message_x, goodbye_message);
    attroff(A_BOLD);

    // Last excution
    wrefresh(win_);

    // necessary.
    this->setIsChanged(false);
}

}  // namespace cli_tetris
