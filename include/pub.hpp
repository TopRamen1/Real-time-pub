//
// Created by user on 2020-11-05.
//

#ifndef PUB_PUB_HPP
#define PUB_PUB_HPP

#include <map>
#include <queue>
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>

#define MAX_BEERS_NUM  5
#define DRINK_TIME 4
#define MAX_MUGS_NUM 10
#define FILL_TIME 1


/// Enum do łatwiejszego operowania na statusach

enum ClientStatus {
    GOING_FOR_FIRST,
    WAITING,
    DRINKING,
    FINISHED,
    GOING_FOR_ANOTHER,
    WASTED,
    KICKED_OUT
};


/// klasa klient przechowuje wszyskie dane dotyczące klienta w pubie
/// udostępnia metody wykożystywane potem w klasie Pub i RealTimePub

class Client {
public:
    explicit Client(int id_, int max_beers_num_ = MAX_BEERS_NUM, double drink_time_ = DRINK_TIME);

    ~Client() = default;

    ClientStatus get_status() const;

    std::string get_status_str() const;

    int get_id() const {return id;}

    void change_status(ClientStatus status1);

    void drink(double t, double t_pr);

    void take_beer(double t, double t_pr);

    void print_status(double t) const ;

private:
    int id;
    int beers_num = 0;
    int max_beers_num;
    double drink_time_end = 0;
    double drink_time;
    ClientStatus status = GOING_FOR_FIRST;
};


/// Klasa Pub zawiera metody wykożystywane bezpośrednio w symulacji w RealTimePub
/// Wszyskie jej metody obsługują wypisywanie każdej zmiany statusu klienta w konsoli
/// Przechowuje klientów w kontenerze std::map dla łatwiejszego dostępu

class Pub {
public:
    explicit Pub(int n, int max_mugs_num_ = MAX_MUGS_NUM, double fill_time_ = FILL_TIME);

    ~Pub() = default;

    ClientStatus client_status(int id_client) const;

    void print_client_report(double t) const;

    bool no_clients() const;

    void all_drink(double t);

    void take_mugs();

    void fill_mugs(double t);

    void give_beer(double t);

    virtual double get_time_now() const {return 0;}

private:
    int mugs_num;
    double fill_time_end = 0;
    int max_mugs_num;
    double fill_time;

    std::map<int, Client> client_map; /// mapuję klientów pod kluczem id
    std::queue<int> client_id_queue; /// kolejka klientów przechowująca id
};


/// Klasa RealTimePub Realizuje symulacje pubu w czasie rzeczywistym (metoda sim)
/// Udostępnia takrze metody i zmienne potrzene do monitorowania czasu symulacji
/// Dziedziczy po klasie Pub

class RealTimePub : public Pub {
public:
    explicit RealTimePub(int n, int max_mugs_num_ = MAX_MUGS_NUM, double fill_time_ = FILL_TIME) : Pub(n, max_mugs_num_,
                                                                                                       fill_time_) {};

    ~RealTimePub() = default;

    void sim();

    void start_timer();

    void update_sim_time();

    double get_time_now() const override;

private:
    clock_t start_time = 0;
    double sim_time = 0;
};

#endif //PUB_PUB_HPP
