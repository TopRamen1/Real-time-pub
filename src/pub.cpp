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
            return "going for first";
        case WAITING:
            return "waiting";
        case DRINKING:
            return "drinking";
        case FINISHED:
            return "finished his beer";
        case WASTED:
            return "wasted";
        case GOING_FOR_ANOTHER:
            return "going for another";
        case KICKED_OUT:
            return "kicked out";
    }
    return "err";
}

void Client::change_status(ClientStatus status1) {
    status = status1;
}

void Client::drink(double t) {
    if (drink_time_end <= t && beers_num == max_beers_num) {
        status = WASTED;
        drink_time_end = 0;
    } else if (drink_time_end <= t && beers_num < max_beers_num) {
        status = FINISHED;
        drink_time_end = 0;
    }
}

void Client::take_beer(double t) {
    if(status == WAITING) {
        status = DRINKING;
        drink_time_end = t + drink_time;
        beers_num++;
    }
}

Pub::Pub(int n, int max_mugs_num_, double fill_time_)  : max_mugs_num(max_mugs_num_), fill_time(fill_time_) {
    // client list and queue initialization
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
    std::cout << "Time:" << t << std::endl;
    for(const auto & it : client_map) {
        std::cout << "Id: " << it.first << " " << it.second.give_status_str() << " DrinkT:" << it.second.get_drink_time_end() << " FillT:" << fill_time_end << std::endl;
    }
    std::cout << std::endl;
}

bool Pub::no_clients() {
    for(auto & it : client_map) {
        if(it.second.give_status() != KICKED_OUT) {
            return false;
        }
    }
    return true;
}

void Pub::all_drink(double t) {
    for(auto & it : client_map) {
        if(it.second.give_status() == DRINKING) {
            it.second.drink(t);
        }
    }
}

void Pub::take_mugs() {
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
    if(no_clients()) {return;}
    all_drink(t);
    take_mugs();
    fill_mugs(t);
    give_beer(t);
    if(t - sim_time_int >= 1) {
        print_client_report(t);
        sim_time_int++;
    }
}

void RealTimePub::sim_int(double t) {
    for (int i = 0 ; i < t ; i++) {
        if(no_clients()) {
            return;
        }
    }
}

void RealTimePub::update_time_now() {
    clock_t time_now_clock = clock();
    time_now_sec = ((double)time_now_clock - (double)start_time)/CLOCKS_PER_SEC;
}

void RealTimePub::sim() {
    print_client_report(0);
    start_timer();

    update_time_now();
    while(!no_clients()) {
        sim_step(time_now_sec);

        update_time_now();
    }
    print_client_report(time_now_sec);

}


