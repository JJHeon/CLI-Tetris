/** Note
 * object-defined
 *
 * -- 설명 --
 * GraphicObject abstract class는 Drawing 여부 확인 method 제공
 * drived class인 TerminalWindowFunction는 ncurse window 생성 및 관련 변수 제공
 * drived class인 TerminalMenuFunction는 ncurse menu 생성 및 제어 method 제공
 *
 * Page는 전체 그림을 의미합니다.
 * Object는 ncurse window 한개를 의미합니다.
 * Baord는 값을 갱신 받거나 전달해야하는 window입니다.
 * -- 변경 이력 -- (21.12.05 이전 기록 없음)
 */

#ifndef CLI_TETRIS_OBJECT_DEFINED_H_
#define CLI_TETRIS_OBJECT_DEFINED_H_

#include <chrono>
#include <array>
#include <vector>

extern "C" {
#include <ncurses.h>
#include <menu.h>
#include <panel.h>
}

namespace cli_tetris::object {

using YX = struct YX {
    int y;
    int x;
    constexpr YX(const int& y, const int& x) : y(y), x(x) {}
    constexpr YX(const YX& obj) : y(obj.y), x(obj.x) {}
    constexpr YX() : y(0), x(0) {}
};

/* GraphicObject Abstract ===================================================================================== */
class GraphicObject {
   private:
    bool is_changed;

   protected:
    GraphicObject();
    GraphicObject(const GraphicObject& object) = delete;

    /**
     * 내부 상태가 변했는지를 설정하는 method
     * UpdatePyhiscs 와 UpdateRendering 내부에서 사용합니다.
     */
    void setIsChanged(bool changed);

   public:
    virtual ~GraphicObject(){};
    bool IsChanged() const;
    virtual void UpdateState() = 0;
    virtual void UpdateRendering() = 0;
};

/* TerminalWindowItem Abstract ===================================================================================== */
/**
 *  ncurse newwin() 을 통해 window 생성 및 Variable 소유 class
 */
class TerminalWindowFunction : public GraphicObject {
   protected:
    const YX currnet_screen_size_;
    YX start_pos_;
    YX win_size_;

    WINDOW* win_;

   protected:
    TerminalWindowFunction(const YX& currnet_screen_size, const YX& start_pos, const YX& win_size);
    // Start position이 상속한 class의 생성자에서 사전에 계산함을 됨을 위함
    TerminalWindowFunction(const YX& currnet_screen_size, const YX& win_size);

   public:
    virtual void UpdateState() = 0;
    virtual void UpdateRendering() = 0;
    virtual ~TerminalWindowFunction();
};
/* TerminalMenuFunction Abstract ===================================================================================== */

class TerminalMenuFunction : public GraphicObject {
   protected:
    // For Menu
    const YX currnet_screen_size_;
    YX menu_start_pos_;
    YX menu_win_size_;

    MENU* menu_;
    WINDOW* menu_win_;

    ITEM** items_;
    char item_names[10][30];
    int item_names_num;
    // Entire UI
    using StructureData = struct StructureData {
        bool hide;
    };
    PANEL* ui_panel_[3];
    StructureData ui_panel_data_[3];

   protected:
    TerminalMenuFunction(const YX& currnet_screen_size, const YX& menu_start_pos, const YX& menu_win_size);
    TerminalMenuFunction(const YX& currnet_screen_size, const YX& menu_win_size);

   public:
    virtual ~TerminalMenuFunction();

    virtual void UpdateState() = 0;
    virtual void UpdateRendering() = 0;

    MENU* GetMenuAccessor() const;
    WINDOW* GetMenuWinAccessor() const;
};

/* StandbyPage Class ===================================================================================== */
class StandbyPage : public TerminalWindowFunction {
   public:
    //사전에 계산된 생성자
    StandbyPage(const YX& currnet_screen_size);
    ~StandbyPage() = default;

    // TerminalWindowFunction override
    void UpdateState() override;
    void UpdateRendering() override;
};

/* ExitPage Class ===================================================================================== */
class ExitPage : public TerminalWindowFunction {
   public:
    ExitPage(const YX& currnet_screen_size);
    ~ExitPage() = default;

    // TerminalWindowFunction override
    void UpdateState() override;
    void UpdateRendering() override;
};

/* MenuObject Class ===================================================================================== */
class MenuObject : public TerminalMenuFunction {
   public:
    MenuObject(const YX& currnet_screen_size);
    ~MenuObject();

    // TerminalMenuFunction override
    void UpdateState() override;
    void UpdateRendering() override;
};

/* FrameUI46X160 Class ===================================================================================== */
class FrameObject46X160 : public TerminalWindowFunction {
   public:
    FrameObject46X160(const YX& currnet_screen_size);
    ~FrameObject46X160();

    // TerminalWindowFunction override
    void UpdateState() override;
    void UpdateRendering() override;
};

/* TetrisBoard Class ===================================================================================== */
class TetrisBoard : public TerminalWindowFunction {
   private:
    const std::vector<std::vector<int>>* board_;

   private:
    // Custom
    void RenderConnectedBoard();

   public:
    TetrisBoard(const YX& currnet_screen_size, const YX& start_pos);
    ~TetrisBoard();

    // TerminalWindowFunction override
    void UpdateState() override;
    void UpdateRendering() override;

    // Custom
    void ConnectBoard(decltype(board_) board);
};

/* TopBoard Class ===================================================================================== */
class TopBoard : public TerminalWindowFunction {
   private:
   public:
    TopBoard(const YX& currnet_screen_size, const YX& start_pos);
    ~TopBoard();

    // TerminalWindowFunction override
    void UpdateState() override;
    void UpdateRendering() override;
};

/* ScoreBoard Class ===================================================================================== */
class ScoreBoard : public TerminalWindowFunction {
   private:
   public:
    ScoreBoard(const YX& currnet_screen_size, const YX& start_pos);
    ~ScoreBoard();

    // TerminalWindowFunction override
    void UpdateState() override;
    void UpdateRendering() override;
};
/* NextTetrisBoard Class ===================================================================================== */
class NextTetrisBoard : public TerminalWindowFunction {
   private:
   public:
    NextTetrisBoard(const YX& currnet_screen_size, const YX& start_pos);
    ~NextTetrisBoard();

    // TerminalWindowFunction override
    void UpdateState() override;
    void UpdateRendering() override;
};
/* LevelBoard Class ===================================================================================== */
class LevelBoard : public TerminalWindowFunction {
   private:
   public:
    LevelBoard(const YX& currnet_screen_size, const YX& start_pos);
    ~LevelBoard();

    // TerminalWindowFunction override
    void UpdateState() override;
    void UpdateRendering() override;
};
/* InformBoard Class ===================================================================================== */
class InformBoard : public TerminalWindowFunction {
   private:
    YX relative_start_pos_;

   public:
    InformBoard(const YX& currnet_screen_size, const YX& start_pos);
    ~InformBoard();

    // TerminalWindowFunction override
    void UpdateState() override;
    void UpdateRendering() override;
};

}  // namespace cli_tetris::object

#endif  // CLI_TETRIS_GAME_OBJECT_DEFINED_H_