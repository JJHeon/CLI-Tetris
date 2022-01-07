#include "ui-handler.h"

#include <exception>
#include <cassert>

#include "object-defined.h"

extern "C" {
#include <ncurses.h>
}

namespace cli_tetris {

UiHandler::UiHandler() {
    assert(!is_initialized_);
    is_initialized_ = true;

    this->Initialize();
    if (has_colors())
        this->SetColors();
    else
        throw std::runtime_error(std::string("E011 : You terminal does not support color"));
}

UiHandler::~UiHandler() {
    if (is_initialized_) this->End();
    is_initialized_ = false;
}

void UiHandler::Initialize() {
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    refresh();
    refresh();
}

void UiHandler::End() {
    is_initialized_ = false;
    keypad(stdscr, FALSE);
    endwin();
}

void UiHandler::SetColors() {
    start_color();  // Start color
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6, COLOR_BLACK, COLOR_CYAN);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);

    init_pair(15, COLOR_WHITE, COLOR_BLACK);
}

bool UiHandler::IsInitialized() const {
    return is_initialized_;
}

object::YX UiHandler::getCurrentScreenSize() {
    if (!this->IsInitialized()) throw std::runtime_error(std::string("E005 : UI 초기화 안됨"));

    object::YX n = {0, 0};
    getmaxyx(stdscr, n.y, n.x);

    return n;
}

void UiHandler::Draw(object::GraphicObject* object) {
    if (!this->IsInitialized()) throw std::runtime_error(std::string("E005 : UI 초기화 안됨"));

    object->UpdateRendering();
}

void UiHandler::ClearScreen() {
    if (!this->IsInitialized()) throw std::runtime_error(std::string("E005 : UI 초기화 안됨"));

    clear();
    refresh();
}

}  // namespace cli_tetris
