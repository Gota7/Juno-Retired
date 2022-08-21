#include "../transition.h"

// Alpha transition.
struct PTransitionAlpha : PTransition
{
    int start;
    int middle;
    int end;
    int flicker;
    int useAltLength;
    int trans1End;
    int trans2Start;

    PTransitionAlpha(float start, float middle, float end, int flicker, int useAltLength, int trans1End, int trans2Start) : start(start), middle(middle), end(end), flicker(flicker), useAltLength(useAltLength), trans1End(trans1End), trans2Start(trans2Start) {}
};