#include "texture.h"

PTexture PTextureCache::Fetch(std::string texPath, PTextureRepeatMode repeatX, PTextureRepeatMode repeatY)
{
    if (cache.find(texPath) == cache.end())
    {
        cache[texPath] = std::make_shared<JTexture>(texPath);
    }
    return PTexture(cache[texPath], repeatX, repeatY);
}