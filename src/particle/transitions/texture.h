#include "../transition.h"

#include "../texture.h"
#include <memory>
#include <vector>

// Texture transition.
struct PTransitionTexture : PTransition
{
    std::vector<std::shared_ptr<PTexture>> sprites;
    int numSprites;
    int interval;
    bool useAsOptions;
    bool useAltLength;

    PTransitionTexture(std::vector<std::shared_ptr<PTexture>> sprites, int numSprites, int interval, bool useAsOptions, bool useAltLength) : sprites(sprites), numSprites(numSprites), interval(interval), useAsOptions(useAsOptions), useAltLength(useAltLength) {}
};