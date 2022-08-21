#include "torus.h"

RGravityTorus::RGravityTorus(glm::vec3 pos, glm::vec3 dir, float radius, float diskRadius, RGravityTorusEdgeType edgeType, bool bothSides) : pos(pos), dir(dir), radius(radius), diskRadius(diskRadius), edgeType(edgeType), bothSides(bothSides)
{
    UpdateMtxIdentity();
}

void RGravityTorus::UpdateMtx(const glm::mat4& mtx)
{
    posTranslated = mtx * glm::vec4(pos, 1.0f);
    dirTranslated = mtx * glm::vec4(dir, 0.0f);
    float len = glm::length(dirTranslated);
    dirTranslated /= len;
    radiusTranslated = radius * len;
}

bool RGravityTorus::CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
{

    // Direction to position.
    glm::vec3 dirToPos = pos - posTranslated;
    float dot = glm::dot(dirToPos, dirTranslated);

    // Bad side.
    if (dot < 0.0f && !bothSides) return false;

    // Modify.
    glm::vec3 tmp = dirTranslated * dot;
    tmp = dirToPos - tmp;
    float length = glm::length(tmp);
    tmp /= length;

    // Near zero check.
    if (glm::abs(length) < 0.001f)
    {
        glm::vec3 unitZ(0.0f, 0.0f, 1.0f);
        float m = glm::dot(unitZ, dirTranslated);
        tmp = unitZ + dirTranslated * -m;
        if (glm::length(tmp) < 0.001f)
        {
            glm::vec3 unitX(1.0f, 0.0f, 0.0f);
            m = glm::dot(unitX, dirTranslated);
            tmp = unitX + dirTranslated * -m;
        }
        tmp = glm::normalize(tmp);
    }

    // Main math.
    glm::vec3 dir;
    if (length >= radiusTranslated)
    {
        if (!(edgeType & TORUS_TYPE_OUTSIDE)) return false;
        dir = posTranslated + tmp * radiusTranslated;
        dir -= pos;
        *outDist = glm::length(dir);
        *outDir = dir / *outDist;
    }
    else if (length >= (radiusTranslated - diskRadius))
    {
        if (dot >= 0.0f) *outDir = -dirTranslated;
        else *outDir = dirTranslated;
        *outDist = glm::abs(dot);
    }
    else
    {
        if (!(edgeType & TORUS_TYPE_INSIDE)) return false;
        dir = posTranslated + tmp * (radiusTranslated - diskRadius);
        dir -= pos;
        *outDist = glm::length(dir);
        *outDir = dir / *outDist;
    }
    if (!InRange(*outDist)) return false;
    else return true;

}

glm::vec3 RGravityTorus::RandomInRange()
{
    return posTranslated + glm::vec3(JRandom::RandomInRange(-range, range), JRandom::RandomInRange(-range, range), JRandom::RandomInRange(-range, range));
}