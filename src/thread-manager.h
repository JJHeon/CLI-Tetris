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

// Thread 관리자
template <typename T>
class CustomThreadManager {
   private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void(T&)>> jobs_;
    std::mutex mutex_;
    std::condition_variable cv_;
    unsigned int request_count_;

   protected:
    CustomThreadManager(int num_works);
    virtual ~CustomThreadManager();


    void AddJob(std::function<void(T&)> func);
};

}  // namespace cli_tetris
#endif  // CLI_TETRIS_THREAD_H_