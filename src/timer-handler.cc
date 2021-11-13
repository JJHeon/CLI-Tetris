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

// enum name
static constexpr unsigned int kNIF = -1;  // Nerver In Field

/* inside setting variable & method ===================================================================================== */

// Timer 개수
static constexpr unsigned int kTIMER_NUM = 10;

//원형 queue와 같은 방식이며 이름만 다르다.
static unsigned int timer_result_start_id = 0;    //원형 queue의 head
static unsigned int timer_result_current_id = 0;  //원형 queue의 tail

static bool timer_result_list[kTIMER_NUM];  // timer queue 이면서 변경 여부 확인.
static bool timer_usage[kTIMER_NUM];        // timer queue 이면서 변경 여부 확인.

static void AlarmHandler(int signal_num, siginfo_t* signal_info, void* ucontext) {
    // while ()
}

static bool IsFullTimerQueue() {
    if ((timer_result_current_id + 1 % kTIMER_NUM) == timer_result_start_id)
        return true;
    else
        return false;
}

/* TimerAccessor Class ===================================================================================== */

TimerAccessor::TimerAccessor(int id, bool* is_running_address) : id_(id), is_running_(is_running_address) {
    // is_allive 는 TimerHandler::CreateTimer에 의해 생성
}
TimerAccessor::~TimerAccessor() {}
TimerAccessor::TimerAccessor(const TimerAccessor& obj) : id_(obj.id_), is_running_(obj.is_running_), is_allive_(obj.is_allive_) {}  //복사 생성자
TimerAccessor::TimerAccessor(TimerAccessor&& obj) : id_(obj.id_), is_running_(obj.is_running_), is_allive_(obj.is_allive_) {}

/* class TimerData  ===================================================================================== */
TimerData::TimerData(std::shared_ptr<bool> accessor_allive) : accessor_allive_(std::move(accessor_allive)) {}
TimerData::TimerData(const TimerData& obj) : accessor_allive_(obj.accessor_allive_), timer_(obj.timer_) {}
TimerData::TimerData(TimerData&& obj) : accessor_allive_(obj.accessor_allive_), timer_(obj.timer_) {}
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

    // ALRM SIGNAL Capture
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    struct sigaction sigac;
    sigac.sa_flags = SA_SIGINFO;
    sigac.sa_mask = set;
    sigac.sa_sigaction = timer::AlarmHandler;
    sigaction(SIGALRM, &sigac, NULL);
}

TimerHandler::~TimerHandler() {
    is_initialize_ = false;
}

TimerAccessor&& TimerHandler::CreateTimer() {
    if (timer::IsFullTimerQueue()) throw std::runtime_error(std::string("E006 : Timer Full"));

    // key 생성
    TimerAccessor key(timer::timer_result_current_id, &timer::timer_result_list[timer_result_current_id]);
    TimerData data(std::make_shared<bool>(true));
    key.is_allive_ = data.accessor_allive_;

    // Linux System call for timer_create
    int ret = 0;
    struct sigevent evp;
    evp.sigev_value.sival_int = timer::timer_result_current_id;
    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGALRM;
    ret = timer_create(CLOCK_REALTIME, &evp, &data.timer_);  // evp가 NULL이면 SIGARM , Value는 Timer id
    if (ret) throw std::runtime_error(std::string("E007 : System call : timer_create error"));

    // key 등록
    timer_list_.insert(std::make_pair(timer::timer_result_current_id, data));
    timer::timer_result_current_id++;

    return std::move(key);
}

}  // namespace cli_tetris::timer