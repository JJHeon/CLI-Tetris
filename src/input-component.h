#ifndef CLI_TETRIS_INPUT_COMPONENT_H_
#define CLI_TETRIS_INPUT_COMPONENT_H_

#include "object-defined.h"

namespace cli_tetris {
/* Command Class ----------------------------------------------------------------------------------- */
class Command {
   public:
    virtual ~Command() {}
    virtual void Excute() = 0;
};

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

#endif  //CLI_TETRIS_INPUT_COMPONENT_H_