#ifndef CLI_TETRIS_UI_H_
#define CLI_TETRIS_UI_H_

#include "thread-manager.h"
#include "object-defined.h"

namespace cli_tetris {

/** UI 가 stand In,Output을 모두 관리합니다.
 *  ConsoleDevice를 반드시 정의를 유도 (ncurse initialize)
 *  Menu, 각 Part drawing등 다양한 drawing method 집합
 */
class Ui : public CustomThreadManager {
   private:
    bool is_initialized;

   public:
    Ui(int thread_workers = 2);
    ~Ui();

   private:
    void Initialize();
    void End();

   public:
    LineColumn getScreenMaxSize() const;
    LineColumn getGameScreenSize() const;
    void Draw(Object& object) {
        // TODO: 여기에 object 객체를 판별해, Object 상속개체가 아니면 throw해주면 좋겠다.

        AddJob(&Object::UpdateRendering);
    }
};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_UI_H_