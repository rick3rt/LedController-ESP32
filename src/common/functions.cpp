#include "functions.hpp"

void constrainDir(int min, int max, int &x, int &dir)
{
    if (x >= max)
    {
        x = max;
        dir = -1;
    }
    else if (x <= min)
    {
        x = min;
        dir = 1;
    }
}
