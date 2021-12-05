#ifndef CLI_TETRIS_THREAD_H_
#define CLI_TETRIS_THREAD_H_

#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <utility>

#include "object-defined.h"

namespace cli_tetris {

/* error Case 1 - thread 정상동작안함
template <typename T>
struct Box {
    T& one;
    std::function<void(T&)> two;
};
*/

/**
 * Thread 관리자
 * Thread-소비자 역할입니다.
 * Worker Function을 생성 단계에서 등록해주어야 합니다.
 * Worker Function은 사용자 정의로 제공되어야 합니다.
 *
 * NOTE: 본래는 object의 function<void(T&)> 형태의 function 객체를 param으로 받으려했습니다. (Object->UpdateRendering()이란 member method를 인자로 넘겨서)
 * 하지만 Thread에 function<void(T&)> 를 넘기는 작업이 잘 되지 않았습니다. Object->UpdateRendering()을 참조하는 과정에서 vtable을 참조하는 것이 아닌지,
 * 본래 의도대로 그리지 못했습니다. error Case 1 은 그때 사용했던 형태입니다.
 * <utility>의 pair는 rvalue로. function<>은 rvalue의 인자로써 전달되지 못합니다. (compile error 입니다.)
 * 따라서 현재는 생성단계에서 Object 객체의 참조자를 queue에 담아 사용하는 형태로 변경했습니다. (이전 queue는 function<>을 인자로 가짐 - error Case 1)
 */
template <typename T>
class CustomThreadManager {
   private:
    std::vector<std::thread> workers_;
    /* error Case 1 - thread 정상동작안함
    std::queue<struct Box<T>> jobs_;
    */
    std::queue<T*> jobs_;  // container는 참조자를 저장할 수 없습니다.
    std::mutex mutex_;
    std::condition_variable cv_;
    unsigned int request_count_;

   protected:
    CustomThreadManager(int num_works, std::function<void(std::mutex&, std::condition_variable&, std::queue<T*>& job, unsigned int&)> worker);
    virtual ~CustomThreadManager();

    void AddJob(T* job);
};

template <typename T>
CustomThreadManager<T>::CustomThreadManager(int num_works, std::function<void(std::mutex&, std::condition_variable&, std::queue<T*>& job, unsigned int&)> worker)
    : request_count_(0) {
    workers_.reserve(static_cast<size_t>(num_works));

    for (int i = 0; i < num_works; ++i)
        workers_.push_back(
            std::thread(
                worker,
                std::ref(mutex_),
                std::ref(cv_),
                std::ref(jobs_),
                std::ref(request_count_)));

    // Detach
    for (auto& n : workers_) n.detach();
}

template <typename T>
CustomThreadManager<T>::~CustomThreadManager() {}

template <typename T>
void CustomThreadManager<T>::AddJob(T* job) {
    // struct Box<T> n = {.one = obj, .two = func};
    jobs_.push(job);

    cv_.notify_one();
}

}  // namespace cli_tetris
#endif  // CLI_TETRIS_THREAD_H_