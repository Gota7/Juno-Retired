#include "scenario.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <tracy/Tracy.hpp>
#if WIN32
    #define YAML_CPP_STATIC_DEFINE
#endif
#include <yaml-cpp/yaml.h>

#include "actor/spawnTable.h"

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
template<>
struct convert<glm::vec3> {
  static Node encode(const glm::vec3& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    return node;
  }

  static bool decode(const Node& node, glm::vec3& rhs) {
    if(!node.IsSequence() || node.size() != 3) {
      return false;
    }
    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    return true;
  }
};
YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v) {
	out << YAML::Flow << YAML::BeginSeq;
    out << v.x;
    out << v.y;
    out << v.z;
    out << YAML::EndSeq;
	return out;
}
template<>
struct convert<GScenarioFog> {
  static Node encode(const GScenarioFog& rhs) {
    Node node;
    node.push_back(rhs.color);
    node.push_back(rhs.start);
    node.push_back(rhs.end);
    node.push_back(rhs.density);
    if (rhs.type == FOG_EXP)
    {
        node.push_back("Exp");
    }
    else if (rhs.type == FOG_EXP2)
    {
        node.push_back("Exp2");
    }
    else
    {
        node.push_back("Linear");
    }
    return node;
  }

  static bool decode(const Node& node, GScenarioFog& rhs) {
    if(!node.IsSequence() || node.size() != 5) {
      return false;
    }
    rhs.color = node[0].as<glm::vec3>();
    rhs.start = node[1].as<float>();
    rhs.end = node[2].as<float>();
    rhs.density = node[3].as<float>();
    std::string type = node[4].as<std::string>();
    if (type == "Exp")
    {
        rhs.type = FOG_EXP;
    }
    else if (type == "Exp2")
    {
        rhs.type = FOG_EXP2;
    }
    else
    {
        rhs.type = FOG_LINEAR;
    }
    return true;
  }
};
YAML::Emitter& operator << (YAML::Emitter& out, const GScenarioFog& v) {
	out << YAML::Flow << YAML::BeginSeq;
    out << v.color;
    out << v.start;
    out << v.end;
    out << v.density;
    if (v.type == FOG_EXP)
    {
        out << "Exp";
    }
    else if (v.type == FOG_EXP2)
    {
        out << "Exp2";
    }
    else
    {
        out << "Linear";
    }
    out << YAML::EndSeq;
	return out;
}
}

void GScenarioFog::Set(JShader& shader)
{
    ZoneScopedN("GScenarioFog::Set");

    shader.SetBool("fog.enabled", true);
    shader.SetVec3("fog.color", glm::value_ptr(color));
    shader.SetFloat("fog.start", start);
    shader.SetFloat("fog.end", end);
    shader.SetFloat("fog.density", density);
    shader.SetInt("fog.type", (int)type);
}

void GScenarioFog::Disable(JShader& shader)
{
    ZoneScopedN("GScenarioFog::Disable");

    shader.SetBool("fog.enabled", false);
}

