#include "cone.h"

void RGravityCone::UpdateMtx(const glm::mat4& mtx)
{
    mtxTranslated = mtx * this->mtx;
    glm::vec3 axisX(mtxTranslated[0][0], mtxTranslated[0][1], mtxTranslated[0][2]);
    magTranslated = glm::length(axisX);
}

bool RGravityCone::CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
{
    // TODO!!!
    return false;
}

glm::vec3 RGravityCone::RandomInRange()
{
    glm::vec3 tmp(JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f));
    return glm::vec4(tmp, 1.0f) * mtxTranslated;
}