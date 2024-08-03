#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    Timer();
    void start();
    void stop(); 
    double elapsedMilliseconds();

private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    bool running;
};

#endif // TIMER_H
