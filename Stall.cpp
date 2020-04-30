#include "Stall.h"

Stall::Stall(int x) : Numerable(x)
{
    available = true;
}

bool Stall::get_available()
{
    std::lock_guard<std::mutex> lock(*m);
    return available;
}

void Stall::set_available(bool status)
{
    std::lock_guard<std::mutex> lock(*m);
    available = status;
}