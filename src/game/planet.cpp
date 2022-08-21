#include "planet.h"
#include "scenario.h"

GPlanet::GPlanet(GScenario* scenario, std::string model, std::string modelLow, glm::mat4 mat) : modelPath(model), modelLowPath(modelPath), mat(mat)
{
    this->model = scenario->modelGroup.LoadOrGetModel(model, scenario->defShader, mat);
    this->modelLow = scenario->modelGroup.LoadOrGetModel(model, scenario->defShader, mat);
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