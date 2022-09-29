#include "spawnInfo.h"

#if WIN32
    #define YAML_CPP_STATIC_DEFINE
#endif
#include <tracy/Tracy.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML {
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
YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v);
}

void PSpawnInfo::Load(PTextureCache& texCache, std::string name)
{
    ZoneScopedN("PSpawnInfo::Load");
    YAML::Node root = YAML::LoadFile("res/pcl/SpawnInfo/" + name + ".yaml");
    std::string shape = root["SpawnShape"].as<std::string>();
    if (shape == "Sphere") spawnShape = P_SPAWNSHAPE_SPHERE;
    else if (shape == "CircleRandom") spawnShape = P_SPAWNSHAPE_CIRCLE_RANDOM;
    else if (shape == "CircleEven") spawnShape = P_SPAWNSHAPE_CIRCLE_EVEN;
    else if (shape == "Ball") spawnShape = P_SPAWNSHAPE_BALL;
    else if (shape == "Disc") spawnShape = P_SPAWNSHAPE_DISC;
    else spawnShape = P_SPAWNSHAPE_POINT;
    std::string mode = root["DrawMode"].as<std::string>();
    if (mode == "Stretch") drawMode = P_DRAWMODE_STRETCH;
    else if (mode == "Billboard") drawMode = P_DRAWMODE_BILLBOARD;
    else drawMode = P_DRAWMODE_3D;
    std::string plane = root["Plane"].as<std::string>();
    if (plane == "X") this->plane = P_PARTICLEPLANE_X;
    else if (plane == "Y") this->plane = P_PARTICLEPLANE_Y;
    else if (plane == "Z") this->plane = P_PARTICLEPLANE_Z;
    else this->plane = P_PARTICLEPLANE_DIR;
    rotate = root["Rotate"].as<bool>();
    randomInitAngle = root["RandomInitAngle"].as<bool>();
    followSystem = root["FollowSystem"].as<bool>();
    allNegativeAxis = root["AllNegativeAxis"].as<bool>();
    horizontalIf3d = root["HorizontalIf3d"].as<bool>();
    selfDestruct = root["SelfDestruct"].as<bool>();
    spawnRate = root["SpawnRate"].as<float>();
    maxDist = root["MaxDist"].as<float>();
    dir = root["Direction"].as<glm::vec3>();
    color = root["Color"].as<glm::vec3>();
    perpSpeed = root["PerpSpeed"].as<float>();
    speed = root["Speed"].as<float>();
    scale = root["Scale"].as<float>();
    horizontalScaleMultiplier = root["HorizontalScaleMultiplier"].as<float>();
    minAngularSpeed = root["MinAngularSpeed"].as<float>();
    maxAngularSpeed = root["MaxAngularSpeed"].as<float>();
    spawnTime = root["SpawnTime"].as<float>();
    scaleRandomness = root["ScaleRandomness"].as<float>();
    lifetime = root["Lifetime"].as<float>();
    lifetimeRandomness = root["LifetimeRandomness"].as<float>();
    speedRandomness = root["SpeedRandomness"].as<float>();
    spawnPeriod = root["SpawnPeriod"].as<float>();
    alpha = root["Alpha"].as<float>();
    speedFalloff = root["SpeedFalloff"].as<float>();
    altLength = root["AltLength"].as<unsigned int>();
    velStretchFactor = root["VelStretchFactor"].as<float>();
    logTexRepeatHorizontal = root["LogTexRepeatHorizontal"].as<unsigned int>();
    logTexRepeatVertical = root["LogTexRepeatVertical"].as<unsigned int>();
    std::string repeatX = root["RepeatX"].as<std::string>();
    PTextureRepeatMode repeatXMode = PTEXTURE_REPEAT_CLAMP;
    if (repeatX == "Repeat") repeatXMode = PTEXTURE_REPEAT_REPEAT;
    else if (repeatX == "Flip") repeatXMode = PTEXTURE_REPEAT_FLIP;
    std::string repeatY = root["RepeatY"].as<std::string>();
    PTextureRepeatMode repeatYMode = PTEXTURE_REPEAT_CLAMP;
    if (repeatY == "Repeat") repeatYMode = PTEXTURE_REPEAT_REPEAT;
    else if (repeatY == "Flip") repeatYMode = PTEXTURE_REPEAT_FLIP;
    texture = texCache.Fetch(root["Texture"].as<std::string>(), repeatXMode, repeatYMode);
}