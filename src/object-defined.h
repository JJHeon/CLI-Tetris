#ifndef CLI_TETRIS_OBJECT_DEFINED_H_
#define CLI_TETRIS_OBJECT_DEFINED_H_

namespace cli_tetris {

using Pos = struct {
    int y;
    int x;
};

using BlockType = enum BlockType {
    I = 0,
    O,
    T,
    L,
    J,
    S,
    Z
};

class Object {
   private:
    Pos pos_yx_;

   protected:
    UiComponent* ui_;
    PhysicsComponent* physics_;

    virtual ~Object(){};
    Object(int pos_y = 0, int pos_x = 0);
    Object(const Object& object) = default;

    Pos getObjectPos() const;
    void setObjectPos(int y, int x);

    virtual void setPhysicsComponent(PhysicsComponent* physics) = 0;
    virtual void setUiComponent(UiComponent* ui) = 0;

   public:
    virtual void UpdatePhysics() = 0;
    virtual void UpdateRendering() = 0;
};

class BlockI :public Object {

};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_GAME_OBJECT_DEFINED_H_