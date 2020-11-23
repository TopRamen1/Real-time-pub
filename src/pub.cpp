//
// Created by user on 2020-11-05.
//
#include "pub.hpp"

ClientStatus Client::get_status() const {
    return status;
}

Client::Client(int id_, int max_beers_num_, double drink_time_) : id(id_), max_beers_num(max_beers_num_),
                                                                  drink_time(drink_time_) {

}

std::string Client::get_status_str() const {
    switch (status) {
        case GOING_FOR_FIRST:
            return "Czeka w kolejce na pierwsze piwo";
        case WAITING:
            return "Czeka na nalanie";
        case DRINKING:
            return "Pije";
        case FINISHED:
            return "Skonczyl piwo";
        case WASTED:
            return "Pijany";
        case GOING_FOR_ANOTHER:
            return "Czeka w kolejce";
        case KICKED_OUT:
            return "Wyrzucony";
    }
    return "ERRROR: Nie znany stan";
}

void Client::print_status(double t) const {
    std::cout << std::fixed;
    std::cout.precision(3);
    std::cout << t << " | Id: " << id << " Status: " << get_status_str() << std::endl;
}

void Client::change_status(const ClientStatus status1) {
    status = status1;
}

void Client::drink(double t, double t_pr) {
    /// Sprawdza czy klient wypił piwo i ma pić kolejne

    if (drink_time_end <= t && beers_num == max_beers_num) {
        status = WASTED;
        drink_time_end = 0;
        print_status(t_pr);
    } else if (drink_time_end <= t && beers_num < max_beers_num) {
        status = FINISHED;
        drink_time_end = 0;
        print_status(t_pr);
    }
}

void Client::take_beer(double t, double t_pr) {
    /// Jeśli klient czeka na piwo zmienia jego status na pije i oblicza czas skończenia picia

    if (status == WAITING) {
        status = DRINKING;
        drink_time_end = t + drink_time;
        beers_num++;
        print_status(t_pr);
    }
}

Pub::Pub(int n, int max_mugs_num_, double fill_time_) : max_mugs_num(max_mugs_num_), fill_time(fill_time_) {
    /// Inicjalizacja klientów oraz dodanie ich do kolejki

    for (int i = 0; i < n; i++) {
        client_map.insert(std::pair<int, Client>(i, Client(i)));
        client_id_queue.push(i);
    }
    mugs_num = max_mugs_num;
}

ClientStatus Pub::client_status(const int id_client) const {
    return client_map.at(id_client).get_status();
}

void Pub::print_client_report(double t) const {
    /// wypisuje status wszystkich kilentów

    std::cout << "Time:" << t << std::endl;
    for (const auto &it : client_map) {
        std::cout << "Id: " << it.first << " Status: " << it.second.get_status_str() << std::endl;
    }
    std::cout << std::endl;
}

bool Pub::no_clients() const {
    /// sprawdza czy w pubie nie ma klientów

    for (auto &it : client_map) {
        if (it.second.get_status() != KICKED_OUT) {
            return false;
        }
    }
    return true;
}

void Pub::all_drink(double t) {
    /// sprawdza któży kienci skończyli pić

    for (auto id : clients_w_mugs) {
        if (clients_w_mugs.empty()) { break; }
        if (client_map.at(id).get_status() == DRINKING) {
            client_map.at(id).drink(t, get_time_now());  /// wysłąm czas kroku do obliczeń i aktualny czas do wyświetlenia
        }
    }
    /*
    for (auto &it : client_map) {
        if (it.second.get_status() == DRINKING) {
            it.second.drink(t, get_time_now());  /// wysłąm czas kroku do obliczeń i aktualny czas do wyświetlenia
        }
    }
     */
}