void GScenario::Load(std::string yaml)
{
    ZoneScopedN("GScenario::Load");

    // Initial setup.
    YAML::Node root = YAML::LoadFile(yaml);

    // Fog.
    if (root["Fog"].IsDefined())
    {
        fog = std::make_unique<GScenarioFog>(root["Fog"].as<GScenarioFog>());
        fog->Set(defShader);
    }
    else
    {
        fog = nullptr;
        GScenarioFog::Disable(defShader);
    }

    // Skybox.
    ModelCubemapTextures cubemapTextures;
    cubemapTextures.push_back(std::tuple(root["SkyboxLeft"].as<std::string>(), root["SkyboxRight"].as<std::string>(), root["SkyboxTop"].as<std::string>(), root["SkyboxBottom"].as<std::string>(), root["SkyboxFront"].as<std::string>(), root["SkyboxBack"].as<std::string>()));
    skybox = std::make_unique<GSkybox>(this, cubemapTextures);

    // Planets.
    YAML::Node planets = root["Planets"];
    this->planets.clear();
    for (YAML::Node planet : planets)
    {
        this->planets.push_back(std::make_unique<GPlanet>(this, planet[0].as<std::string>(), planet[1].as<std::string>(), planet[2].as<glm::mat4>()));
    }

    // Colliders.
    YAML::Node colliders = root["Colliders"];
    this->miscModels.clear();
    this->colliders = KTree(std::vector<std::unique_ptr<KMesh>>());
    this->colliderInfo.clear();
    for (YAML::Node col : colliders)
    {
        GScenarioCollider c;
        c.type = col[0].as<std::string>();
        if (c.type == "Model")
        {
            c.type = "Model";
            c.path = col[1].as<std::string>();
            c.mat = col[2].as<glm::mat4>();
            c.debug = col[3].as<bool>();
            auto cMod = std::make_unique<KModel>(c.path, c.mat);
            if (c.debug) // Debug.
            {
                miscModels.push_back(cMod->ToJModel(kclShader));
            }
            this->colliders.AddMesh(std::move(cMod));
            colliderInfo.push_back(c);
        }
    }

    // Gravities.
    YAML::Node gravities = root["Gravity"];
    this->gravMgr.gravities.clear();
    for (YAML::Node grav : gravities)
    {
        std::string type = grav["Type"].as<std::string>();
        bool found = true;
        if (type == "Disk")
        {
            gravMgr.AddGravity(std::make_unique<RGravityDisk>(
                grav["Pos"].as<glm::vec3>(),
                grav["Dir"].as<glm::vec3>(),
                grav["SideDir"].as<glm::vec3>(),
                grav["Radius"].as<float>(),
                grav["ValidDegrees"].IsDefined() ? grav["ValidDegrees"].as<float>() : 360.0f,
                grav["BothSides"].IsDefined() ? grav["BothSides"].as<bool>() : true,
                grav["EdgeGravity"].IsDefined() ? grav["EdgeGravity"].as<bool>() : true
            ), grav["Priority"].IsDefined() ? grav["Priority"].as<unsigned int>() : 0);
        }
        else if (type == "Parallel")
        {
            std::string rangeType = grav["RangeType"].as<std::string>();
            if (rangeType == "Sphere")
            {
                gravMgr.AddGravity(std::make_unique<RGravityParallel>(
                    grav["Pos"].as<glm::vec3>(),
                    grav["Dir"].as<glm::vec3>(),
                    grav["BaseDist"].as<float>()
                ));
            }
            else if (rangeType == "Box")
            {
                RGravityParallelDistanceTypeEnum type = PARALLEL_DISTANCE_DEFAULT;
                std::string distType;
                if (grav["DistType"].IsDefined())
                {
                    distType = grav["DistType"].as<std::string>();
                    if (distType == "X")
                    {
                        type = PARALLEL_DISTANCE_X;
                    }
                    else if (distType == "Y")
                    {
                        type = PARALLEL_DISTANCE_Y;
                    }
                    else if (distType == "Z")
                    {
                        type = PARALLEL_DISTANCE_Z;
                    }
                }
                gravMgr.AddGravity(std::make_unique<RGravityParallel>(
                    grav["Pos"].as<glm::vec3>(),
                    grav["Dir"].as<glm::vec3>(),
                    type,
                    grav["Mtx"].as<glm::mat4>()
                ));
            }
            else if (rangeType == "Cylinder")
            {
                gravMgr.AddGravity(std::make_unique<RGravityParallel>(
                    grav["Pos"].as<glm::vec3>(),
                    grav["Dir"].as<glm::vec3>(),
                    grav["CylinderRadius"].as<float>(),
                    grav["CylinderHeight"].as<float>()
                ));
            }
            else
            {
                found = false;
            }
        }
        else
        {
            found = false;
        }
        if (found)
        {
            auto& last = gravMgr.gravities[gravMgr.gravities.size() - 1];
            if (grav["Active"].IsDefined()) last->active = grav["Active"].as<bool>();
            if (grav["Inverted"].IsDefined()) last->inverted = grav["Inverted"].as<bool>();
            if (grav["Offset"].IsDefined()) last->offset = grav["Offset"].as<float>();
            if (grav["Range"].IsDefined()) last->range = grav["Range"].as<float>();
            if (grav["Strength"].IsDefined()) last->strength = grav["Strength"].as<float>();
            if (grav["PullType"].IsDefined())
            {
                std::string type = grav["PullType"].as<std::string>();
                if (type == "Normal")
                {
                    last->type = GRAVITY_NORMAL;
                }
                else if (type == "Shadow")
                {
                    last->type = GRAVITY_SHADOW;
                }
                else if (type == "NormalShadow")
                {
                    last->type = GRAVITY_NORMAL_SHADOW;
                }
            }
        }
    }

    // Load actor infos.
    YAML::Node actors = root["Actors"];
    this->actors.clear();
    actorSpawnInfo.clear();
    for (YAML::Node actor : actors)
    {
        GSpawnInfo spwn(*this);
        spwn.actorID = actor[0].as<std::string>();
        spwn.vecs = actor[1].as<std::vector<glm::vec3>>();
        spwn.floats = actor[2].as<std::vector<float>>();
        spwn.strings = actor[3].as<std::vector<std::string>>();
        actorSpawnInfo.push_back(spwn);
        auto spawned = GSpawnTable::Spawn(spwn);
        if (spawned)
        {
            if (spawned->LoadResources()) this->actors.push_back(std::move(spawned));
        }
    }

}

