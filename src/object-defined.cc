#include "object-defined.h"

namespace cli_tetris {

Object::Object(int pos_y = 0, int pos_x = 0)
    : ui_(nullptr), physics_(nullptr) {
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
