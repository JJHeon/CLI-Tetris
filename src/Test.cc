/** Note
 * Main
 *
 * -- 설명 --
 * GameManager Start
 *
 * -- 변경 이력 -- (21.12.05 이전 기록 없음)
 */

#include <iostream>
#include <chrono>
#include <thread>

#include "game-module.h"
#include "service-manager.h"
#include "ui-handler.h"
#include "random-generate-handler.h"

extern "C" {
#include <signal.h>
#include <time.h>
#include <ncurses.h>
}

void TimerTestcode();

static void ShowErrorToNcurse(const char* context) {
    mvprintw(0, 0, context);
    refresh();
}

int main(void) {
    using cli_tetris::GameManager;
    using cli_tetris::Locator;
    using cli_tetris::StateCode;
    using cli_tetris::UiHandler;
    using cli_tetris::random::RandomValueHandler;
    using cli_tetris::timer::TimerHandler;
    /**
     *  Ui -
     *  ncurse 화면 초기화와 관련된 함수
     *  단 한개의 객체만 허용됨
     *
     *  TimerHandler - linux system call 포함
     *  단 한개의 객체만 허용됨
     * */
    // Provider Init
    try {
        Locator::provideUiHandler(new UiHandler());
        Locator::provideTimerHandler(new TimerHandler);
        Locator::provideRandomValueHandler(new RandomValueHandler(1, 7,
                                                                  1, 4));
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    /* Tetris Manager Create */
    // GameManager tetris(StateCode::kStart);
    GameManager tetris(StateCode::kSoloPlay);

    // Tetris Manager Initalize
    try {
        tetris.Initialize();
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        ShowErrorToNcurse(e.what());
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    if (!tetris.CheckGameState()) return -1;

    // Tetris Manager Start
    try {
        tetris.Run();
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        ShowErrorToNcurse(e.what());
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    } catch (...) {
        std::cout << std::string("Default Error!") << std::endl;
    }

    // Finish ncurse, timer
    Locator::releaseUiHandler();
    Locator::releaseTimerHandler();
    Locator::releaseRandomValueHandler();

    return 0;
}

namespace cli_tetris::Test::TestCode {
static void TimerTestcode() {
    using cli_tetris::Locator;
    using namespace cli_tetris::timer;
    TimerHandler* tmp = Locator::getTimerHandler();

    TimerAccessor acc = tmp->CreateTimer();
    // Start 5초 타이머
    tmp->SetTimer(acc, 5, 0);
    tmp->StopTimer(acc);

    // Sleep 10 후 확인
    struct timespec t = {.tv_sec = 10, .tv_nsec = 200};
    struct timespec t2;

    std::cout << "Start : IsAlive : " << acc.IsAlive() << "|IsRunning : " << acc.IsRunning() << std::endl;
retry:
    int ret = nanosleep(&t, &t2);  // nanosleep 은 signal에 영향을 받아 중간에 중단된다.
    if (ret == -1) {
        if (errno == EINTR) {
            t.tv_sec = t2.tv_sec;
            t.tv_nsec = t2.tv_nsec;
            goto retry;
        }
    }

    std::cout << "Finish : IsAlive : " << acc.IsAlive() << "|IsRunning : " << acc.IsRunning() << std::endl;

    tmp->DeleteTimer(acc);

    t.tv_nsec = 0;
    t.tv_sec = 5;
retry2:
    ret = nanosleep(&t, &t2);  // nanosleep 은 signal에 영향을 받아 중간에 중단된다.
    if (ret == -1) {
        if (errno == EINTR) {
            t.tv_sec = t2.tv_sec;
            t.tv_nsec = t2.tv_nsec;
            goto retry2;
        }
    }

    std::cout << "Deleted : IsAlive : " << acc.IsAlive() << "|IsRunning : " << acc.IsRunning() << std::endl;

    t.tv_nsec = 0;
    t.tv_sec = 5;
retry3:
    ret = nanosleep(&t, &t2);  // nanosleep 은 signal에 영향을 받아 중간에 중단된다.
    if (ret == -1) {
        if (errno == EINTR) {
            t.tv_sec = t2.tv_sec;
            t.tv_nsec = t2.tv_nsec;
            goto retry3;
        }
    }

    TimerAccessor acc2 = tmp->CreateTimer();
    tmp->SetTimer(acc, 3, 0);
    std::cout << "Start2 : IsAlive : " << acc2.IsAlive() << "|IsRunning : " << acc2.IsRunning() << std::endl;

    t.tv_nsec = 0;
    t.tv_sec = 1;
retry4:
    ret = nanosleep(&t, &t2);  // nanosleep 은 signal에 영향을 받아 중간에 중단된다.
    if (ret == -1) {
        if (errno == EINTR) {
            t.tv_sec = t2.tv_sec;
            t.tv_nsec = t2.tv_nsec;
            goto retry4;
        }
    }

    tmp->SetTimer(acc, 7, 0);

    t.tv_nsec = 0;
    t.tv_sec = 3;
retry5:
    ret = nanosleep(&t, &t2);  // nanosleep 은 signal에 영향을 받아 중간에 중단된다.
    if (ret == -1) {
        if (errno == EINTR) {
            t.tv_sec = t2.tv_sec;
            t.tv_nsec = t2.tv_nsec;
            goto retry5;
        }
    }
    std::cout << "Start2 after 3, set 7: IsAlive : " << acc2.IsAlive() << "|IsRunning : " << acc2.IsRunning() << std::endl;

    t.tv_nsec = 0;
    t.tv_sec = 5;
retry6:
    ret = nanosleep(&t, &t2);  // nanosleep 은 signal에 영향을 받아 중간에 중단된다.
    if (ret == -1) {
        if (errno == EINTR) {
            t.tv_sec = t2.tv_sec;
            t.tv_nsec = t2.tv_nsec;
            goto retry6;
        }
    }
    std::cout << "Start2 after 8: IsAlive : " << acc2.IsAlive() << "|IsRunning : " << acc2.IsRunning() << std::endl;

    t.tv_nsec = 0;
    t.tv_sec = 5;
retry7:
    ret = nanosleep(&t, &t2);  // nanosleep 은 signal에 영향을 받아 중간에 중단된다.
    if (ret == -1) {
        if (errno == EINTR) {
            t.tv_sec = t2.tv_sec;
            t.tv_nsec = t2.tv_nsec;
            goto retry7;
        }
    }
}
}  // namespace cli_tetris::Test::TestCode
