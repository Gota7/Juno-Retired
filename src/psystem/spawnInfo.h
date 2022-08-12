#pragma once

#include "texture.h"
#include <glm/glm.hpp>
#include <memory>

// Particle spawn shape.
enum PSpawnShapeEnum
{
    P_SPAWNSHAPE_POINT,
    P_SPAWNSHAPE_SPHERE,
    P_SPAWNSHAPE_CIRCLE_RANDOM,
    P_SPAWNSHAPE_CIRCLE_EVEN,
    P_SPAWNSHAPE_BALL,
    P_SPAWNSHAPE_DISC
};

// Particle draw mode.
enum PDrawModeEnum
{
    P_DRAWMODE_3D,
    P_DRAWMODE_STRETCH,
    P_DRAWMODE_BILLBOARD
};

// Particle plane.
enum PParticlePlane
{
    P_PARTICLEPLANE_X,
    P_PARTICLEPLANE_Y,
    P_PARTICLEPLANE_Z,
    P_PARTICLEPLANE_DIR
};

// Particle spawn info.
struct PSpawnInfo
{
    PSpawnShapeEnum spawnShape; // Shape to spawn from.
    PDrawModeEnum drawMode; // How particle is drawn.
    PParticlePlane plane; // If shape is flat, spawn perpendicular to this axis.
    bool rotate; // Particle can rotate.
    bool randomInitAngle; // Have a random rotation angle instead of 0 deg.
    bool followSystem; // If to follow parent system when particle moves.
    bool allNegativeAxis; // If axis is <-1, -1, -1>.
    bool horizontalIf3d; // If rendering 3d do horizontal or verticle.
    bool selfDestruct; // If the particle can self destruct.
    float spawnRate; // How many particles to spawn each frame.
    float maxDist; // Initial maximum distance from system's position.
    glm::vec3 dir; // Direction the system points.
    glm::vec3 color; // Color of particles.
    float perpSpeed; // Speed away from particle system's direction.
    float speed; // Speed in particle system's direction.
    float scale; // Half the side length of the particle square.
    float horizontalScaleMultiplier; // What to multiply the horizontal scale by.
    float minAngularSpeed; // Minimum angular speed.
    float maxAngularSpeed; // Maximum angular speed.
    unsigned int frames; // How many frames the system can spawn particles for.
    unsigned int scaleRandomness; // Varies random scale of particles.
    unsigned int lifetime; // How many frames particles exist for.
    unsigned int lifetimeRandomness; // Random lifetime of particles.
    unsigned int speedRandomness; // Random speed of particles.
    unsigned int spawnPeriod; // Just set to frames + 1.
    unsigned int alpha; // How opaque particles are.
    unsigned int speedFalloff; // How fast speed decelerates or accelerates.
    unsigned int altLength; // Alternate number of frames to use for transitions instead of lifetime.
    float velStretchFactor; // How much to stretch by velocity.
    unsigned int logTexRepeatHorizontal; // Repeat texture in X direction count.
    unsigned int logTexRepeatVertical; // Repeat texture in Y direction count.
    PTexture texture; // Texture to use.

    // Load from a YAML file.
    void Load(PTextureCache& texCache, std::string name);

};