#ifndef CLI_TETRIS_GAME_OBJECT_DEFINED_H_
#define CLI_TETRIS_GAME_OBJECT_DEFINED_H_

namespace cli_tetris {

namespace object {
using TYPE = enum TYPE {
    MENU_ICON = 0,
    BLOCK = 1
};
}  // namespace object

class Object {
   private:
    object::TYPE item_type_;

   public:
    object::TYPE getObjectType() const;
    Object(object::TYPE item_type);
};

}  // namespace cli_tetris

#endif  // CLI_TETRIS_GAME_OBJECT_DEFINED_H_