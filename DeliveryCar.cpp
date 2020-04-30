#include "DeliveryCar.h"

DeliveryCar::DeliveryCar(Tank* tankptr)
{
    status = 1;
    this->tank = tankptr;
    mutex = new std::mutex();
}

void DeliveryCar::wait()
{
    status = 1;
    // random time between 2.5 and 3.5 seconds
    static thread_local std::uniform_real_distribution<> rand(1, 1.4);
    double time = 2.5 * 1000 * rand(rng);
    // calculating progress
    progress = time;
    for(int i = 0; i<=time; i = i+10)
    {
        mutex->lock();
        progress = time/i*100;
        mutex->unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void DeliveryCar::refuel()
{
    status = 0;

    int max_volume = tank->get_max_volume();
    // calculating progress
    progress = max_volume;

    while(tank->get_current_volume()<max_volume)
    {
        mutex->lock();
        progress = tank->get_current_volume();
        mutex->unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        tank->set_current_volume(tank->get_current_volume()+10);
    }
}

void DeliveryCar::live()
{
    do
    {
        wait();
        if(tank->get_current_volume() == 0)
            refuel();

    } while(deliveryCar_running);
    
}

std::thread DeliveryCar::live_thread()
{
    return std::thread(&DeliveryCar::live, this);
}

int DeliveryCar::get_progress()
{
    std::lock_guard<std::mutex> lock(*mutex);
    return progress;
}

int DeliveryCar::get_status()
{
    std::lock_guard<std::mutex> lock(*mutex);
    return status;
}