void Pub::take_mugs() {
    /// Odbiera kufle od klientów któży skończyli pić i dodaje ich na koniec kolejki lub wyrzuca z baru

    auto it = clients_w_mugs.begin();
    while (it != clients_w_mugs.end()) { /// iteracja whilem bo usuwamy elementy listy w trakcie iteracji

        if (client_map.at(clients_w_mugs.front()).get_status() == FINISHED) {
            mugs_num++;
            client_id_queue.push(clients_w_mugs.front()); /// dodanie klienta do kolejki
            client_map.at(clients_w_mugs.front()).change_status(GOING_FOR_ANOTHER);
            client_map.at(clients_w_mugs.front()).print_status(get_time_now());
            clients_w_mugs.erase(it++);
        } else if (client_map.at(clients_w_mugs.front()).get_status() == WASTED) {
            mugs_num++;
            client_map.at(clients_w_mugs.front()).change_status(KICKED_OUT);
            client_map.at(clients_w_mugs.front()).print_status(get_time_now());
            clients_w_mugs.erase(it++);
        } else {
            it++;
        }
    }
}

void Pub::fill_mugs(double t) {
    /// Napełnia kufle klientom w kolejce w zależniości od tego ile jest wolnych kufli

    if (fill_time_end == 0) {
        while (mugs_num > 0) {
            if (client_id_queue.empty()) { break; }
            client_map.at(client_id_queue.front()).change_status(WAITING);
            client_map.at(client_id_queue.front()).print_status(get_time_now()); /// wyświetlam aktualny czas
            clients_w_mugs.push_back(client_id_queue.front());

            fill_time_end = t + fill_time; /// do obliczeń wykożystuje czas kroku

            client_id_queue.pop(); /// usunięcie id z kolejki
            mugs_num--;
        }
    }
}

void Pub::give_beer(double t) {
    /// Wydaje napełnione kufle czekającym klientom

    if (fill_time_end <= t) {
        for (auto id : clients_w_mugs) {
            if (clients_w_mugs.empty()) { break; }
            if ( client_map.at(id).get_status() == WAITING) {
                client_map.at(id).take_beer(t, get_time_now());  /// wysłąm czas kroku do obliczeń i aktualny czas do wyświetlenia
            }
        }
        fill_time_end = 0;
    }
}

void RealTimePub::start_timer() {
    /// Restartuje czas symulacji

    start_time = clock();
    sim_time = 0;
}

void RealTimePub::update_sim_time() {
    /// Funkcja odświerzająca czas symulacj przechowyany w pamięci

    clock_t time_now_clock = clock();
    sim_time = ((double) time_now_clock - (double) start_time) / CLOCKS_PER_SEC;
}

double RealTimePub::get_time_now() const {
    /// Funkcja zwracająca aktualny czas symulacji

    return ((double) clock() - (double) start_time) / CLOCKS_PER_SEC;
}

void RealTimePub::sim() {
    /// Realizacja symulacji w czasie rzeczywistym wywołując kroki symulacji aż do zamknięcia baru
    /// Wypisuje raport stanó wszyskich klientów co sekundę

    print_client_report(0); /// raport początkowy by sprawdzić poprawność inicjalizacji klientów

    double time = clock(); /// niezależny pomiar czasu wykonania by ocenić błąd

    start_timer(); /// Urochomienie timera wewnętrznego symulacji
    update_sim_time();

    while (!no_clients()) { /// symulacja wykonuje się aż wszyscy klienci opuszą bar

        /// wywołanie kolejnych kroków symulacji

        all_drink(sim_time); /// sprawdzenie czy któryś klient skończył pić

        take_mugs(); /// odebranie kufli od klientów któży skończyli

        fill_mugs(sim_time); /// Napełnienie kufli jeśli jest taka potrzeba

        give_beer(sim_time); /// Rozdanie piw oczekującym

        update_sim_time(); /// Czas symulacji odświerzamy po wykonaniu wszyskich kroków
    }

    time = ((double) clock() - (double) time) / CLOCKS_PER_SEC; /// pomiar całkowitego czasu

    std::cout << "\nCalkowity czas wykonania symulacji: " << time << std::endl;
}


