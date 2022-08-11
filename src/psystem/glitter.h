#pragma once

#include "spawnInfo.h"

// Angle copy mode.
enum PGlitterAngleCopyMode
{
    PGLITTER_ANGLECOPY_NONE,
    PGLITTER_ANGLECOPY_ANGLE,
    PGLITTER_ANGLECOPY_ANGLE_AND_SPEED
};

// Glitter.
struct PGlitter
{
    PGlitterAngleCopyMode angleCopyMode;
    bool hasEffects;
    bool hasScaleTrans;
    bool hasAlphaTrans;
    bool followSystem;
    bool useGlitterColor;
    PDrawModeEnum drawMode;
    float speedRand;
    float scale;
    int lifetime;
    int speedMult;
    int scaleMult;
    glm::vec3 color;
    int rate;
    int wait;
    int period = 1;
    std::shared_ptr<PTexture> sprite;
    int logTexRepeatHorz;
    int logTexRepeatVert;
};