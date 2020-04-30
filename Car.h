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
    int acquired_pump;
    void check_running();

public:
    Car(int numb, Tank* tank, std::vector<Stall> &pumps);
    void live();
    std::thread live_thread();
    void wait();
    void refuel();
    int get_progress();
    int get_status();
    int get_pump();
    void fetch_pump();
};