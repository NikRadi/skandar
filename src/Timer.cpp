#include "Timer.hpp"


void Timer::Start() {
    m_time_start = std::chrono::steady_clock::now();
}

int64 Timer::GetTimeElapsedMilliSeconds() {
    std::chrono::steady_clock::time_point m_time_stop = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_time_stop - m_time_start).count();
}
