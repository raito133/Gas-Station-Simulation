#pragma once
#include <mutex>
#include <vector>

class Numerable
{
const int number;

public:
    Numerable(int x);
    std::mutex* m;
    int get_number();
};