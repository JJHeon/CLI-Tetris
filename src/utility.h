#ifndef CLI_TETRIS_UTILITY_H_
#define CLI_TETRIS_UTILITY_H_

namespace cli_tetris::utility {

template <typename T>
class PointerQueue {
   private:
    std::queue<T*> queue_;
    const int limit_queue_num_;

   private:
    bool IsFullQueue() const;

   public:
    PointerQueue(const int& limit_queue);
    virtual ~PointerQueue();
    void ResetQueue();
    bool Put(T* command);
    T* Get();
};

template <typename T>
void PointerQueue<T>::ResetQueue() {
    while (!queue_.empty()) {
        T* ptr = queue_.front();
        queue_.pop();
        delete ptr;
    }
}
template <typename T>
bool PointerQueue<T>::Put(T* pointer) {
    if (IsFullQueue()) return false;

    queue_.push(pointer);
    return true;
}
template <typename T>
T* PointerQueue<T>::Get() {
    if (queue_.empty()) return nullptr;

    T* ptr = queue_.front();
    queue_.pop();

    return ptr;
}
template <typename T>
bool PointerQueue<T>::IsFullQueue() const {
    if (limit_queue_num_ == queue_.size())
        return true;
    else
        return false;
}
template <typename T>
PointerQueue<T>::PointerQueue(const int& limit_queue)
    : limit_queue_num_(limit_queue) {
}
template <typename T>
PointerQueue<T>::~PointerQueue() {
    this->ResetQueue();
}
}  // namespace cli_tetris::utility

#endif  // CLI_TETRIS_UTILITY_H_