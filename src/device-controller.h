#ifndef CLI_TETRIS_DEVICE_CONTROLLER_H_
#define CLI_TETRIS_DEVICE_CONTROLLER_H_

namespace cli_tetris {

/** ConsoleDevice는 UI를 위한 추상화돤 Device입니다. 하지만 - 
 *  ncurse c function을 wrap할 의도였지만 하지못했습니다.
 *  ncurse가 방대하고 얼마나 사용될지 모르기 때문입니다.
 *  CATION: 오직 단 하나의 instance만 생성되어야 합니다. (전역변수의 문제를 고려해 singleton은 고려하지 않았습니다.)
 *  initscr과 같은 ncurse 사용의 초기화만을 담당하는 class입니다.
 *  ncurse 함수는 ConsoleDevice를 포함해 object component와, 각 State의 Ui 부분에서 사용됩니다.
 */
class ConsoleDevice final {
   private:
    bool is_initialized;

   public:
    ConsoleDevice();
    ~ConsoleDevice();

    void Initialize();
    void ReturnDevice();
};

}  // namespace cli_tetris

#endif  //CLI_TETRIS_DEVICE_CONTROLLER_H_
