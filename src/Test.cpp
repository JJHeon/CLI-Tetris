#include <iostream>

#include "game-module.h"
#include "service-manager.h"

int main(void) {
    using cli_tetris::CreditState;
    using cli_tetris::DuoPlayState;
    using cli_tetris::EndState;
    using cli_tetris::GameManager;
    using cli_tetris::MenuState;
    using cli_tetris::MultiPlayState;
    using cli_tetris::SettingState;
    using cli_tetris::SoloPlayState;
    using cli_tetris::StartState;
    using cli_tetris::StateCode;
    using cli_tetris::TemperaryStopState;

    using cli_tetris::Locator;
    using cli_tetris::Ui;


    /** Ui의 생성에 ConsoleDevice가 필요하도록 의도적으로 배치.
     *  ConsoleDevice 생성자 내부에 ncurse 화면 초기화와 관련된 함수가 있으며 
     *  한번만 초기화되어야 하기 떄문이다.
     * */ 
    Locator::provideUi(new Ui(new cli_tetris::ConsoleDevice));

    /* 목표로한 완전한 Tetris Manager 선언 */
    // GameManager tetris(
    //     StateCode::kStart,
    //     new StartState(&tetris),
    //     new EndState,
    //     new MenuState,
    //     new TemperaryStopState,
    //     new SoloPlayState,
    //     new DuoPlayState,
    //     new MultiPlayState,
    //     new SettingState,
    //     new CreditState);

    /* 개발 단계 Tetris Manager */
    GameManager tetris(StateCode::kStart,
                       new StartState(tetris));

    if (!tetris.CheckGameState()) return -1;
    tetris.Run();

    return 0;
}