void GScenario::Save(std::string yaml)
{
    ZoneScopedN("GScenario::Save");

    // Initial setup.
    std::fstream file;
    file.open(yaml, std::ios::out);
    file.clear();
    YAML::Emitter root(file);

    // Save fog.
    root << YAML::BeginMap;
    if (fog) root << YAML::Key << "Fog" << YAML::Value << *fog;

    // Save skybox.
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
    root << YAML::EndSeq;

    // Save colliders.
    root << YAML::Key << "Colliders";
    root << YAML::Value << YAML::BeginSeq;
    for (auto& col : colliderInfo)
    {
        root << YAML::Flow << YAML::BeginSeq;
        root << col.type << col.path << col.mat << col.debug;
        root << YAML::EndSeq;
    }
    root << YAML::EndSeq;

    // Save gravity.
    root << YAML::Key << "Gravity";
    root << YAML::Value << YAML::BeginSeq;
    for (unsigned int i = 0; i < gravMgr.gravities.size(); i++)
    {
        root << YAML::BeginMap;
        RGravity* grav = gravMgr.gravities[i].get();
        auto disk = dynamic_cast<RGravityDisk*>(grav);
        if (disk)
        {
            root << YAML::Key << "Type" << YAML::Value << "Disk";
            root << YAML::Key << "Pos" << YAML::Value << disk->pos;
            root << YAML::Key << "Dir" << YAML::Value << disk->direction;
            root << YAML::Key << "SideDir" << YAML::Value << disk->sideDirection;
            root << YAML::Key << "Radius" << YAML::Value << disk->radius;
            if (disk->validDegrees != 360.0f) root << YAML::Key << "ValidDegrees" << YAML::Value << disk->validDegrees;
            if (disk->bothSides != true) root << YAML::Key << "BothSides" << YAML::Value << disk->bothSides;
            if (disk->edgeGravity != true) root << YAML::Key << "EdgeGravity" << YAML::Value << disk->edgeGravity;
            if (disk->priority != 50) root << YAML::Key << "Priority" << YAML::Value << disk->priority;
        }
        if (grav->active != true) root << YAML::Key << "Active" << YAML::Value << disk->active;
        if (grav->inverted != false) root << YAML::Key << "Inverted" << YAML::Value << disk->inverted;
        if (grav->offset != 0.0f) root << YAML::Key << "Offset" << YAML::Value << disk->offset;
        if (grav->range != -1.0f) root << YAML::Key << "Range" << YAML::Value << disk->range;
        if (grav->strength != 1.0f) root << YAML::Key << "Strength" << YAML::Value << disk->strength;
        if (grav->type != GRAVITY_NORMAL_SHADOW)
        {
            if (grav->type == GRAVITY_NORMAL) root << YAML::Key << "PullType" << YAML::Value << "Normal";
            else if (grav->type == GRAVITY_SHADOW) root << YAML::Key << "PullType" << YAML::Value << "Shadow";
            else if (grav->type == GRAVITY_NORMAL_SHADOW) root << YAML::Key << "PullType" << YAML::Value << "NormalShadow";
        }
        root << YAML::EndMap;
    }
    root << YAML::EndSeq;

    // Save actors.
    root << YAML::Key << "Actors";
    root << YAML::Value << YAML::BeginSeq;
    for (auto& actor : actorSpawnInfo)
    {
        root << YAML::Flow << YAML::BeginSeq;
        root << actor.actorID << actor.vecs << actor.floats << actor.strings;
        root << YAML::EndSeq;
    }
    root << YAML::EndSeq << YAML::EndMap;

    // Save YAML.
    file.close();

}

void GScenario::Render()
{
    ZoneScopedN("GScenario::Render");

    // Draw skybox.
    glDepthFunc(GL_LEQUAL);
    skybox->Render();
    glDepthFunc(GL_LESS);

    // Draw planets.
    for (auto& planet : planets)
    {
        planet->Render(0.0f);
    }

    // Misc models.
    for (auto& model : miscModels)
    {
        model->Render();
    }

    // Actors.
    for (auto& actor : actors)
    {
        if (!actor->Render())
        {
            actor.reset();
        }
    }
    actors.erase(std::remove(actors.begin(), actors.end(), nullptr), actors.end());

}

void GScenario::Update()
{
    ZoneScopedN("GScenario::Update");
    if (paused) return;

    // Actors.
    for (auto& actor : actors)
    {
        if (!actor->Update())
        {
            actor.reset();
        }
    }
    actors.erase(std::remove(actors.begin(), actors.end(), nullptr), actors.end());

}