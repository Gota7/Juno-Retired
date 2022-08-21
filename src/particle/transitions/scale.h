#include "../transition.h"

// Scale transition.
struct PTransitionScale : PTransition
{
    float start;
    float middle;
    float end;
    int trans1End;
    int trans2Start;
    bool useAltLength;

    PTransitionScale(float start, float middle, float end, int trans1End, int trans2Start, bool useAltLength) : start(start), middle(middle), end(end), trans1End(trans1End), trans2Start(trans2Start), useAltLength(useAltLength) {}
};