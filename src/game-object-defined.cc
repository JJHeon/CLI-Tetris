#include "game-object-defined.h"

namespace cli_tetris {

using object::TYPE;

TYPE Object::getObjectType() const {
    return item_type_;
}

Object::Object(TYPE item_type)
    : item_type_(item_type) {}

}  // namespace cli_tetris
