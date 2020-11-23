#include <iostream>
#include "pub.hpp"


int main() {
    std::string retry = "tak";

    std::cout << "\nProgram symulujacy pub\n" << std::endl;

    while (retry == "tak") {
        int n;
        std::cout << "Podaj ilosc klientow: ";
        std::cin >> n;
        std::cout << '\n';

        RealTimePub p1 = RealTimePub(n); /// inicializacja pubu

        p1.sim(); /// Uruchomienie symulacji

        std::cout << "\nChcesz otworzyc pub ponownie?(tak/nie): ";
        std::cin >> retry;
        std::cout << '\n';
    }
    return 0;
}
