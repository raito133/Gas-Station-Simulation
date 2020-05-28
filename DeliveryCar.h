#pragma once
#include <thread>
#include <random>
#include <mutex>
#include "Tank.h"
#include "Stall.h"

extern bool deliveryCar_running;

class DeliveryCar
{
    int status; // 0 refueling, 1 - waiting
    int progress;
    void park();
    void refuel();
    void live();
    void wait();
    std::mt19937 rng{ std::random_device{}() };
    std::mutex* mutex; 
    Tank* tank;
    int acquired_parking;
    std::vector<Stall>& parking;


public:
    DeliveryCar(Tank* tankptr, std::vector<Stall> &parkptr);
    std::thread live_thread();
    int get_status();
    int get_progress();
    int get_parking();
};