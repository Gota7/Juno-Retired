#include "skybox.h"
#include "scenario.h"

Vertex skyboxVertices[] = {
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)), // 0, 5, 8, 9, 24, 29           0
    Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)), // 1, 7, 30 - 1                 1
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)), // 2, 3, 12, 17, 32, 33         2
    Vertex(glm::vec3( 1.0f,  1.0f, -1.0f)), // 4, 16, 25                    3
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)), // 6, 11, 18, 23, 31, 34        4
    Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)), // 10, 19, 28                   5
    Vertex(glm::vec3( 1.0f, -1.0f,  1.0f)), // 13, 22, 35                   6
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)) // 14, 15, 20, 21, 26, 27        7
};

GLuint skyboxIndices[] =
{
    0, 1, 2,
    2, 3, 0,
    4, 1, 0,
    0, 5, 4,
    2, 6, 7,
    7, 3, 2,
    4, 5, 7,
    7, 6, 4,
    0, 3, 7,
    7, 5, 0,
    1, 4, 2,
    2, 4, 6
};

GSkybox::GSkybox(GScenario* scenario, ModelCubemapTextures textures)
{
    std::string combinedNames = COMBINED_CUBEMAP_NAME(std::get<0>(textures[0]), std::get<1>(textures[0]), std::get<2>(textures[0]), std::get<3>(textures[0]), std::get<4>(textures[0]), std::get<5>(textures[0]));
    if (!scenario->modelGroup.ModelLoaded(combinedNames))
    {
        std::vector<std::unique_ptr<JMesh>> meshes;
        meshes.push_back(std::make_unique<JMesh>(
            skyboxVertices,
            sizeof(skyboxVertices),
            GL_STATIC_DRAW,
            skyboxIndices,
            sizeof(skyboxIndices),
            GL_STATIC_DRAW,
            GL_TRIANGLES,
            sizeof(skyboxIndices) / sizeof(skyboxIndices[0]),
            GL_UNSIGNED_INT,
            0
        ));
        std::vector<std::unique_ptr<JMaterialTex>> materials;
        materials.push_back(std::make_unique<JMaterialTex>(combinedNames, "res/tex/null.png"));
        scenario->modelGroup.models[combinedNames] = std::make_unique<JModel> (
            meshes,
            textures,
            materials,
            scenario->skyboxShader
        );
        scenario->skyboxShader.Use();
        scenario->skyboxShader.SetInt("skybox", 0);
        Vertex::SetAttributes();
    }
    model = scenario->modelGroup.LoadOrGetModel(combinedNames, scenario->skyboxShader);
}

void GSkybox::Render()
{
    model->Render();
}