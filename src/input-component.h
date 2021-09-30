#ifndef CLI_TETRIS_INPUT_COMPONENT_H_
#define CLI_TETRIS_INPUT_COMPONENT_H_

namespace cli_tetris {

class Command {
   public:
    virtual ~Command() {}
    virtual void Excute() = 0;
};

class MoveObjectCommand : public Command {
    
}

}  // namespace cli_tetris

#endif  //CLI_TETRIS_INPUT_COMPONENT_H_