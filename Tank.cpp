#include "Tank.h"


Tank::Tank(int max) : max_volume(max)
{
    current_volume = max_volume;
    m = new std::mutex();
}

int Tank::get_max_volume()
{
    return max_volume;
}

int Tank::get_current_volume()
{
    std::lock_guard<std::mutex> lock(*m);
    return current_volume;
}

void Tank::set_current_volume(int volume)
{
    std::lock_guard<std::mutex> lock(*m);
    current_volume = volume;
    return;
}

bool Tank::tank(int volume)
{
    std::lock_guard<std::mutex> lock(*m);
    if(current_volume < 100)
    {
        return false;
    }
    else
    {
        current_volume -= 100;
        return true;
    }
    
    
}
