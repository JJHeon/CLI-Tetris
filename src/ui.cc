#include "ui.h"

#include <exception>

#include "object-defined.h"

extern "C" {
#include <ncurses.h>
}

namespace cli_tetris {

namespace user_thread_worker {

static void UiWorker(std::mutex& mutex, std::condition_variable& cv, std::queue<Object*>& job, unsigned int& request_count) {
    while (true) {
        std::unique_lock<std::mutex> lock(mutex);

        cv.wait(lock,
                [&job] {
                    return !job.empty();
                });

        // auto& obj = job.front().one;
        // auto exc = job.front().two;
        auto painter = job.front();
        job.pop();

        lock.unlock();

        // excute
        // exc(obj);
        painter->UpdateRendering();
    }
}
};  // namespace user_thread_worker

Ui::Ui(int thread_workers)
    : CustomThreadManager<Object>(thread_workers, user_thread_worker::UiWorker), is_initialized(false) {
    this->Initialize();
}

Ui::~Ui() {
    if (is_initialized) this->End();
}

void Ui::Initialize() {
    is_initialized = true;
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    refresh();
    refresh();
}

void Ui::End() {
    is_initialized = false;
    endwin();
}

bool Ui::IsInitialized() const {
    return is_initialized;
}

LineColumn Ui::getScreenMaxSize() {
    if (!this->IsInitialized()) throw std::runtime_error(std::string("E005 : UI 초기화 안됨"));

    LineColumn n = {0, 0};
    getmaxyx(stdscr, n.line, n.column);

    return n;
}

void Ui::Draw(Object* object) {
    if (!this->IsInitialized()) throw std::runtime_error(std::string("E005 : UI 초기화 안됨"));

    // TODO: 여기에 object 객체를 판별해, Object 상속개체가 아니면 throw해주면 좋겠다.
    CustomThreadManager<Object>::AddJob(object);
}

void Ui::ClearScreen() {
    if (!this->IsInitialized()) throw std::runtime_error(std::string("E005 : UI 초기화 안됨"));

    clear();
}

}  // namespace cli_tetris
