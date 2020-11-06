#include <iostream>
#include "pub.hpp"


int main() {
    Pub p1 = Pub(1,15);
    p1.print_client_report(0);
    p1.sim(50);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
