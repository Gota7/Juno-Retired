#include "cube.h"

#include <tracy/Tracy.hpp>

RGravityCube::RGravityCube(glm::mat4 mtx, RGravityCubeValidAreaFlags validAreas) : mtx(mtx), validAreas(validAreas)
{
    ZoneScopedN("RGravityCube::RGravityCube");
    UpdateMtxIdentity();
}

int RGravityCube::CalcGravityArea(const glm::vec3& pos)
{
    ZoneScopedN("RGravityCube::CalcGravityArea");

    // Get direction from translated position to current position.
    glm::vec3 dirToPos = pos - glm::vec3(mtxTranslated[3]);
    int flags = 0;

    // X distance.
    glm::vec3 axisX(mtxTranslated[0][0], mtxTranslated[0][1], mtxTranslated[0][2]);
    float distX = glm::dot(dirToPos, axisX) / extents.x;

    // X flags.
    if (distX > extents.x)
    {
        if (!(validAreas & CUBE_AREA_VALID_X_RIGHT)) return CUBE_AREA_BAD;
        flags += CUBE_AREA_X_RIGHT;
    }
    else if (distX >= -extents.x)
    {
        flags += CUBE_AREA_X_INSIDE;
    }
    else
    {
        if (!(validAreas & CUBE_AREA_VALID_X_LEFT)) return CUBE_AREA_BAD;
        flags += CUBE_AREA_X_LEFT;
    }

    // Y distance.
    glm::vec3 axisY(mtxTranslated[1][0], mtxTranslated[1][1], mtxTranslated[1][2]);
    float distY = glm::dot(dirToPos, axisY) / extents.y;

    // Y flags.
    if (distY > extents.y)
    {
        if (!(validAreas & CUBE_AREA_VALID_Y_RIGHT)) return CUBE_AREA_BAD;
        flags += CUBE_AREA_Y_RIGHT;
    }
    else if (distY >= -extents.y)
    {
        flags += CUBE_AREA_Y_INSIDE;
    }
    else
    {
        if (!(validAreas & CUBE_AREA_VALID_Y_LEFT)) return CUBE_AREA_BAD;
        flags += CUBE_AREA_Y_LEFT;
    }

    // Z distance.
    glm::vec3 axisZ(mtxTranslated[2][0], mtxTranslated[2][1], mtxTranslated[2][2]);
    float distZ = glm::dot(dirToPos, axisZ) / extents.z;

    // Z flags.
    if (distZ > extents.z)
    {
        if (!(validAreas & CUBE_AREA_VALID_Z_RIGHT)) return CUBE_AREA_BAD;
        flags += CUBE_AREA_Z_RIGHT;
    }
    else if (distZ >= -extents.z)
    {
        flags += CUBE_AREA_Z_INSIDE;
    }
    else
    {
        if (!(validAreas & CUBE_AREA_VALID_Z_LEFT)) return CUBE_AREA_BAD;
        flags += CUBE_AREA_Z_LEFT;
    }
    return flags;

}

bool RGravityCube::CalcFaceGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist, int flags)
{
    ZoneScopedN("RGravityCube::CalcFaceGravity");

    if (flags == CUBE_AREA_X_LEFT + CUBE_AREA_Y_INSIDE + CUBE_AREA_Z_INSIDE)
    {
        glm::vec3 axisX(mtxTranslated[0][0], mtxTranslated[0][1], mtxTranslated[0][2]);
        *outDir = axisX;
    }
    else if (flags == CUBE_AREA_X_INSIDE + CUBE_AREA_Y_LEFT + CUBE_AREA_Z_INSIDE)
    {
        glm::vec3 axisY(mtxTranslated[1][0], mtxTranslated[1][1], mtxTranslated[1][2]);
        *outDir = axisY;
    }
    else if (flags == CUBE_AREA_X_INSIDE + CUBE_AREA_Y_INSIDE + CUBE_AREA_Z_LEFT)
    {
        glm::vec3 axisZ(mtxTranslated[2][0], mtxTranslated[2][1], mtxTranslated[2][2]);
        *outDir = axisZ;
    }
    else if (flags == CUBE_AREA_X_RIGHT + CUBE_AREA_Y_INSIDE + CUBE_AREA_Z_INSIDE)
    {
        glm::vec3 axisX(mtxTranslated[0][0], mtxTranslated[0][1], mtxTranslated[0][2]);
        *outDir = -axisX;
    }
    else if (flags == CUBE_AREA_X_INSIDE + CUBE_AREA_Y_RIGHT + CUBE_AREA_Z_INSIDE)
    {
        glm::vec3 axisY(mtxTranslated[1][0], mtxTranslated[1][1], mtxTranslated[1][2]);
        *outDir = -axisY;
    }
    else if (flags == CUBE_AREA_X_INSIDE + CUBE_AREA_Y_INSIDE + CUBE_AREA_Z_RIGHT)
    {
        glm::vec3 axisZ(mtxTranslated[2][0], mtxTranslated[2][1], mtxTranslated[2][2]);
        *outDir = -axisZ;
    }
    else
    {
        return false;
    }

    // Normalize direction.
    float axisSize = glm::length(*outDir);
    *outDir /= axisSize;
    glm::vec3 translated = glm::vec3(mtxTranslated[3]) - pos;

    // Project onto axis.
    *outDist = glm::max(glm::dot(translated, *outDir) - axisSize, 0.0f);
    return true;

}

