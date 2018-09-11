#include "Utility.h"

int Utility::math_limit(int target_val, int min, int max)
{
    int val = target_val;

    if (target_val > max) {
        val = max;
    }

    if (target_val < min) {
        val = min;
    }

    return val;
}
