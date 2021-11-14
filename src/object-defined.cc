#include "object-defined.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <exception>
#include <iostream>

extern "C" {
#include <ncurses.h>
}

namespace cli_tetris {
/* Object Class ===================================================================================== */

Object::Object(const YX& start_pos)
    : start_pos_(start_pos), is_changed(true) {}
Object::Object()
    : start_pos_(YX(0, 0)), is_changed(true) {}

YX Object::getObjectPos() const {
    return start_pos_;
}

void Object::setObjectPos(int pos_y, int pos_x) {
    start_pos_.y = pos_y;
    start_pos_.x = pos_x;
}

bool Object::IsChanged() const {
    return is_changed;
}

void Object::setIsChanged(bool changed) {
    is_changed = changed;
}

/* UI Class ===================================================================================== */
UI::UI(const YX& currnet_screen_size, const YX& start_pos, const YX& win_size)
    : Object(start_pos), currnet_screen_size_(currnet_screen_size), win_size_(win_size), win_(NULL) {
    // if (win_ == NULL) throw std::runtime_error(std::string("E010 : SetWindow 정의 안됨"));
}
UI::UI(const YX& currnet_screen_size)
    : Object(), currnet_screen_size_(currnet_screen_size), win_(NULL), win_size_(YX(0, 0)) {}

UI::~UI() {
    if (win_ != NULL) delwin(win_);
}

/* FramePerSecond Class ===================================================================================== */
// FramePerSecondUI::FramePerSecondUI(YX start_pos)
//     : Object(start_pos) {
//     // win_ = newwin(size_.y - 1, size_.x - 1, start_pos_.y, start_pos_.x);
//     win_ = newwin(size_.y, size_.x, start_pos_.y, start_pos_.x);
// }
// FramePerSecondUI::FramePerSecondUI(int start_y, int start_x)
//     : Object(start_y, start_x) {
//     // win_ = newwin(size_.y -1, size_.x - 1, start_y, start_x);
//     win_ = newwin(size_.y, size_.x, start_y, start_x);
// }

// FramePerSecondUI::~FramePerSecondUI() {
//     delwin(win_);
// }

// void FramePerSecondUI::UpdatePhysics() {
//     // nothing
// }
// void FramePerSecondUI::UpdateRendering() {
//     std::time_t t = std::chrono::high_resolution_clock::to_time_t(present_);

//     // Draw
//     mvwprintw(win_, 0, 0, "Sec : %lld", static_cast<int64_t>(t));

//     // Last excution
//     wrefresh(win_);

//     // necessary
//     this->setIsChanged(false);
// }
// void FramePerSecondUI::UpdateCurrentTime(std::chrono::time_point<std::chrono::high_resolution_clock>* present) {
//     present_ = *present;
//     this->setIsChanged(true);
// }

// void FramePerSecondUI::UpdateDifferTime(std::chrono::duration<int64_t, std::nano>* diff) {
//     diff_ = *diff;
//     this->setIsChanged(true);
// }

/* StandbyUI Class ===================================================================================== */
StandbyUI::StandbyUI(const YX& currnet_screen_size)
    : UI(currnet_screen_size) {
    win_size_.y = 46;
    win_size_.x = 160;

    // Start pos, center로
    start_pos_.y = (currnet_screen_size.y - win_size_.y) / 2;
    start_pos_.x = (currnet_screen_size.x - win_size_.x) / 2;
    // std::cout << currnet_screen_size.y << " " << currnet_screen_size.x << "d" << std::endl;
    win_ = newwin(win_size_.y, win_size_.x, start_pos_.y, start_pos_.x);
}

StandbyUI::~StandbyUI() {}

void StandbyUI::UpdatePhysics() {
    // nothing

    // necessary
    this->setIsChanged(true);
}
void StandbyUI::UpdateRendering() {
    // std::cout << " oh my god " << std::endl;
    const char* hello_message =
        "Hi Thank you for playing my game.\n\
    This tetris is made for improving my coding skill\n\
    so if you want to look at the code. please visit www.github.com/heonjj/CLI-Tetris\n\
    I will grad if you take a look my code and comment(or criticize)\n\n\
    Anyway!, I made this almost two month.\n\
    So happily play game.";
    int hello_message_y = 10;
    int hello_message_x = 4;

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
ExitUI::ExitUI(const YX& currnet_screen_size)
    : UI(currnet_screen_size) {
    win_size_.y = 46;
    win_size_.x = 160;

    // Start pos, center로
    start_pos_.y = (currnet_screen_size.y - win_size_.y) / 2;
    start_pos_.x = (currnet_screen_size.x - win_size_.x) / 2;

    win_ = newwin(win_size_.y, win_size_.x, start_pos_.y, start_pos_.x);
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
    int goodbye_message_x = 4;

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
