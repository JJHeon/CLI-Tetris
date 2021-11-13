#include "timer-handler.h"

#include <exception>
#include <memory>
#include <string>
#include <cassert>
#include <utility>

#define _POSIX_C_SOURCE 199309L
#include <signal.h>
#include <time.h>
extern "C" {
}

namespace cli_tetris::timer {

namespace linux_call {
// enum name
static constexpr unsigned int kNIF = -1;  // Nerver In Field

/* inside setting variable & method ===================================================================================== */

// Timer 개수
static constexpr unsigned int kTIMER_NUM = 10;

// timer 수신 결과 여부 확인
static bool timer_result_list[kTIMER_NUM] = {
    false,
};

// timer 사용 여부 확인.
static bool timer_usage[kTIMER_NUM] = {
    false,
};

static void AlarmHandler(int signal_num, siginfo_t* signal_info, void* ucontext) {
    if (signal_num == SIGALRM) {
        int id = signal_info->_sifields._rt.si_sigval.sival_int;
        timer_result_list[id] = false;
    }
}

static void CaptureAlarmSignal() {
    // ALRM SIGNAL Capture
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    struct sigaction sigac;
    sigac.sa_flags = SA_SIGINFO;
    sigac.sa_mask = set;
    sigac.sa_sigaction = timer::linux_call::AlarmHandler;
    sigaction(SIGALRM, &sigac, NULL);
}

static void InitializeTimerVariable() {
    // Global variable init again
    for (int i = 0; i < kTIMER_NUM; ++i) {
        timer_result_list[i] = false;
        timer_usage[i] = false;
    }
}

static bool IsFullTimerQueue() {
    for (unsigned int i = 0; i <= kTIMER_NUM; ++i) {
        if (!timer_usage[i]) return false;
    }
    return true;
}

static int GetTimerID() {
    for (unsigned int i = 0; i <= kTIMER_NUM; ++i) {
        if (!timer_usage[i]) {
            timer_usage[i] = true;
            return i;
        }
    }
}
static void DeleteTimer(const int& id, timer_t& timer) {
    if (id < 0 || id >= kTIMER_NUM) throw std::out_of_range(std::string("ReturnTimerID out of bound"));
    timer_usage[id] = false;
    timer_result_list[id] = false;
    timer_delete(timer);
}

static void CreateTimer(const int& id, timer_t& timer) {
    int ret = 0;
    struct sigevent evp;
    evp.sigev_value.sival_int = id;
    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGALRM;
    ret = timer_create(CLOCK_REALTIME, &evp, &timer);  // evp가 NULL이면 SIGARM , Value는 Timer id
    if (ret) throw std::runtime_error(std::string("Linux System call : timer_create error"));
}

static void RunTimer(const int& id, timer_t& timer, const int& sec, const int& nanosec) {
    if (id < 0 || id >= kTIMER_NUM) throw std::out_of_range(std::string("RunTimer out of bound"));

    timer_result_list[id] = true;

    struct itimerspec ts;
    int ret;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = sec;
    ts.it_value.tv_nsec = nanosec;
    ret = timer_settime(timer, 0, &ts, NULL);
    if (ret) throw std::runtime_error(std::string("Linux System call : timer_settime error"));
}

static void StopTimer(const int& id, timer_t& timer) {
    struct itimerspec ts;
    int ret;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = 0;
    ret = timer_settime(timer, 0, &ts, NULL);

    timer_result_list[id] = false;
}

}  // namespace linux_call

/* TimerAccessor Class ===================================================================================== */

TimerAccessor::TimerAccessor(int id, bool* is_running_address) : id_(id), is_running_(is_running_address) {
    // is_allive 는 TimerHandler::CreateTimer에 의해 생성
}
TimerAccessor::~TimerAccessor() {}
TimerAccessor::TimerAccessor(const TimerAccessor& obj) : id_(obj.id_), is_running_(obj.is_running_), is_allive_(obj.is_allive_) {}  //복사 생성자
TimerAccessor::TimerAccessor(TimerAccessor&& obj) noexcept : id_(obj.id_), is_running_(obj.is_running_), is_allive_(obj.is_allive_) {}

bool TimerAccessor::IsRunning() const {
    return *is_running_;
}
bool TimerAccessor::IsAlive() const {
    std::shared_ptr<bool> p = is_allive_.lock();
    if (p) {
        return *p;
    } else
        return false;
}

/* class TimerData  ===================================================================================== */
TimerData::TimerData(std::shared_ptr<bool> accessor_allive) : accessor_allive_(std::move(accessor_allive)) {}
TimerData::TimerData(const TimerData& obj) : accessor_allive_(obj.accessor_allive_), timer_(obj.timer_) {}
TimerData::TimerData(TimerData&& obj) noexcept : accessor_allive_(obj.accessor_allive_), timer_(obj.timer_) {}
TimerData& TimerData::operator=(const TimerData& obj) {
    this->accessor_allive_ = obj.accessor_allive_;
    this->timer_ = obj.timer_;

    return *this;
}

/* TimerHandler Class ===================================================================================== */
bool TimerHandler::is_initialize_ = false;

TimerHandler::TimerHandler() {
    // Constructor only have one
    assert(is_initialize_);
    is_initialize_ = true;

    // Signal System call
    linux_call::CaptureAlarmSignal();
    // Initialize
    linux_call::InitializeTimerVariable();
}

TimerHandler::~TimerHandler() {
    is_initialize_ = false;
}

TimerAccessor&& TimerHandler::CreateTimer() {
    if (linux_call::IsFullTimerQueue()) throw std::runtime_error(std::string("E006 : Timer Full"));

    // key 생성
    int id = linux_call::GetTimerID();
    TimerAccessor key(id, &linux_call::timer_result_list[id]);
    TimerData data(std::make_shared<bool>(true));
    key.is_allive_ = data.accessor_allive_;

    // Linux System call for timer_create
    linux_call::CreateTimer(id, data.timer_);

    // key 등록
    timer_list_.insert(std::make_pair(id, data));

    return std::move(key);
}

void TimerHandler::DeleteTimer(TimerAccessor& accessor) {
    auto itr = timer_list_.find(accessor.id_);
    if (itr != timer_list_.end()) {
        // System call
        linux_call::DeleteTimer(accessor.id_, itr->second.timer_);

        timer_list_.erase(itr);
        return;
    }

    else
        throw std::runtime_error(std::string("E007 : 지우려는 Timer 없음"));
}

void TimerHandler::SetTimer(TimerAccessor& accessor, const int& sec, const int& nanosec) {
    auto itr = timer_list_.find(accessor.id_);
    if (itr != timer_list_.end()) {
        linux_call::RunTimer(accessor.id_, itr->second.timer_, sec, nanosec);

        return;
    }

    throw std::runtime_error(std::string("E008 : 시작하려는 Timer 없음"));
    return;
}
void TimerHandler::StopTimer(TimerAccessor& accessor) {
    auto itr = timer_list_.find(accessor.id_);
    if (itr != timer_list_.end()) {
        linux_call::StopTimer(accessor.id_, itr->second.timer_);

        return;
    }

    throw std::runtime_error(std::string("E008 : 멈추려는 Timer 없음"));
    return;
}

}  // namespace cli_tetris::timer