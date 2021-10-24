#include <iostream>

#include "game-module.h"
#include "service-manager.h"
#include "ui.h"

int main(void) {
    // using cli_tetris::CreditState;
    // using cli_tetris::DuoPlayState;
    // using cli_tetris::EndState;
    using cli_tetris::GameManager;
    // using cli_tetris::MenuState;
    // using cli_tetris::MultiPlayState;
    // using cli_tetris::SettingState;
    // using cli_tetris::SoloPlayState;
    using cli_tetris::StartState;
    using cli_tetris::StateCode;
    // using cli_tetris::TemperaryStopState;

    using cli_tetris::Locator;
    using cli_tetris::Ui;

    /**
     *  ncurse 화면 초기화와 관련된 함수
     *  한번만 초기화 되어야 함.
     * */
    Locator::provideUi(new Ui);

    /* Tetris Manager */
    GameManager tetris(Locator::getUi(), StateCode::kStart);

    try {
        tetris.Initialize();
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    if (!tetris.CheckGameState()) return -1;
    tetris.Run();

    // Finish ncurse
    Locator::releaseUi();
    return 0;
}