#include "cone.h"

#include <tracy/Tracy.hpp>

RGravityCone::RGravityCone(glm::mat4 mtx, float topCutRate, bool enableBottom) : mtx(mtx), topCutRate(topCutRate), enableBottom(enableBottom)
{
    ZoneScopedN("RGravityCone::RGravityCone");
    UpdateMtxIdentity();
}

glm::vec3 RGravityCone::ProjElem(glm::vec3& a, glm::vec3& b, float* outLen)
{
    ZoneScopedN("RGravityCone::ProjElem");
    *outLen = glm::dot(a, b);
    return a + b * -(*outLen);
}

glm::vec3 RGravityCone::ProjToLine(const glm::vec3& pos, glm::vec3& p0, glm::vec3& p1)
{
    ZoneScopedN("RGravityCone::ProjToLine");
    glm::vec3 scratch = p1 - p0;
    float proj = glm::dot(scratch, pos) - glm::dot(scratch, p0);
    float coord = proj / glm::dot(scratch, scratch);
    return p0 + scratch * coord;
}

bool RGravityCone::CalcGravityFromPos(const glm::vec3& pos, glm::vec3 other, glm::vec3* outDir, float* outDist)
{
    ZoneScopedN("RGravityCone::CalcGravityFromPos");
    *outDir = other - pos;
    *outDist = glm::length(*outDir);
    if (InRange(*outDist))
    {
        *outDir /= *outDist;
        return true;
    }
    else return false;
}

glm::vec3 RGravityCone::LerpVec(glm::vec3& a, glm::vec3& b, float alpha)
{
    ZoneScopedN("RGravityCone::LerpVec");
    return a + (b - a) * alpha;
}

float RGravityCone::LerpVal(float a, float b, float alpha)
{
    ZoneScopedN("RGravityCone::LerpVal");
    return a + (b - a) * alpha;
}

void RGravityCone::UpdateMtx(const glm::mat4& mtx)
{
    ZoneScopedN("RGravityCone::UpdateMtx");
    mtxTranslated = mtx * this->mtx;
    glm::vec3 axisX(mtxTranslated[0][0], mtxTranslated[0][1], mtxTranslated[0][2]);
    magTranslated = glm::length(axisX);
}

bool RGravityCone::CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
{
    ZoneScopedN("RGravityCone::CalcOwnGravity");

    // Get Y axis.
    glm::vec3 axisY(mtxTranslated[1][0], mtxTranslated[1][1], mtxTranslated[1][2]);
    float height = glm::length(axisY);
    axisY /= height;

    // Get translation.
    glm::vec3 translation = glm::vec3(mtxTranslated[3]);
    glm::vec3 tmp = pos - translation;

    // Project on Y axis.
    float dot;
    tmp = ProjElem(tmp, axisY, &dot);

    // Check if not in center of cone.
    if (glm::length(tmp) >= 0.001f)
    {
        float dist = glm::length(tmp);
        glm::vec3 topPoint = translation + axisY * height;
        glm::vec3 bottomPoint = translation + tmp * (magTranslated / dist);

        // Top of cone.
        if (dot >= 0.0f)
        {

            // Circle instead of pointy tip cone.
            glm::vec3 tmp2(0.0f);
            if (topCutRate >= 0.01f)
            {
                // The top of the cone is a circle instead of a pointy tip, located "rate" from the top of the cone.

                // Compute the location on the circle where we're projecting. This becomes our new top point
                // for the purposes of the line projection.
                topPoint = LerpVec(topPoint, bottomPoint, topCutRate);

                // Test whether we're on the surface itself.

                // Center of the top circle.
                tmp2 = translation + axisY * LerpVal(height, 0.0f, topCutRate);

                // Test the angles to see whether we're closer to the top of the circle, or the line.
                glm::vec3 test1 = topPoint - tmp2;
                glm::vec3 test2 = pos - topPoint;

                if (glm::dot(test1, test2) <= 0.0f)
                {

                    // We're on the top surface! Compute the right distance.
                    tmp2 = pos - tmp2;
                    *outDist = glm::max(0.0f, glm::dot(axisY, tmp2));

                    if (InRange(*outDist))
                    {
                        *outDir = axisY;
                        return true;
                    }
                    else return false;

                }
            }

            glm::vec3 tmp3;
            tmp2 = ProjToLine(pos, topPoint, bottomPoint);

            if (glm::dot(tmp2 - pos, tmp2 - pos) >= 0.001f)
            {
                if (height >= 0.001f && magTranslated >= 0.001f && dist < (magTranslated - (dot * (magTranslated / height))))
                {
                    // On surface.
                    *outDir = pos - tmp2;
                    *outDist = glm::length(*outDir);
                    *outDir /= *outDist;
                    return true;
                }
                else
                {
                    return CalcGravityFromPos(pos, tmp2, outDir, outDist);
                }
            }
            else
            {
                // On surface of slanted bit of cone. Align our position towards the cone's axis line.

                // Axis of top -> bottom cone point.
                translation = topPoint - bottomPoint;
                translation = glm::normalize(translation);

                tmp = -tmp;
                float tmpLen;
                translation = ProjElem(tmp, translation, &tmpLen);
                float translationLen = glm::length(translation);
                if (translationLen >= 0.001f)
                {
                    *outDir = translation / translationLen;
                    *outDist = translationLen;
                }
                else
                {
                    // If all else fails, fall back to the the cone's direction.
                    *outDir = -axisY;
                    *outDist = 0.0f;
                }
                return true;
            }

        }

        // Bottom of cone.
        else
        {
            if (!enableBottom) return false;
            glm::vec3 tmp2 = ProjToLine(pos, translation, bottomPoint);
            if (glm::dot(tmp2 - pos, tmp2 - pos) >= 0.001f)
            {
                return CalcGravityFromPos(pos, tmp2, outDir, outDist);
            }
            else
            {
                // If all else fails, fall back to the the cone's direction.
                *outDir = -axisY;
                *outDist = 0.0f;
                return true;
            }
        }

    }
    else
    {

        // Exact center of cone, either top or bottom.
        *outDist = glm::abs(dot);
        if (dot > 0.0f)
        {
            *outDist = glm::max(0.0f, *outDist - LerpVal(0.0f, height, topCutRate));
        }
        if (InRange(*outDist))
        {
            *outDir = axisY * (dot > 0.0f ? -1.0f : 1.0f);
            return true;
        }
        else
        {
            return false;
        }

    }

}

glm::vec3 RGravityCone::RandomInRange()
{
    ZoneScopedN("RGravityCone::RandomInRange");
    glm::vec3 tmp(JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f));
    return glm::vec4(tmp, 1.0f) * mtxTranslated;
}