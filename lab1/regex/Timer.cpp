#include "Timer.h"

Timer::Timer() : running(false) {}

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
    running = true;
}

void Timer::stop() {
    endTime = std::chrono::high_resolution_clock::now();
    running = false;
}

double Timer::elapsedMilliseconds() {
    std::chrono::duration<double, std::milli> elapsed = (running ? std::chrono::high_resolution_clock::now() - startTime : endTime - startTime);
    return elapsed.count();
}

