#include "object-defined.h"

#include <ncurses.h>

namespace cli_tetris {
/* Object Class ===================================================================================== */
Object::Object(int pos_y, int pos_x)
    : is_changed(false) {
    start_yx_.y = pos_y;
    start_yx_.x = pos_x;
}

Pos Object::getObjectPos() const {
    return start_yx_;
}

void Object::setObjectPos(int y, int x) {
    start_yx_.y = y;
    start_yx_.x = x;
}
/* StandbyUI Class ===================================================================================== */
StandbyUI::StandbyUI() {
}

StandbyUI::~StandbyUI() {
}

void StandbyUI::UpdatePhysics() {
    // nothing
}
void StandbyUI::UpdateRendering() {
}
}  // namespace cli_tetris
