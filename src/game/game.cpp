#include "game.h"
#include "../kcl.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

GGame::GGame()
{

    // Shader setup.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    ShaderList shaderList;
    shaderList.push_back(std::pair("res/shd/defaultVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/defaultFrag.glsl", GL_FRAGMENT_SHADER));
    shader = std::make_unique<JShader>(shaderList);
    shaderList.clear();
    shaderList.push_back(std::pair("res/shd/skyboxVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/skyboxFrag.glsl", GL_FRAGMENT_SHADER));
    skyboxShader = std::make_unique<JShader>(shaderList);
    shaderList.clear();
    shaderList.push_back(std::pair("res/shd/particleVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/particleFrag.glsl", GL_FRAGMENT_SHADER));
    particleShader = std::make_unique<JShader>(shaderList);
    shaderList.clear();
    shaderList.push_back(std::pair("res/shd/kclVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/kclFrag.glsl", GL_FRAGMENT_SHADER));
    kclShader = std::make_unique<JShader>(shaderList);

    // Camera setup.
    camera = std::make_unique<JFreeCam>();
    camera->cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    // Light setup.
    shader->Use();
    lightDirectional = std::make_unique<JLightDirectional>(glm::vec3(-0.0f, -1.0f, -0.0f));
    lightDirectional->diffuse = glm::vec3(0.95f, 0.95f, 0.7f);
    lightDirectional->ambient = glm::vec3(1.0f, 0.501f, 0.188f) * 0.45f;
    lightDirectional->SetVars(*shader);

    // Particle manager.
    particleMgr = std::make_unique<PManager>(5);
    //auto& system = particleMgr->AddSystemGravity("GravityTest", std::make_unique<RGravityDisk>(glm::vec3(0.0f, -2.7f, -4.2f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 5.5f));
    //system.gravity->range = 20.0f;
    //system.gravity->offset = 1.5f;

    // Uniform buffer setup.
    matrices = std::make_unique<JUniformBuffer>(sizeof(glm::mat4) * 2, GL_STATIC_DRAW);
    matrices->ConnectToShader(*shader, "Matrices");
    matrices->ConnectToShader(*particleShader, "Matrices");
    matrices->ConnectToShader(*kclShader, "Matrices");

}

std::map<int, GScenarioEntry> GGame::GetScenarioList(std::string level)
{
    std::fstream file;
    std::map<int, GScenarioEntry> ret;
    file.open("res/scn/" + level + "/Scenarios.txt", std::ios::in);
    if (file.is_open())
    {
        std::string tp;
        while(std::getline(file, tp))
        {
            std::stringstream line(tp);
            GScenarioEntry entry;
            std::string id;
            std::getline(line, id, '|');
            std::getline(line, entry.title, '|');
            std::getline(line, entry.yaml, '|');
            ret[std::stoi(id)] = entry;
        }
        file.close();
    }
    else std::cout << "ERROR: Failed to open " + level + " scenario list." << std::endl;
    return ret;
}

void GGame::LoadLevelScenario(std::string level, int scenario)
{
    auto list = GetScenarioList(level);
    if (list.find(scenario) != list.end())
    {
        this->scenario = std::make_unique<GScenario>(*shader, *skyboxShader, *kclShader);
        std::cout << "INFO: Loading level \"" << level << "\" scenario " << scenario << ": \"" + list[scenario].title << "\"" << std::endl;
        this->scenario->Load("res/scn/" + level + "/" + list[scenario].yaml);
    }
}

void GGame::Render()
{

    // Update logic.
    particleMgr->Update();
    scenario->Update();

    // Camera stuff.
    camera->Update();
    glm::mat4 projection = camera->ProjectionMatrix();
    glm::mat4 view = camera->ViewMatrix();
    matrices->Bind();
    matrices->SendData(0, glm::value_ptr(projection), sizeof(glm::mat4));
    matrices->SendData(sizeof(glm::mat4), glm::value_ptr(view), sizeof(glm::mat4));
    matrices->Unbind();

    // Skybox.
    scenario->skybox->model->shader.Use();
    scenario->skybox->model->shader.SetMatrix("projection", glm::value_ptr(projection));
    scenario->skybox->model->shader.SetMatrix("view", glm::value_ptr(glm::mat4(glm::mat3(view))));

    // Scenario.
    scenario->Render();

    // Particles.
    particleMgr->Render(*particleShader);

}