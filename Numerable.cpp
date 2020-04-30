#include "Numerable.h"

Numerable::Numerable(const int x) : number(x)
{
    m = new std::mutex();
}

int Numerable::get_number()
{
    return number;
}