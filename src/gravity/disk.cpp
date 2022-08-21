#include "disk.h"

RGravityDisk::RGravityDisk(glm::vec3 pos, glm::vec3 direction, glm::vec3 sideDirection, float radius, float validDegrees, bool bothSides, bool edgeGravity) : pos(pos), direction(direction), sideDirection(sideDirection), radius(radius), validDegrees(validDegrees), bothSides(bothSides), edgeGravity(edgeGravity)
{
    UpdateParams();
    UpdateMtxIdentity();
}

void RGravityDisk::UpdateParams()
{
    float theta = glm::radians(validDegrees * 0.5f);
    validCos = glm::cos(theta);
    float len = glm::dot(direction, sideDirection);
    glm::vec3 orthoNormal = direction * -len + sideDirection;
    glm::mat4 tmpMat = glm::rotate(glm::mat4(1.0f), theta, direction);
    sideDirectionOrtho = tmpMat * glm::vec4(orthoNormal, 1.0f);
}

void RGravityDisk::UpdateMtx(const glm::mat4& mtx)
{
    posTranslated = mtx * glm::vec4(pos, 1.0f);
    directionTranslated = mtx * glm::vec4(direction, 0.0f);
    sideDirectionTranslated = mtx * glm::vec4(sideDirectionOrtho, 0.0f);
    float len = glm::length(sideDirectionTranslated);
    sideDirectionTranslated = glm::normalize(sideDirectionTranslated);
    worldRadius = radius * len;
}

bool RGravityDisk::CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
{

    // Direction to position.
    glm::vec3 dirToPos = pos - posTranslated;
    float dot = glm::dot(dirToPos, directionTranslated);

    // Wrong side.
    if (dot < 0.0f && !bothSides) return false;

    // Degree is valid.
    glm::vec3 tmp = dirToPos - directionTranslated * dot;
    float len = glm::length(tmp);
    tmp = glm::normalize(tmp);
    if (validCos > -1.0f && glm::dot(tmp, sideDirectionTranslated) < validCos) return false;

    // Now find gravity dir.
    glm::vec3 gravity = glm::vec3(0.0f);
    float dist = 0.0f;
    if (len <= worldRadius)
    {
        if (dot >= 0.0f)
        {
            gravity = -direction;
        }
        else
        {
            gravity = direction;
        }
        dist = fabs(dot);
    }
    else
    {
        if (!edgeGravity) return false;
        gravity = tmp * worldRadius;
        gravity += posTranslated;
        gravity -= pos;
        dist = glm::length(gravity);
        gravity = glm::normalize(gravity);
    }

    // Clean up.
    if (!InRange(dist)) return false;
    *outDir = gravity;
    *outDist = dist;
    return true;

}

glm::vec3 RGravityDisk::RandomInRange()
{
    float min = offset;
    float max = range;
    if (max < min) max = 1000.0f; // Some arbitrary high number.
    return posTranslated + glm::normalize(glm::vec3(JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f))) * JRandom::RandomInRange(min, max);
}