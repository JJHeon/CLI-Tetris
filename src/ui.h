#ifndef CLI_TETRIS_UI_H_
#define CLI_TETRIS_UI_H_

namespace cli_tetris {

using LineColumn = struct LineColumn {
    int line;
    int column;
};

/** UI 가 stand In,Output을 모두 관리합니다.
 *  ConsoleDevice를 반드시 정의를 유도 (ncurse initialize)
 *  Menu, 각 Part drawing등 다양한 drawing method 집합
 */
class Ui {
   private:
    bool is_initialized;

   public:
    Ui();
    ~Ui();

   private:
    void Initialize();
    void End();

   public:
    LineColumn getScreenMaxSize();
};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_UI_H_