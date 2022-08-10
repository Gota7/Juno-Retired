#include "scenario.h"
#include "yaml.h"

void GScenario::Load(std::string yaml)
{

    // Initial setup.
    Yaml::Node root;
    Yaml::Parse(root, yaml.c_str());

    // Skybox.
    ModelCubemapTextures cubemapTextures;
    cubemapTextures.push_back(std::tuple(root["SkyboxLeft"].As<std::string>(), root["SkyboxRight"].As<std::string>(), root["SkyboxTop"].As<std::string>(), root["SkyboxBottom"].As<std::string>(), root["SkyboxFront"].As<std::string>(), root["SkyboxBack"].As<std::string>()));
    skybox = std::make_unique<GSkybox>(this, cubemapTextures);

}

void GScenario::Render()
{

    // Draw skybox.
    glDepthFunc(GL_LEQUAL);
    skybox->Render();
    glDepthFunc(GL_LESS);

}