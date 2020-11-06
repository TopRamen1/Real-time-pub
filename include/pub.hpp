//
// Created by user on 2020-11-05.
//

#ifndef PUB_PUB_HPP
#define PUB_PUB_HPP

#include <map>
#include <queue>
#include <iostream>
#include <string>

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
    Client(int id_, double max_beers_num_ = MAX_BEERS_NUM, double drink_time_ = DRINK_TIME);
    ClientStatus give_status() const;
    std::string give_status_str() const;
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
    Pub(int id_, int n, int max_mugs_num_ = MAX_MUGS_NUM, double fill_time_ = FILL_TIME);
    ClientStatus client_status(const int id_client) const;
    void print_client_report(double t) const;
    bool no_clients();

    /// sim methods
    int sim_step(double t);
    void sim(double t);

    void take_mugs();
    void fill_mugs(double t);
    void give_beer(double t);
    void all_drink(double t);
    //void update_queue(double t);

private:
    int id;
    int mugs_num;
    double fill_time_end = 0;
    int max_mugs_num;
    double fill_time;

    std::map<int, Client> client_map;
    std::queue<int> client_id_queue;
};


#endif //PUB_PUB_HPP
