#include "object-defined.h"

namespace cli_tetris {

Object::Object(int pos_y, int pos_x)
    : is_changed(false) {
    pos_yx_.y = pos_y;
    pos_yx_.x = pos_x;
}

Pos Object::getObjectPos() const {
    return pos_yx_;
}

void Object::setObjectPos(int y, int x) {
    pos_yx_.y = y;
    pos_yx_.x = x;
}

}  // namespace cli_tetris
