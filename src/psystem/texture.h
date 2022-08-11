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
    JTexture& texture;
    PTextureRepeatMode repeatX;
    PTextureRepeatMode repeatY;
};

// Cache of textures. TODO!!!