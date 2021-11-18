#include "object-defined.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <exception>
#include <cstdlib>
#include <cstring>

extern "C" {

#include <menu.h>
#include <panel.h>
}
#include <ncurses.h>

#include "game-module.h"

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
    So happily play game. ";
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
/* MenuUI Class ===================================================================================== */
MenuUI::MenuUI(const YX& currnet_screen_size)
    : UI(currnet_screen_size) {
    win_size_.y = 46;
    win_size_.x = 160;

    /* Create Menu ---------------------------------------------------------------------------------- */
    // Start pos, center로
    start_pos_.y = (currnet_screen_size.y - win_size_.y) / 2;
    start_pos_.x = (currnet_screen_size.x - win_size_.x) / 2;
    //외곽 WIndow
    win_ = newwin(win_size_.y, win_size_.x, start_pos_.y, start_pos_.x);

    // menu item 등록
    const char* item_0 = "Keep Playing";
    const char* item_1 = "New Game";
    const char* item_2 = "Load Privous Game";
    const char* item_3 = "Play together";
    const char* item_4 = "See board";
    const char* item_5 = "Exit";

    std::strncpy(item_names[0], item_0, std::strlen(item_0) + 1);
    std::strncpy(item_names[1], item_1, std::strlen(item_1) + 1);
    std::strncpy(item_names[2], item_2, std::strlen(item_2) + 1);
    std::strncpy(item_names[3], item_3, std::strlen(item_3) + 1);
    std::strncpy(item_names[4], item_4, std::strlen(item_4) + 1);
    std::strncpy(item_names[5], item_5, std::strlen(item_5) + 1);

    item_names_num = 6;
    items_ = (ITEM**)calloc(item_names_num + 1, sizeof(ITEM*));
    for (int i = 0; i < item_names_num; ++i) {
        items_[i] = new_item(item_names[i], item_names[i]);
    }
    items_[item_names_num] = (ITEM*)NULL;
    //  Create Menu
    menu_ = new_menu((ITEM**)items_);

    // Create menu win
    int menu_win_size_y = 12;
    int menu_win_size_x = 30;
    int menu_start_pos_y = (currnet_screen_size.y - menu_win_size_y) / 2;
    int menu_start_pos_x = (currnet_screen_size.x - menu_win_size_x) / 2;
    menu_win_ = newwin(menu_win_size_y, menu_win_size_x, menu_start_pos_y, menu_start_pos_x);

    // Setting menu, menu_win
    set_menu_win(menu_, menu_win_);
    set_menu_sub(menu_, derwin(menu_win_, 8, 22, 3, 1));
    set_menu_mark(menu_, " -> ");

    /* Set Panel ---------------------------------------------------------------------------------- */

    ui_panel_[0] = new_panel(win_);
    ui_panel_[1] = new_panel(menu_win_);

    ui_panel_data_[0].hide = false;
    ui_panel_data_[1].hide = false;

    set_panel_userptr(ui_panel_[0], &ui_panel_data_[0]);
    set_panel_userptr(ui_panel_[1], &ui_panel_data_[1]);
}
MenuUI::~MenuUI() {
    unpost_menu(menu_);
    free_menu(menu_);
    delwin(menu_win_);
    for (int i = 0; i < item_names_num; ++i) free_item(items_[i]);
}

void MenuUI::UpdatePhysics() {
    // nothing

    // necessary
    this->setIsChanged(true);
}

void MenuUI::UpdateRendering() {
    // Draw
    attron(A_BOLD);
    box(win_, 0, 0);
    attroff(A_BOLD);
    box(menu_win_, 0, 0);
    post_menu(menu_);

    // Last excution
    update_panels();
    doupdate();

    // necessary.
    this->setIsChanged(false);
}
MENU* MenuUI::GetMenuAccessor() const {
    return menu_;
}
WINDOW* MenuUI::GetMenuWinAccessor() const {
    return menu_win_;
}
/* FrameUI Class ===================================================================================== */
FrameUI46X160::FrameUI46X160(const YX& currnet_screen_size)
    : UI(currnet_screen_size) {
    win_size_.y = 46;
    win_size_.x = 160;

    // Start pos, center로
    start_pos_.y = (currnet_screen_size.y - win_size_.y) / 2;
    start_pos_.x = (currnet_screen_size.x - win_size_.x) / 2;

    win_ = newwin(win_size_.y, win_size_.x, start_pos_.y, start_pos_.x);
}
FrameUI46X160::~FrameUI46X160() {}

void FrameUI46X160::UpdatePhysics() {
    // nothing
    // necessary
    this->setIsChanged(true);
}

void FrameUI46X160::UpdateRendering() {
    // Draw
    box(win_, 0, 0);

    // Last excution
    wrefresh(win_);

    // necessary.
    this->setIsChanged(false);
}

/* TetrisBoardUI Class ===================================================================================== */
TetrisBoardUI::TetrisBoardUI(const YX& currnet_screen_size, const YX& offset)
    : UI(currnet_screen_size) {
    // Set win Size
    // win_size_.y = 22;
    // win_size_.x = 24;
    win_size_.y = 42;
    win_size_.x = 44;

    // Set relative_start_pos
    YX game_screen_size = GameManager::getNeededScreenSize();
    relative_start_pos_.y = ((currnet_screen_size.y - game_screen_size.y) / 2) + 1;
    relative_start_pos_.x = ((currnet_screen_size.x - game_screen_size.x) / 2) + 1;

    // Start pos
    start_pos_.y = relative_start_pos_.y + offset.y;
    start_pos_.x = relative_start_pos_.x + offset.x;

    win_ = newwin(win_size_.y, win_size_.x, start_pos_.y, start_pos_.x);
    //  win_ = newwin(22, 24,30 ,60);
}
TetrisBoardUI::~TetrisBoardUI() {}

void TetrisBoardUI::UpdatePhysics() {
    // nothing

    // necessary
    this->setIsChanged(true);
}

void TetrisBoardUI::UpdateRendering() {
    //  Draw
    wattron(win_, COLOR_PAIR(3));
    for (int i = 0; i != win_size_.y; ++i) {
        wmove(win_, i, 0);
        if (i == 0 || i == (win_size_.y - 1)) {
            for (int j = 0; j != win_size_.x; ++j) waddch(win_, ' ');
        }

        mvwaddch(win_, i, 0, ' ');
        mvwaddch(win_, i, 1, ' ');
        mvwaddch(win_, i, win_size_.x - 2, ' ');
        mvwaddch(win_, i, win_size_.x - 1, ' ');
    }
    wattroff(win_, COLOR_PAIR(3));

    // Last excution
    wrefresh(win_);

    // necessary.
    this->setIsChanged(false);
}

}  // namespace cli_tetris
