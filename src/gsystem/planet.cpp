#include "planet.h"

GPlanet::GPlanet(GScenario* scenario, std::string model, std::string modelLow, glm::mat4 mat)
{
    this->model = scenario->modelGroup.LoadOrGetModel(model, scenario->defShader, mat);
    this->modelLow = scenario->modelGroup.LoadOrGetModel(model, scenario->defShader, mat);
    this->mat = mat;
}

void GPlanet::Render(float distToPlayer)
{
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