#include <iostream>

#include "game-module.h"
#include "service-manager.h"
#include "ui.h"

extern "C" {
#include <signal.h>
#include <time.h>
}

void TimerTestcode();

int main(void) {
    using cli_tetris::GameManager;
    using cli_tetris::Locator;
    using cli_tetris::StateCode;
    using cli_tetris::Ui;
    using cli_tetris::timer::TimerHandler;

    /**
     *  Ui -
     *  ncurse 화면 초기화와 관련된 함수
     *  한번만 초기화 되어야 함.
     *
     *  TimerHandler - linux system call 포함
     *  한번만 초기화 되어야 함.
     * */
    // Locator::provideUi(new Ui{1});
    Locator::provideTimerHandler(new TimerHandler);

    //* TestCode for Timer */
    TimerTestcode();

    /* Tetris Manager */
    // GameManager tetris(Locator::getUi(), StateCode::kStart);

    /*
    try {
        tetris.Initialize();
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    if (!tetris.CheckGameState()) return -1;
    tetris.Run();
    */
    // Finish ncurse, timer
    // Locator::releaseUi();
    Locator::releaseTimerHandler();

    return 0;
}

void TimerTestcode() {
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