#pragma once

#include "../jsystem.h"

// Texture repeat mode.
enum PTextureRepeatMode
{
    PTEXTURE_REPEAT_CLAMP,
    PTEXTURE_REPEAT_REPEAT,
    PTEXTURE_REPEAT_FLIP,
};

// Particle texture.
struct PTexture
{
    std::shared_ptr<JTexture> texture;
    PTextureRepeatMode repeatX;
    PTextureRepeatMode repeatY;

    PTexture() {}
    PTexture(std::shared_ptr<JTexture> texture, PTextureRepeatMode repeatX, PTextureRepeatMode repeatY) : texture(texture), repeatX(repeatX), repeatY(repeatY) {}
};

// Cache of textures.
struct PTextureCache
{
    std::map<std::string, std::shared_ptr<JTexture>> cache;

    PTexture Fetch(std::string texPath, PTextureRepeatMode repeatX, PTextureRepeatMode repeatY);
};