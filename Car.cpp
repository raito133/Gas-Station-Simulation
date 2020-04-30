#include "Car.h"

Car::Car(int numb, Tank* tankptr, std::vector<Stall> &pumpsvec) : Numerable(numb), pumps(pumpsvec)
{
    mutex = new std::mutex();
    this->tank = tankptr;
    acquired_pump = -1;
}


void Car::wait()
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

void Car::fetch_pump()
{
    status = 2;
    for(unsigned int i = 0; acquired_pump == -1; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(pumps[i].get_available() == true)
        {
            acquired_pump = pumps[i].get_number();
            pumps[i].set_available(false);
            break;
        }
        if (i == pumps.size() - 1)
            i = -1;
    }
}

void Car::refuel()
{
    
    status = 0;

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
    while(tank->get_current_volume() < 100)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    tank->set_current_volume(tank->get_current_volume() - 100);
    pumps[acquired_pump].set_available(true);
    acquired_pump = -1;
    
}

void Car::live()
{
    do
    {
        wait();
        fetch_pump();
        refuel();
        if(running == false)
        {
            if(acquired_pump != -1)
                pumps[acquired_pump].set_available(true);
            break;
        }
    
    } while(true);
}

std::thread Car::live_thread()
{
    return std::thread(&Car::live, this);
}


int Car::get_progress()
{
    std::lock_guard<std::mutex> lock(*mutex);
    return progress;
}

int Car::get_status()
{
    std::lock_guard<std::mutex> lock(*mutex);
    return status;
}

int Car::get_pump()
{
    std::lock_guard<std::mutex> lock(*mutex);
    return acquired_pump;
}