#pragma once
#include "Numerable.h"
#include "Tank.h"
#include "Stall.h"
#include <thread>
#include <random>

extern bool running;

class Car : public Numerable
{
    bool carRunning;
    int status; // 0 - refuel, 1 - wait, 2 - looking for pump, 3 - paying, 4 - parking
    int progress;
    std::mt19937 rng{ std::random_device{}() };
    std::mutex* mutex; 
    Tank* tank;
    std::vector<Stall>& pumps;
    std::vector<Stall>& parking;
    int acquired_pump;
    int acquired_parking;
    void check_running();
    int ticket;
    int chosen_pump;

public:
    Car(int numb, Tank* tank, std::vector<Stall> &pumps, std::vector<Stall> &parkptr);
    void live();
    std::thread live_thread();
    void wait();
    void refuel();
    int get_progress();
    int get_status();
    int get_pump();
    int get_parking();
    void fetch_pump();
    void park();
    int get_ticket();
    int get_chosen_pump();
};