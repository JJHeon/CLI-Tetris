#include "object-defined.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <exception>
#include <cstdlib>
#include <cstring>
#include <algorithm>

extern "C" {
#include <ncurses.h>
#include <menu.h>
#include <panel.h>
}

#include "game-module.h"

namespace cli_tetris::object {
/* GraphicObject Abstract ===================================================================================== */
GraphicObject::GraphicObject()
    : is_changed(true) {}

bool GraphicObject::IsChanged() const {
    return is_changed;
}

void GraphicObject::setIsChanged(bool changed) {
    is_changed = changed;
}

/* TerminalWindowFunction Abstract ===================================================================================== */
TerminalWindowFunction::TerminalWindowFunction(const YX& currnet_screen_size, const YX& start_pos, const YX& win_size)
    : start_pos_(start_pos), currnet_screen_size_(currnet_screen_size), win_size_(win_size), win_(NULL) {
    win_ = newwin(win_size_.y, win_size_.x, start_pos_.y, start_pos_.x);
}
TerminalWindowFunction::TerminalWindowFunction(const YX& currnet_screen_size, const YX& win_size)
    : currnet_screen_size_(currnet_screen_size), win_size_(win_size), win_(NULL) {
    start_pos_.y = (currnet_screen_size.y - win_size_.y) / 2;
    start_pos_.x = (currnet_screen_size.x - win_size_.x) / 2;
    win_ = newwin(win_size_.y, win_size_.x, start_pos_.y, start_pos_.x);
}

TerminalWindowFunction::~TerminalWindowFunction() {
    if (win_ != NULL) delwin(win_);
}

/* TerminalMenuFunction Abstract ===================================================================================== */
TerminalMenuFunction::TerminalMenuFunction(const YX& currnet_screen_size, const YX& menu_start_pos, const YX& menu_win_size)
    : menu_start_pos_(menu_start_pos), currnet_screen_size_(currnet_screen_size), menu_win_size_(menu_win_size), menu_win_(NULL) {
    menu_win_ = newwin(menu_win_size_.y, menu_win_size_.x, menu_start_pos_.y, menu_start_pos_.x);
}
TerminalMenuFunction::TerminalMenuFunction(const YX& currnet_screen_size, const YX& menu_win_size)
    : currnet_screen_size_(currnet_screen_size), menu_win_size_(menu_win_size), menu_win_(NULL) {
    menu_start_pos_.y = (currnet_screen_size.y - menu_win_size_.y) / 2;
    menu_start_pos_.x = (currnet_screen_size.x - menu_win_size_.x) / 2;

    menu_win_ = newwin(menu_win_size_.y, menu_win_size_.x, menu_start_pos_.y, menu_start_pos_.x);
}

TerminalMenuFunction::~TerminalMenuFunction() {
    if (menu_win_ != NULL) delwin(menu_win_);
}

MENU* TerminalMenuFunction::GetMenuAccessor() const {
    return menu_;
}
WINDOW* TerminalMenuFunction::GetMenuWinAccessor() const {
    return menu_win_;
}

/* StandbyPage Class ===================================================================================== */
StandbyPage::StandbyPage(const YX& currnet_screen_size)
    : TerminalWindowFunction(currnet_screen_size, YX(46, 160)) {
}

void StandbyPage::UpdateState() {
    // nothing

    // necessary
    this->setIsChanged(true);
}
void StandbyPage::UpdateRendering() {
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
/* ExitPage Class ===================================================================================== */
ExitPage::ExitPage(const YX& currnet_screen_size)
    : TerminalWindowFunction(currnet_screen_size, YX(46, 160)) {}

void ExitPage::UpdateState() {
    // nothing
    // necessary
    this->setIsChanged(true);
}

void ExitPage::UpdateRendering() {
    const char* goodbye_message =
        "It's for development page.";
    int goodbye_message_y = 10;
    int goodbye_message_x = 4;

    // Draw
    attron(A_BOLD);
    box(win_, 0, 0);
    mvwprintw(win_, goodbye_message_y, goodbye_message_x, goodbye_message);
    attroff(A_BOLD);

    // Last excution
    wrefresh(win_);

    // necessary.
    this->setIsChanged(false);
}
/* MenuObject Class ===================================================================================== */
MenuObject::MenuObject(const YX& currnet_screen_size)
    : TerminalMenuFunction(currnet_screen_size, YX(12, 30)) {
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

    // Setting menu, menu_win
    set_menu_win(menu_, menu_win_);
    set_menu_sub(menu_, derwin(menu_win_, 8, 22, 3, 1));
    set_menu_mark(menu_, " -> ");
}
MenuObject::~MenuObject() {
    unpost_menu(menu_);
    free_menu(menu_);
    for (int i = 0; i < item_names_num; ++i) free_item(items_[i]);
}

void MenuObject::UpdateState() {
    // nothing
    // necessary
    this->setIsChanged(true);
}

void MenuObject::UpdateRendering() {
    // Draw
    attron(A_BOLD);
    box(menu_win_, 0, 0);
    attroff(A_BOLD);

    // Last excution
    post_menu(menu_);
    wrefresh(menu_win_);

    // necessary.
    this->setIsChanged(false);
}

/* FrameUI Class ===================================================================================== */
FrameObject46X160::FrameObject46X160(const YX& currnet_screen_size)
    : TerminalWindowFunction(currnet_screen_size, YX(46, 160)) {
}
FrameObject46X160::~FrameObject46X160() {}

void FrameObject46X160::UpdateState() {
    // nothing
    // necessary
    this->setIsChanged(true);
}

void FrameObject46X160::UpdateRendering() {
    // Draw
    box(win_, 0, 0);

    // Last excution
    wrefresh(win_);

    // necessary.
    this->setIsChanged(false);
}

/* TetrisBoardUI Class ===================================================================================== */
TetrisBoard::TetrisBoard(const YX& currnet_screen_size, const YX& start_pos)
    : TerminalWindowFunction(currnet_screen_size, start_pos, YX(42, 44)) {
}
TetrisBoard::~TetrisBoard() {}

void TetrisBoard::UpdateState() {
    // necessary
    this->setIsChanged(true);
}

void TetrisBoard::UpdateRendering() {
    wattron(win_, A_BOLD);
    box(win_, 0, 0);
    wattroff(win_, A_BOLD);

    // Last excution
    wrefresh(win_);

    // necessary.
    this->setIsChanged(false);
}

/* TopBoard Class ===================================================================================== */
TopBoard::TopBoard(const YX& currnet_screen_size, const YX& start_pos)
    : TerminalWindowFunction(currnet_screen_size, start_pos, YX(5, 28)) {
}
TopBoard::~TopBoard() {}

void TopBoard::UpdateState() {
    // nothing

    // necessary
    this->setIsChanged(true);
}

void TopBoard::UpdateRendering() {
    //  Draw
    wattron(win_, A_BOLD);
    box(win_, 0, 0);
    mvwprintw(win_, 1, 8, "TOP");
    mvwprintw(win_, 3, 8, "000000000000");
    wattroff(win_, A_BOLD);

    // Last excution
    wrefresh(win_);

    // necessary
    this->setIsChanged(false);
}

/* ScoreBoard Class ===================================================================================== */
ScoreBoard::ScoreBoard(const YX& currnet_screen_size, const YX& start_pos)
    : TerminalWindowFunction(currnet_screen_size, start_pos, YX(5, 28)) {}
ScoreBoard::~ScoreBoard() {}

void ScoreBoard::UpdateState() {
    // nothing
    // necessary
    this->setIsChanged(true);
}

void ScoreBoard::UpdateRendering() {
    //  Draw
    wattron(win_, A_BOLD);
    box(win_, 0, 0);
    mvwprintw(win_, 1, 8, "SCORE");
    mvwprintw(win_, 3, 8, "000000000000");
    wattroff(win_, A_BOLD);

    // Last excution
    wrefresh(win_);

    // necessary
    this->setIsChanged(false);
}
/* NextTetrisBoard Class ===================================================================================== */
NextTetrisBoard::NextTetrisBoard(const YX& currnet_screen_size, const YX& start_pos)
    : TerminalWindowFunction(currnet_screen_size, start_pos, YX(13, 28)) {}

NextTetrisBoard::~NextTetrisBoard() {}

void NextTetrisBoard::UpdateState() {
    // nothing
    // necessary
    this->setIsChanged(true);
}

void NextTetrisBoard::UpdateRendering() {
    //  Draw
    wattron(win_, A_BOLD);
    box(win_, 0, 0);
    mvwprintw(win_, 1, 8, "Next Tetris");
    wattroff(win_, A_BOLD);

    // Last excution
    wrefresh(win_);

    // necessary
    this->setIsChanged(false);
}
/* LevelBoard Class ===================================================================================== */
LevelBoard::LevelBoard(const YX& currnet_screen_size, const YX& start_pos)
    : TerminalWindowFunction(currnet_screen_size, start_pos, YX(5, 28)) {}

LevelBoard::~LevelBoard() {}

void LevelBoard::UpdateState() {
    // nothing
    // necessary
    this->setIsChanged(true);
}

void LevelBoard::UpdateRendering() {
    //  Draw
    wattron(win_, A_BOLD);
    box(win_, 0, 0);
    mvwprintw(win_, 1, 8, "LEVEL");
    mvwprintw(win_, 3, 8, "1");
    wattroff(win_, A_BOLD);

    // Last excution
    wrefresh(win_);

    // necessary
    this->setIsChanged(false);
}
/* InformBoard Class ===================================================================================== */
InformBoard::InformBoard(const YX& currnet_screen_size, const YX& start_pos)
    : TerminalWindowFunction(currnet_screen_size, start_pos, YX(10, 28)) {}

InformBoard::~InformBoard() {}

void InformBoard::UpdateState() {
    // nothing
    // necessary
    this->setIsChanged(true);
}

void InformBoard::UpdateRendering() {
    //  Draw
    wattron(win_, A_BOLD);
    box(win_, 0, 0);
    mvwprintw(win_, 1, 8, "INFORM");
    wattroff(win_, A_BOLD);

    // Last excution
    wrefresh(win_);

    // necessary
    this->setIsChanged(false);
}
}  // namespace cli_tetris::object
