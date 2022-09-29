#include "texture.h"

#include <tracy/Tracy.hpp>

PTexture PTextureCache::Fetch(std::string texPath, PTextureRepeatMode repeatX, PTextureRepeatMode repeatY)
{
    ZoneScopedN("PTextureCache::Fetch");

    if (cache.find(texPath) == cache.end())
    {
        cache[texPath] = std::make_shared<JTexture>(texPath);
    }
    return PTexture(cache[texPath], repeatX, repeatY);
}