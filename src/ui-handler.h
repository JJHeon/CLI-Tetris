/** Note
 * ncurse handling class
 *
 * GraphicObject class와 dependency
 * ncurse 사용하기 전에 Setting, object를 사용하기 위한 사전 조건의 책임을 가짐
 *
 *
 */

#ifndef CLI_TETRIS_UI_H_
#define CLI_TETRIS_UI_H_

#include "object-defined.h"

extern "C" {
#include <ncurses.h>
}

namespace cli_tetris {

/* UiHandler::ControlMenuDriver()의 parm */
using MenuRequest = enum class MenuRequest {
    UP,
    DOWN
};

class UiHandler {
   private:
    bool is_initialized_;

   public:
    UiHandler();
    ~UiHandler();

   private:
    bool IsInitialized() const;
    void Initialize();
    void End();
    void SetColors();

   public:
    object::YX getCurrentScreenSize();
    void Draw(object::GraphicObject* object);
    void ClearScreen();
    int getInput();

    void ControlMenuDriver(MENU* menu_ptr, MenuRequest request);
};

inline int UiHandler::getInput() {
    int input;
    input = getch();
    return input;
}

inline void UiHandler::ControlMenuDriver(MENU* menu_ptr, MenuRequest request) {
    switch (request) {
        case MenuRequest::UP:
            menu_driver(menu_ptr, REQ_UP_ITEM);
            break;
        case MenuRequest::DOWN:
            menu_driver(menu_ptr, REQ_DOWN_ITEM);
            break;
        default:
            break;
    }
}

}  // namespace cli_tetris

#endif  // CLI_TETRIS_UI_H_