#include "../transition.h"

#include <glm/glm.hpp>

// Color transition.
struct PTransitionColor : PTransition
{
    glm::vec3 colorStart;
    glm::vec3 colorEnd;
    int trans1Start;
    int trans2Start;
    int trans2End;
    bool useAsOptions;
    bool useAltLength;
    bool smoothTrans;

    PTransitionColor(glm::vec3 colorStart, glm::vec3 colorEnd, int trans1Start, int trans2Start, int trans2End, bool useAsOptions, bool useAltLength, bool smoothTrans) :colorStart(colorStart), colorEnd(colorEnd), trans1Start(trans1Start), trans2Start(trans2Start), trans2End(trans2End), useAsOptions(useAsOptions), useAltLength(useAltLength), smoothTrans(smoothTrans) {}
};