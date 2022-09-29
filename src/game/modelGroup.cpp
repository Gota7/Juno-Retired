#include "modelGroup.h"

#include <tracy/Tracy.hpp>

bool GModelGroup::ModelLoaded(const std::string& path)
{
    ZoneScopedN("GModelGroup::ModelLoaded");

    return models.find(path) != models.end();
}

std::shared_ptr<JModel> GModelGroup::LoadOrGetModel(const std::string& path, JShader& shader, glm::mat4 mat)
{
    ZoneScopedN("GModelGroup::LoadOrGetModel");

    if (!ModelLoaded(path))
    {
        models[path] = std::make_shared<JModel>(path, shader, mat);
    }
    return models[path];
}