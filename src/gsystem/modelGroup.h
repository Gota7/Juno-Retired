#pragma once

#include "../jsystem.h"
#include <map>
#include <memory>

// Model group.
struct GModelGroup
{
    std::map<std::string, std::shared_ptr<JModel>> models;

    // If a model exists.
    bool ModelLoaded(const std::string& path);

    // Load a model or fetch it.
    std::shared_ptr<JModel> LoadOrGetModel(const std::string& path, JShader& shader, glm::mat4 mat = glm::mat4(1.0f));

};