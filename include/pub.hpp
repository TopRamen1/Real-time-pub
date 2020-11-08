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

enum ClientStatus {
    GOING_FOR_FIRST,
    WAITING,
    DRINKING,
    FINISHED,
    GOING_FOR_ANOTHER,
    WASTED,
    KICKED_OUT
};

class Client {
public:
    explicit Client(int id_, int max_beers_num_ = MAX_BEERS_NUM, double drink_time_ = DRINK_TIME);
    ClientStatus give_status() const;
    std::string give_status_str() const;
    double get_drink_time_end() const {return drink_time_end;}
    int get_beers_num() const {return beers_num;}
    void change_status(ClientStatus status1);
    void drink(double t);
    void take_beer(double t);


private:
    int id;
    int beers_num = 0;
    int max_beers_num;
    double drink_time_end = 0;
    double drink_time;
    ClientStatus status = GOING_FOR_FIRST;
};

class Pub {
public:
    explicit Pub(int n, int max_mugs_num_ = MAX_MUGS_NUM, double fill_time_ = FILL_TIME);
    ClientStatus client_status(int id_client) const;
    void print_client_report(double t) const;
    bool no_clients();

    void all_drink(double t);
    void take_mugs();
    void fill_mugs(double t);
    void give_beer(double t);


private:
    int mugs_num;
    double fill_time_end = 0;
    int max_mugs_num;
    double fill_time;

    std::map<int, Client> client_map;
    std::queue<int> client_id_queue;
};

class RealTimePub: public Pub {
public:
    void sim_step(double t);
    void sim();
    void sim_int(double t);

    void start_timer() { start_time = clock(); sim_time_int = 0;};
    void update_time_now();

    explicit RealTimePub(int n, int max_mugs_num_ = MAX_MUGS_NUM, double fill_time_ = FILL_TIME) : Pub(n, max_mugs_num_, fill_time_) {};

private:
    int sim_time_int = 0; /// in int seconds
    clock_t start_time = 0;
    double time_now_sec = 0;
};

#endif //PUB_PUB_HPP
