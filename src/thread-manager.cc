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
        if (!job->empty()) {
            auto exc = job->front();

            job->pop();

            // Drawing UI
            exc();
        }
    }
}

CustomThreadManager::CustomThreadManager(int num_works)
    : request_count_(0) {
    for (int i = 0; i < num_works; ++i)
        workers_.push_back(
            std::thread(
                std::ref(mutex_),
                std::ref(cv_),
                std::ref(jobs_),
                std::ref(request_count_)));
}

}  // namespace cli_tetris