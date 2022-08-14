#include "rand.h"
#include <stdlib.h>

float JRandom::RandomInRange(float min, float max)
{
    return (rand() % 1000) / 1000.0f * (max - min) + min;
}