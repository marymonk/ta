#include "regex.h"
#include "Timer.h"

int main() {
    ClassInfo info;

    Timer timer;
    timer.start();

    info.processInput();
    timer.stop();

    info.printClassPairs();

    
    std::cout << "time in milliseconds: " << timer.elapsedMilliseconds() << std::endl;
    return 0;
}