#include "planet.h"
#include "scenario.h"

#include <tracy/Tracy.hpp>

GPlanet::GPlanet(GScenario* scenario, std::string model, std::string modelLow, glm::mat4 mat) : modelPath(model), modelLowPath(modelPath), mat(mat)
{
    ZoneScopedN("GPlanet::GPlanet");

    this->model = scenario->modelGroup.LoadOrGetModel(model, scenario->defShader, mat);
    this->modelLow = scenario->modelGroup.LoadOrGetModel(model, scenario->defShader, mat);
}

void GPlanet::Render(float distToPlayer)
{
    ZoneScopedN("GPlanet::Render");

    if (distToPlayer <= MAX_DIST_TO_PLAYER)
    {
        if (model.use_count() > 2) model->matrix = mat; // Set mat only if needed.
        model->Render();
    }
    else
    {
        if (modelLow.use_count() > 2) model->matrix = mat;
        modelLow->Render();
    }
}