bool RGravityCube::CalcEdgeGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist, int flags)
{
    ZoneScopedN("RGravityCube::CalcEdgeGravity");

    glm::vec3 axis;
    glm::vec3 influence;
    glm::vec3 axisX(mtxTranslated[0][0], mtxTranslated[0][1], mtxTranslated[0][2]);
    glm::vec3 axisY(mtxTranslated[1][0], mtxTranslated[1][1], mtxTranslated[1][2]);
    glm::vec3 axisZ(mtxTranslated[2][0], mtxTranslated[2][1], mtxTranslated[2][2]);
    if (flags == CUBE_AREA_X_INSIDE + CUBE_AREA_Y_LEFT + CUBE_AREA_Z_LEFT)
    {
        axis = axisX;
        influence = -axisY - axisZ;
    }
    else if (flags == CUBE_AREA_X_LEFT + CUBE_AREA_Y_INSIDE + CUBE_AREA_Z_LEFT)
    {
        axis = axisY;
        influence = -axisX - axisZ;
    }
    else if (flags == CUBE_AREA_X_RIGHT + CUBE_AREA_Y_INSIDE + CUBE_AREA_Z_LEFT)
    {
        axis = axisY;
        influence = axisX - axisZ;
    }
    else if (flags == CUBE_AREA_X_INSIDE + CUBE_AREA_Y_RIGHT + CUBE_AREA_Z_LEFT)
    {
        axis = axisX;
        influence = axisY - axisZ;
    }
    else if (flags == CUBE_AREA_X_LEFT + CUBE_AREA_Y_LEFT + CUBE_AREA_Z_INSIDE)
    {
        axis = axisZ;
        influence = -axisX - axisY;
    }
    else if (flags == CUBE_AREA_X_RIGHT + CUBE_AREA_Y_LEFT + CUBE_AREA_Z_INSIDE)
    {
        axis = axisZ;
        influence = axisX - axisY;
    }
    else if (flags == CUBE_AREA_X_LEFT + CUBE_AREA_Y_RIGHT + CUBE_AREA_Z_INSIDE)
    {
        axis = axisZ;
        influence = -axisX + axisY;
    }
    else if (flags == CUBE_AREA_X_RIGHT + CUBE_AREA_Y_RIGHT + CUBE_AREA_Z_INSIDE)
    {
        axis = axisZ;
        influence = axisX + axisY;
    }
    else if (flags == CUBE_AREA_X_INSIDE + CUBE_AREA_Y_LEFT + CUBE_AREA_Z_RIGHT)
    {
        axis = axisX;
        influence = -axisY + axisZ;
    }
    else if (flags == CUBE_AREA_X_LEFT + CUBE_AREA_Y_INSIDE + CUBE_AREA_Z_RIGHT)
    {
        axis = axisY;
        influence = -axisX + axisZ;
    }
    else if (flags == CUBE_AREA_X_RIGHT + CUBE_AREA_Y_INSIDE + CUBE_AREA_Z_RIGHT)
    {
        axis = axisY;
        influence = axisX + axisZ;
    }
    else if (flags == CUBE_AREA_X_INSIDE + CUBE_AREA_Y_RIGHT + CUBE_AREA_Z_RIGHT)
    {
        axis = axisX;
        influence = axisY + axisZ;
    }
    else
    {
        return false;
    }

    // Orthagonalize to axis.
    glm::vec3 tmp = influence + glm::vec3(mtxTranslated[3]) - pos;
    axis = glm::normalize(axis);
    float m = glm::dot(tmp, axis);
    *outDir = tmp + axis * -m;

    // Finish output.
    if (!(*outDir == glm::vec3(0.0f)))
    {
        *outDist = glm::length(*outDir);
        *outDir /= *outDist;
        return true;
    }
    else
    {
        *outDir = glm::normalize(influence);
        *outDist = 0.0f;
        return true;
    }

}

