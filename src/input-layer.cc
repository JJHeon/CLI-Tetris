#include "input-layer.h"

#include <queue>

namespace cli_tetris {
bool CommandQueue::IsFullQueue() const {
    if (limit_queue_num_ == queue_.size())
        return true;
    else
        return false;
}

CommandQueue::CommandQueue(const int& limit_queue)
    : limit_queue_num_(limit_queue) {
}
CommandQueue::~CommandQueue() {
    this->ResetQueue();
}
void CommandQueue::ResetQueue() {
    while (!queue_.empty()) {
        Command* ptr = queue_.front();
        queue_.pop();
        delete ptr;
    }
}

bool CommandQueue::Put(Command* command) {
    if (IsFullQueue()) return false;

    queue_.push(command);
    return true;
}
Command* CommandQueue::Get() {
    if (queue_.empty()) return nullptr;

    Command* ptr = queue_.front();
    queue_.pop();

    return ptr;
}

/* Command Class ----------------------------------------------------------------------------------- */
MenuItemMoveCommand::MenuItemMoveCommand(Move where)
    : where_(where) {
}
MenuItemMoveCommand::~MenuItemMoveCommand() {
}
void MenuItemMoveCommand::Excute() {}
}  // namespace cli_tetris
