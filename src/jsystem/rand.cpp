#include "rand.h"
#include <stdlib.h>
#include <tracy/Tracy.hpp>

float JRandom::RandomInRange(float min, float max)
{
    ZoneScopedN("JRandom::RandomInRange");
    return (rand() % 1000) / 1000.0f * (max - min) + min;
}

int JRandom::RandomInRangeW(int min, int max)
{
    ZoneScopedN("JRandom::RandomInRangeW");
    return rand() % (max - min) + min;
}