#ifndef CLI_TETRIS_THREAD_H_
#define CLI_TETRIS_THREAD_H_

#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

#include "object-defined.h"

namespace cli_tetris {

// ThreadPool 처럼 만들 계획.
class CustomThreadManager {
   private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> jobs_;
    std::mutex mutex_;
    std::condition_variable cv_;
    unsigned int request_count_;

   public:
    CustomThreadManager(int num_works);
    virtual ~CustomThreadManager();
};

}  // namespace cli_tetris
#endif  // CLI_TETRIS_THREAD_H_