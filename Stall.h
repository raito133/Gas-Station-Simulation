#pragma once
#include "Numerable.h"

class Stall : public Numerable
{
    bool available;

public:

    Stall(int x);
    bool get_available();
    void set_available(bool status);
};