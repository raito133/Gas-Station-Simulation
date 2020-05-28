#pragma once
#include "Numerable.h"

class Stall : public Numerable
{
    bool available;
    int ticket;
    int current_ticket;

public:

    Stall(int x);
    bool get_available();
    void set_available(bool status);
    int try_to_take();
    int try_to_take_pump(int next);
    int get_ticket();
    int finish();
    int print_current_ticket();
    int print_ticket();
};