#include "manager.h"

void RManager::AddGravity(std::unique_ptr<RGravity> gravity, unsigned int priority)
{
    if (priority != 0) gravity->priority = priority;
    gravity->managed = true;
    for (int i = 0; i < gravities.size(); i++)
    {
        if (gravity->priority >= gravities[i]->priority)
        {
            gravities.insert(gravities.begin() + i, std::move(gravity));
            return;
        }
    }
    gravities.push_back(std::move(gravity));
}

bool RManager::CalcGravity(const glm::vec3& pos, RGravityType type, glm::vec3* outDir, float* outMag, RGravityOutInfo* outInfo)
{
    glm::vec3 totalGravity(0.0f);
    bool foundGravity = false;
    float largestMag = -1.0f;
    unsigned int maxPri = 0;
    for (auto& gravity : gravities)
    {
        if (gravity->priority < maxPri) break;
        glm::vec3 grav;
        float mag;
        if (gravity->CalcGravity(pos, &grav, &mag))
        {
            bool storeInfo = false;
            if (gravity->priority == maxPri)
            {
                foundGravity = true;
                totalGravity += grav;
                if (largestMag < mag)
                {
                    storeInfo = true;
                }
            }
            else // Gravity is higher.
            {
                maxPri = gravity->priority;
                largestMag = mag;
                totalGravity = grav;
                foundGravity = true;
                storeInfo = true;
            }
            if (outInfo && storeInfo)
            {
                outInfo->gravity = gravity.get();
                outInfo->gravityDirection = grav;
                outInfo->largestPriority = gravity->priority;
            }
        }
    }
    if (outDir) *outDir = totalGravity;
    if (outMag) *outMag = largestMag;
    return foundGravity;

}