#include "Stall.h"

Stall::Stall(int x) : Numerable(x)
{
    available = true;
    ticket = -1;
    current_ticket = 0;
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

int Stall::try_to_take()
{
    std::lock_guard<std::mutex> lock(*m);
    if(available == false)
        return -1;
    else
    {
        available = false;
        return get_number();
    }
}

int Stall::finish()
{
    std::lock_guard<std::mutex> lock(*m);
    available = true;
    current_ticket++;
}

int Stall::try_to_take_pump(int ticket)
{
    std::lock_guard<std::mutex> lock(*m);
    if(available == false)
    {
        return -1;
    }
    else
    {
        if(this->current_ticket == ticket)
        {
            available = false;
            return get_number();
        }
        else
        {
            return -1;
        }
        
    }
}

int Stall::get_ticket()
{
    std::lock_guard<std::mutex> lock(*m);
    if(ticket == -1)
    {
        ticket = 0;
        return 0;
    }
    else
    {
        ticket++;
        return ticket;
    }

}

int Stall::print_ticket()
{
    std::lock_guard<std::mutex> lock(*m);
    return ticket;
}

int Stall::print_current_ticket()
{
    std::lock_guard<std::mutex> lock(*m);
    return current_ticket;

}