bool RGravityCube::CalcCornerGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist, int flags)
{
    ZoneScopedN("RGravityCube::CalcCornerGravity");

    glm::vec3 tmp(0.0f);
    glm::vec3 axisX(mtxTranslated[0][0], mtxTranslated[0][1], mtxTranslated[0][2]);
    glm::vec3 axisY(mtxTranslated[1][0], mtxTranslated[1][1], mtxTranslated[1][2]);
    glm::vec3 axisZ(mtxTranslated[2][0], mtxTranslated[2][1], mtxTranslated[2][2]);
    if (flags == CUBE_AREA_X_LEFT + CUBE_AREA_Y_LEFT + CUBE_AREA_Z_LEFT)
    {
        tmp = -axisX - axisY - axisZ;
    }
    else if (flags == CUBE_AREA_X_RIGHT + CUBE_AREA_Y_LEFT + CUBE_AREA_Z_LEFT)
    {
        tmp = axisX - axisY - axisZ;
    }
    else if (flags == CUBE_AREA_X_LEFT + CUBE_AREA_Y_RIGHT + CUBE_AREA_Z_LEFT)
    {
        tmp = -axisX + axisY - axisZ;
    }
    else if (flags == CUBE_AREA_X_RIGHT + CUBE_AREA_Y_RIGHT + CUBE_AREA_Z_LEFT)
    {
        tmp = axisX + axisY - axisZ;
    }
    else if (flags == CUBE_AREA_X_LEFT + CUBE_AREA_Y_LEFT + CUBE_AREA_Z_RIGHT)
    {
        tmp = -axisX - axisY + axisZ;
    }
    else if (flags == CUBE_AREA_X_RIGHT + CUBE_AREA_Y_LEFT + CUBE_AREA_Z_RIGHT)
    {
        tmp = axisX - axisY + axisZ;
    }
    else if (flags == CUBE_AREA_X_LEFT + CUBE_AREA_Y_RIGHT + CUBE_AREA_Z_RIGHT)
    {
        tmp = -axisX + axisY + axisZ;
    }
    else if (flags == CUBE_AREA_X_RIGHT + CUBE_AREA_Y_RIGHT + CUBE_AREA_Z_RIGHT)
    {
        tmp = axisX + axisY + axisZ;
    }
    else
    {
        return false;
    }

    // Get output.
    *outDir = tmp + glm::vec3(mtxTranslated[3]) - pos;
    if (!(*outDir == glm::vec3(0.0f)))
    {
        *outDist = glm::length(*outDir);
        *outDir /= *outDist;
        return true;
    }
    else
    {
        *outDir = glm::normalize(tmp);
        *outDist = 0.0f;
        return true;
    }

}

void RGravityCube::UpdateMtx(const glm::mat4& mtx)
{
    ZoneScopedN("RGravityCube::UpdateMtx");

    mtxTranslated = mtx * this->mtx;
    glm::vec3 axisX(mtxTranslated[0][0], mtxTranslated[0][1], mtxTranslated[0][2]);
    glm::vec3 axisY(mtxTranslated[1][0], mtxTranslated[1][1], mtxTranslated[1][2]);
    glm::vec3 axisZ(mtxTranslated[2][0], mtxTranslated[2][1], mtxTranslated[2][2]);
    extents = glm::vec3(glm::dot(axisX, axisX), glm::dot(axisY, axisY), glm::dot(axisZ, axisZ));
}

bool RGravityCube::CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
{
    ZoneScopedN("RGravityCube::CalcOwnGravity");

    int flags = CalcGravityArea(pos);
    if (flags == CUBE_AREA_BAD) return false;
    if (!CalcFaceGravity(pos, outDir, outDist, flags))
    {
        if (!CalcEdgeGravity(pos, outDir, outDist, flags))
        {
            if (!CalcCornerGravity(pos, outDir, outDist, flags))
            {
                return false;
            }
        }
    }
    if (InRange(*outDist)) return true;
    else return false;
}

glm::vec3 RGravityCube::RandomInRange()
{
    ZoneScopedN("RGravityCube::RandomInRange");

    float mag = range >= 0 ? range : 6.0f;
    glm::vec3 tmp(JRandom::RandomInRange(-mag, mag), JRandom::RandomInRange(-mag, mag), JRandom::RandomInRange(-mag, mag));
    return glm::vec4(tmp, 1.0f) * mtxTranslated;
}