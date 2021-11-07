#include "thread-manager.h"

#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <queue>
#include <vector>
#include <condition_variable>

namespace cli_tetris {

static void worker(std::mutex& mutex, std::condition_variable& cv, std::queue<std::function<void()>>& job, unsigned int& request_count) {
    while (true) {
        std::unique_lock<std::mutex> lock(mutex);

        cv.wait(lock,
                [&job] {
                    return !job.empty();
                });

        auto exc = job.front();
        job.pop();

        lock.unlock();

        // excute
        exc();
    }
}

template <typename T>
CustomThreadManager<T>::CustomThreadManager(int num_works)
    : request_count_(0) {
    for (int i = 0; i < num_works; ++i)
        workers_.push_back(
            std::thread(
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
void CustomThreadManager<T>::AddJob(std::function<void(T&)> func) {
    jobs_.push(func);

    cv_.notify_one();
}

}  // namespace cli_tetris