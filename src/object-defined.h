#ifndef CLI_TETRIS_OBJECT_DEFINED_H_
#define CLI_TETRIS_OBJECT_DEFINED_H_

namespace cli_tetris {

using Pos = struct Pos {
    int y;
    int x;
};

class Object {
   private:
    Pos pos_yx_;
    bool is_changed;

   protected:
    virtual ~Object(){};
    Object(int pos_y = 0, int pos_x = 0);
    Object(const Object& object) = delete;

    Pos getObjectPos() const;
    void setObjectPos(int y, int x);

   public:
    virtual void UpdatePhysics() = 0;
    virtual void UpdateRendering() = 0;
};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_GAME_OBJECT_DEFINED_H_