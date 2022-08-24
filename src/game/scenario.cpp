#include "scenario.h"
#include <fstream>
#include <memory>
#include <sstream>
#if WIN32
    #define YAML_CPP_STATIC_DEFINE
#endif
#include <yaml-cpp/yaml.h>

namespace YAML {
template<>
struct convert<glm::mat4> {
  static Node encode(const glm::mat4& rhs) {
    Node node;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            node.push_back(rhs[i][j]);
        }
    }
    return node;
  }

  static bool decode(const Node& node, glm::mat4& rhs) {
    if(!node.IsSequence() || node.size() != 16) {
      return false;
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            rhs[i][j] = node[i * 4 + j].as<float>();
        }
    }
    return true;
  }
};
YAML::Emitter& operator << (YAML::Emitter& out, const glm::mat4& v) {
	out << YAML::Flow << YAML::BeginSeq;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out << v[i][j];
        }
    }
    out << YAML::EndSeq;
	return out;
}
}

void GScenario::Load(std::string yaml)
{

    // Initial setup.
    YAML::Node root = YAML::LoadFile(yaml);

    // Skybox.
    ModelCubemapTextures cubemapTextures;
    cubemapTextures.push_back(std::tuple(root["SkyboxLeft"].as<std::string>(), root["SkyboxRight"].as<std::string>(), root["SkyboxTop"].as<std::string>(), root["SkyboxBottom"].as<std::string>(), root["SkyboxFront"].as<std::string>(), root["SkyboxBack"].as<std::string>()));
    skybox = std::make_unique<GSkybox>(this, cubemapTextures);

    // Planets.
    YAML::Node planets = root["Planets"];
    for (YAML::Node planet : planets)
    {
        this->planets.push_back(std::make_unique<GPlanet>(this, planet[0].as<std::string>(), planet[1].as<std::string>(), planet[2].as<glm::mat4>()));
    }

}

void GScenario::Save(std::string yaml)
{

    // Initial setup.
    std::fstream file;
    file.open(yaml, std::ios::out);
    file.clear();
    YAML::Emitter root(file);

    // Save skybox.
    root << YAML::BeginMap;
    std::stringstream skyboxLine(static_cast<JMaterialTex*>(skybox->model->materials[0].get())->diffuseName);
    std::string skyboxOut;
    root << YAML::Key << "SkyboxLeft";
    std::getline(skyboxLine, skyboxOut, ';');
    root << YAML::Value << skyboxOut;
    root << YAML::Key << "SkyboxRight";
    std::getline(skyboxLine, skyboxOut, ';');
    root << YAML::Value << skyboxOut;
    root << YAML::Key << "SkyboxTop";
    std::getline(skyboxLine, skyboxOut, ';');
    root << YAML::Value << skyboxOut;
    root << YAML::Key << "SkyboxBottom";
    std::getline(skyboxLine, skyboxOut, ';');
    root << YAML::Value << skyboxOut;
    root << YAML::Key << "SkyboxFront";
    std::getline(skyboxLine, skyboxOut, ';');
    root << YAML::Value << skyboxOut;
    root << YAML::Key << "SkyboxBack";
    std::getline(skyboxLine, skyboxOut, ';');
    root << YAML::Value << skyboxOut;

    // Save planets.
    root << YAML::Key << "Planets";
    root << YAML::Value << YAML::BeginSeq;
    for (unsigned int i = 0; i < planets.size(); i++)
    {
        root << YAML::Flow << YAML::BeginSeq;
        root << planets[i]->modelPath << planets[i]->modelLowPath << planets[i]->mat;
        root << YAML::EndSeq;
    }
    root << YAML::EndSeq << YAML::EndMap;

    // Save YAML.
    file.close();

}

void GScenario::Render()
{

    // Draw skybox.
    glDepthFunc(GL_LEQUAL);
    skybox->Render();
    glDepthFunc(GL_LESS);

    // Draw planets.
    for (auto& planet : planets)
    {
        planet->Render(0.0f);
    }

}