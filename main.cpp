#include <iostream>
#include "pub.hpp"


int main() {
    std::string retry = "tak";
    while (retry == "tak") {
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
