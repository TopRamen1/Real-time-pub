#include <iostream>
#include "pub.hpp"


int main() {
    char retry = 'Y';
    while (retry == 'Y') {
        int n;
        std::cout << "Podaj ilosc klientow: ";
        std::cin >> n;
        std::cout << '\n';

        RealTimePub p1 = RealTimePub(n);
        p1.sim();

        std::cout << "Chcesz otworzyc pub ponownie?(tak/nie): ";
        std::cin >> retry;
        std::cout << '\n';
    }
    return 0;
}
