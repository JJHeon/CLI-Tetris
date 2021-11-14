/**
 *  Command Pattern을 위해 생성되었지만, 개발하다보니 현재 Tetris에는
 *  Command Pattern이 필요없다. 구지 이용하려고 하면 더 번거러워질 우려가 있다.
 *  사용하지 않는 것으로 한다.
 */

#ifndef CLI_TETRIS_INPUT_COMPONENT_H_
#define CLI_TETRIS_INPUT_COMPONENT_H_

#include "object-defined.h"

#include <queue>

namespace cli_tetris {
class CommandQueue {
   private:
    std::queue<Command*> queue_;
    const int limit_queue_num_;

   private:
    bool IsFullQueue() const;

   public:
    CommandQueue(const int& limit_queue);
    ~CommandQueue();
    void ResetQueue();
    bool Put(Command* command);
    Command* Get();
};

/* Command Class ----------------------------------------------------------------------------------- */
class Command {
   public:
    virtual ~Command() {}
    virtual void Excute() = 0;
};

class MenuItemMoveCommand : public Command {
   public:
    using Move = enum Move { kUP = 0,
                             kDown,
                             kLeft,
                             kRight };

   private:
    Move where_;

   public:
    MenuItemMoveCommand(Move where);
    ~MenuItemMoveCommand();
    void Excute() override;
};

/* Block key ----------------------------------------------------------------------------------- */
class ChangeBlockCommand : public Command {
   private:
    Object* object_;

   public:
    ChangeBlockCommand(Object* object = nullptr);
    virtual ~ChangeBlockCommand();

    virtual void Excute() override;
};

class FallBlockCommand : public Command {
   private:
    Object* object_;

   public:
    FallBlockCommand(Object* Object = nullptr);
    virtual ~FallBlockCommand();

    virtual void Excute() override;
};

class MoveDownBlockCommand : public Command {
   private:
    Object* object_;

   public:
    MoveDownBlockCommand(Object* Object = nullptr);
    virtual ~MoveDownBlockCommand();

    virtual void Excute() override;
};

class MoveLeftBlockCommand : public Command {
   private:
    Object* object_;

   public:
    MoveLeftBlockCommand(Object* Object = nullptr);
    virtual ~MoveLeftBlockCommand();

    virtual void Excute() override;
};

class MoveRightBlockCommand : public Command {
   private:
    Object* object_;

   public:
    MoveRightBlockCommand(Object* Object = nullptr);
    virtual ~MoveRightBlockCommand();

    virtual void Excute() override;
};

/* InputHandler Class ----------------------------------------------------------------------------------- */

}  // namespace cli_tetris

#endif  // CLI_TETRIS_INPUT_COMPONENT_H_