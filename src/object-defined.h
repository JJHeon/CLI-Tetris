#ifndef CLI_TETRIS_OBJECT_DEFINED_H_
#define CLI_TETRIS_OBJECT_DEFINED_H_

namespace cli_tetris {

using Pos = struct {
    int y;
    int x;
};

class Object {
   private:
    Pos pos_yx_;

   protected:
    virtual ~Object(){};
    Object(int pos_y = 0, int pos_x = 0);
    Object(const Object& object) = default;

    Pos getObjectPos() const;
    void setObjectPos(int y, int x);
};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_GAME_OBJECT_DEFINED_H_