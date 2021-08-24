#ifndef SKANDAR_TIMER_HPP
#define SKANDAR_TIMER_HPP
#include "Common.hpp"
#include <chrono>


class Timer {
public:
    void Start();
    int64 GetTimeElapsedMilliSeconds();

private:
    std::chrono::steady_clock::time_point m_time_start;

};

#endif // SKANDAR_TIMER_HPP