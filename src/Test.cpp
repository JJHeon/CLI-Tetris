#include <iostream>

#include "game-module.h"

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

    GameManager tetris(
        StateCode::kStart,
        new StartState(&tetris),
        new EndState,
        new MenuState,
        new TemperaryStopState,
        new SoloPlayState,
        new DuoPlayState,
        new MultiPlayState,
        new SettingState,
        new CreditState);

    if (!tetris.CheckGameState()) return -1;
    tetris.Run();

    return 0;
}