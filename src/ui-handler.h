#ifndef CLI_TETRIS_UI_H_
#define CLI_TETRIS_UI_H_

#include "thread-manager.h"
#include "object-defined.h"
#include "utility.h"

extern "C" {
#include <ncurses.h>
}

namespace cli_tetris {



/* UiHandler::ControlMenuDriver()의 parm */
using MenuRequest = enum MenuRequest {
    UP,
    DOWN
};

/** UI 가 stand In,Output을 모두 관리합니다.
 *  ConsoleDevice를 반드시 정의를 유도 (ncurse initialize)
 *  Menu, 각 Part drawing등 다양한 drawing method 집합
 */
// /* thread에서 ncurse 출력 문제로 threadManager 사용안하기로 결정 */
class UiHandler : public CustomThreadManager<Object> {
    // class UiHandler {
   private:
    bool is_initialized_;

   public:
    /* thread에서 ncurse 출력 문제로 threadManager 사용안하기로 결정 */
    UiHandler(int thread_workers = 2);
    // UiHandler(const int limit_queue_num);
    // UiHandler();
    ~UiHandler();

   private:
    bool IsInitialized() const;
    void Initialize();
    void End();
    void SetColors();

   public:
    YX getCurrentScreenSize();
    // YX getGameScreenSize() const;
    void Draw(Object* object);
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
/* PointerQueue Class ===================================================================================== */

}  // namespace cli_tetris

#endif  // CLI_TETRIS_UI_H_