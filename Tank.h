#pragma once
#include <thread>
#include <mutex>

class Tank
{
    const int max_volume;
    int current_volume;

public:

    Tank(int max);
    std::mutex* m;
    int get_max_volume();
    int get_current_volume();
    void set_current_volume(int volume);
    void refuel();

};