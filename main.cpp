#include <iostream>
#include "pub.hpp"


int main() {
    clock_t start_time = clock();

    RealTimePub p1 = RealTimePub(1,5);
    p1.sim(200);

    clock_t time_now_clock = clock();
    double time_now_sec = ((double)time_now_clock - (double)start_time)/CLOCKS_PER_SEC;
    std::cout << "Program finished in: " << time_now_sec << std::endl;
    return 0;
}
