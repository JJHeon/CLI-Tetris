#ifndef CLI_TETRIS_OBJECT_DEFINED_H_
#define CLI_TETRIS_OBJECT_DEFINED_H_

extern "C" {
#include <ncurses.h>
}

namespace cli_tetris {

using LineColumn = struct LineColumn {
    int line;
    int column;
};

/* Object Class ===================================================================================== */
class Object {
   private:
    bool is_changed;

   protected:
    LineColumn start_pos_;

   protected:
    Object(int pos_y = 0, int pos_x = 0);
    Object(LineColumn start_pos);
    Object(const Object& object) = delete;

    LineColumn getObjectPos() const;
    void setObjectPos(int y, int x);

    /**
     * 내부 상태가 변했는지를 설정하는 method
     * UpdatePyhiscs 와 UpdateRendering 내부에서 사용합니다.
     */
    void setIsChanged(bool changed);

   public:
    virtual ~Object(){};

    bool IsChanged() const;
    virtual void UpdatePhysics() = 0;
    virtual void UpdateRendering() = 0;
};

/* StandbyUI Class ===================================================================================== */
class StandbyUI : public Object {
   protected:
    WINDOW* win_;
    LineColumn length_;

   protected:
    constexpr static LineColumn size_ = {.line = 46, .column = 160};

   public:
    StandbyUI(LineColumn start_pos);
    StandbyUI(int start_y, int start_x);
    ~StandbyUI();
    void UpdatePhysics() override;
    void UpdateRendering() override;
};

/* ExitUI Class ===================================================================================== */
class ExitUI : public StandbyUI {
   public:
    ExitUI(LineColumn start_pos);
    ExitUI(int start_y, int start_x);
    ~ExitUI();
    void UpdatePhysics() override;
    void UpdateRendering() override;
};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_GAME_OBJECT_DEFINED_H_