#include "followCam.h"

#include <tracy/Tracy.hpp>

void GFollowCam::Update()
{
    ZoneScopedN("GFollowCam::Update");

    JCamera::Update();
}