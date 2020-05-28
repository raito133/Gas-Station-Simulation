#include "Car.h"

Car::Car(int numb, Tank* tankptr, std::vector<Stall> &pumpsvec, std::vector<Stall> &parkingvec) : Numerable(numb), pumps(pumpsvec), parking(parkingvec)
{
    mutex = new std::mutex();
    this->tank = tankptr;
    acquired_pump = -1;
    acquired_parking = -1;
    ticket = -1;
    chosen_pump = -1;
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
    chosen_pump = -1;
    if(pumps[0].print_ticket() > pumps[1].print_ticket())
    {
        chosen_pump = 1;
    }
    else
    {
        chosen_pump = 0;
    }
    ticket = pumps[chosen_pump].get_ticket();
    while(acquired_pump == -1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int number = pumps[chosen_pump].try_to_take_pump(ticket);
        if (number != -1)
        {
            acquired_pump = number;
            break;
        }
    }
}

void Car::park()
{
    status = 4;
    for(int i = 0; acquired_parking == -1; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int number = parking[i].try_to_take();
        if (number != -1)
        {
            acquired_parking = number;
            break;
        }
        if (i == parking.size() - 1)
            i = -1;
    }
}

void Car::refuel()
{
    
    status = 0;
    parking[acquired_parking].set_available(true);
    acquired_parking = -1;
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

    while(true)
    {
        if(tank->tank(100) == true)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    pumps[acquired_pump].finish();
    acquired_pump = -1;
    ticket = -1;
    
}

void Car::live()
{
    do
    {
        wait();
        park();
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

int Car::get_parking()
{
    std::lock_guard<std::mutex> lock(*mutex);
    return acquired_parking;
}

int Car::get_ticket()
{
    std::lock_guard<std::mutex> lock(*mutex);
    return ticket;
}

int Car::get_chosen_pump()
{
    std::lock_guard<std::mutex> lock(*mutex);
    return chosen_pump;
}