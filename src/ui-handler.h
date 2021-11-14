#ifndef CLI_TETRIS_UI_H_
#define CLI_TETRIS_UI_H_

#include "thread-manager.h"
#include "object-defined.h"

namespace cli_tetris {

/** UI 가 stand In,Output을 모두 관리합니다.
 *  ConsoleDevice를 반드시 정의를 유도 (ncurse initialize)
 *  Menu, 각 Part drawing등 다양한 drawing method 집합
 */
class UiHandler : public CustomThreadManager<Object> {
   private:
    bool is_initialized_;

   public:
    UiHandler(int thread_workers = 2);
    ~UiHandler();

   private:
    bool IsInitialized() const;
    void Initialize();
    void End();

   public:
    YX getCurrentScreenSize();
    // YX getGameScreenSize() const;
    void Draw(Object* object);
    void ClearScreen();
};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_UI_H_