#include <iostream>
#include "pub.hpp"


int main() {
    char retry = 'Y';
    while (retry == 'Y') {
        int n;
        std::cout << "Enter number of clients: ";
        std::cin >> n;
        std::cout << '\n';

        RealTimePub p1 = RealTimePub(n);
        p1.sim();

        std::cout << "Do you want to open pub again(Y/n): ";
        std::cin >> retry;
        std::cout << '\n';
    }
    return 0;
}
