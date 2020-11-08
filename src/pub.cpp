//
// Created by user on 2020-11-05.
//
#include "pub.hpp"

ClientStatus Client::give_status() const {
    return status;
}

Client::Client(int id_, int max_beers_num_, double drink_time_) : id(id_), max_beers_num(max_beers_num_), drink_time(drink_time_) {

}

std::string Client::give_status_str() const {
    switch (status) {
        case GOING_FOR_FIRST:
            return "Czeka w kolejce na pierwsze piwo";
        case WAITING:
            return "Czeka na nalanie";
        case DRINKING:
            return "Pije";
        case FINISHED:
            return "Skończyl piwo";
        case WASTED:
            return "Pijany";
        case GOING_FOR_ANOTHER:
            return "Czeka w kolejce";
        case KICKED_OUT:
            return "Wyrzucony";
    }
    return "ERRROR: Nie znany stan";
}

void Client::change_status(ClientStatus status1) {
    status = status1;
}

void Client::drink(double t) {
    /// Sprawdza czy klient wypił piwo i ma pić kolejne
    if (drink_time_end <= t && beers_num == max_beers_num) {
        status = WASTED;
        drink_time_end = 0;
    } else if (drink_time_end <= t && beers_num < max_beers_num) {
        status = FINISHED;
        drink_time_end = 0;
    }
}

void Client::take_beer(double t) {
    /// Jeśli klient czeka na piwo zmienia jego status na pije i oblicza czas skończenia picia
    if(status == WAITING) {
        status = DRINKING;
        drink_time_end = t + drink_time;
        beers_num++;
    }
}

Pub::Pub(int n, int max_mugs_num_, double fill_time_)  : max_mugs_num(max_mugs_num_), fill_time(fill_time_) {
    /// Inicjalizacja klientów oraz dodanie ich do kolejki
    for(int i = 0 ; i < n ; i++) {
        client_map.insert(std::pair<int,Client>(i,Client(i)));
        client_id_queue.push(i);
    }
    mugs_num = max_mugs_num;
}

ClientStatus Pub::client_status(const int id_client) const {
    return client_map.at(id_client).give_status();
}

void Pub::print_client_report(double t) const {
    /// wypisuje status wszystkich kilentów
    std::cout << "Time:" << t << std::endl;
    for(const auto & it : client_map) {
        std::cout << "Id: " << it.first << " Status: " << it.second.give_status_str() << std::endl;
    }
    std::cout << std::endl;
}

bool Pub::no_clients() {
    /// sprawdza czy w pubie nie ma klientów
    for(auto & it : client_map) {
        if(it.second.give_status() != KICKED_OUT) {
            return false;
        }
    }
    return true;
}

void Pub::all_drink(double t) {
    /// sprawdza któży kienci skończyli pić
    for(auto & it : client_map) {
        if(it.second.give_status() == DRINKING) {
            it.second.drink(t);
        }
    }
}

void Pub::take_mugs() {
    /// Odbiera kufle od klientów któży skończyli pić i dodaje ich na koniec kolejki lub wyrzuca z baru
    for(auto & it : client_map) {
        if(it.second.give_status() == FINISHED) {
            mugs_num ++;
            client_id_queue.push(it.first); /// queue update
            it.second.change_status(GOING_FOR_ANOTHER);
        } else if (it.second.give_status() == WASTED) {
            mugs_num ++;
            it.second.change_status(KICKED_OUT);
        }
    }
}

void Pub::fill_mugs(double t) {
    /// Napełnia kufle klientom w kolejce w zależniości od tego ile jest wolnych kufli
    if(fill_time_end == 0) {
        while (mugs_num > 0) {
            if(client_id_queue.empty()){ break;}
            client_map.at(client_id_queue.front()).change_status(WAITING);
            fill_time_end = t + fill_time;
            client_id_queue.pop();
            mugs_num--;
        }
    }
}

void Pub::give_beer(double t) {
    /// Wydaje napełnione kufle czekającym klientom
    if(fill_time_end <= t) {
        for(auto & it : client_map) {
            if(it.second.give_status() == WAITING) {
                it.second.take_beer(t);
            }
        }
        fill_time_end = 0;
    }
}

void RealTimePub::sim_step(double t) {
    /// Realizacja jednego kroku w symulacji poprzez sprawdzenie wszyskich funkcji realizujących krok
    if(no_clients()) {return;}
    all_drink(t);
    take_mugs();
    fill_mugs(t);
    give_beer(t);
}

void RealTimePub::start_timer() {
    /// Restartuje czas symulacji
    start_time = clock();
    sim_time_int = 0;
}

void RealTimePub::update_time_now() {
    /// Funkcja odświerzająca czas symulacj
    clock_t time_now_clock = clock();
    time_now_sec = ((double)time_now_clock - (double)start_time)/CLOCKS_PER_SEC;
}

void RealTimePub::sim() {
    /// Realizacja symulacji w czasie rzeczywistym wywołując kroki symulacji aż do zamknięcia baru
    /// Wypisuje raport stanó wszyskich klientów co sekundę
    print_client_report(0);

    start_timer();
    update_time_now();
    while(!no_clients()) {
        sim_step(time_now_sec);

        if(time_now_sec - sim_time_int >= 1) {
            print_client_report(time_now_sec);
            sim_time_int++;
        }

        update_time_now();
    }
}

void RealTimePub::sim_int(double t) {
    /// Symulacja poprzez iterację po sekundach w int (do szybkiego debugowania)
    for (int i = 0 ; i < t ; i++) {
        if(no_clients()) {
            return;
        }
    }
}

