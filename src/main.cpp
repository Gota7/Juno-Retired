// Here lies some very bad sample code for the JSystem.
// TODO: Multisampling?

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "buffers.h"
#include "cameras/freeCam.h"
#include "frame.h"
#include "framebuffer.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include "vertexModes/vertex.h"
#include "vertexModes/vertexNormalUV.h"
#include "vertexModes/vertexUV.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unique_ptr<JShader> lightShader;
std::unique_ptr<JShader> shader;
std::unique_ptr<JShader> shaderInstanced;
std::unique_ptr<JShader> framebufferShader;
std::unique_ptr<JShader> skyboxShader;
std::unique_ptr<JModel> cubeModel;
std::unique_ptr<JModel> levelModel;
std::unique_ptr<JModel> backpackModel;
std::unique_ptr<JModel> skyboxModel;
std::unique_ptr<JMesh> quadMesh;
std::unique_ptr<JFreeCam> camera;
std::unique_ptr<JLightPoint> lightPoint;
std::unique_ptr<JLightDirectional> lightDirectional;
std::unique_ptr<JLightSpot> lightSpot;
std::unique_ptr<JFramebuffer> framebuffer;
std::unique_ptr<JUniformBuffer> matrices;
std::unique_ptr<JInstanceBuffer> cubePositions;

VertexNormalUV vertices[] = {
    VertexNormalUV(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)), // bottom-left
    VertexNormalUV(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)), // top-right
    VertexNormalUV(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)), // bottom-right
    VertexNormalUV(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)), // top-right
    VertexNormalUV(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)), // bottom-left
    VertexNormalUV(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)), // top-left
    VertexNormalUV(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)), // bottom-left
    VertexNormalUV(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)), // bottom-right
    VertexNormalUV(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)), // top-right
    VertexNormalUV(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)), // top-right
    VertexNormalUV(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)), // top-left
    VertexNormalUV(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)), // bottom-left
    VertexNormalUV(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)), // top-right
    VertexNormalUV(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)), // top-left
    VertexNormalUV(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)), // bottom-left
    VertexNormalUV(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)), // bottom-left
    VertexNormalUV(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)), // bottom-right
    VertexNormalUV(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)), // top-right
    VertexNormalUV(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)), // top-left
    VertexNormalUV(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)), // bottom-right
    VertexNormalUV(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)), // top-right
    VertexNormalUV(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)), // bottom-right
    VertexNormalUV(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)), // top-left
    VertexNormalUV(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)), // bottom-left
    VertexNormalUV(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)), // top-right
    VertexNormalUV(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)), // top-left
    VertexNormalUV(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)), // bottom-left
    VertexNormalUV(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)), // bottom-left
    VertexNormalUV(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)), // bottom-right
    VertexNormalUV(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)), // top-right
    VertexNormalUV(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)), // top-left
    VertexNormalUV(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)), // bottom-right
    VertexNormalUV(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)), // top-right
    VertexNormalUV(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)), // bottom-right
    VertexNormalUV(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)), // top-left
    VertexNormalUV(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)) // bottom-left
};

GLuint indices[] =
{
    0, 1, 2,
    3, 4, 5,
    6, 7, 8,
    9, 10, 11,
    12, 13, 14,
    15, 16, 17,
    18, 19, 20,
    21, 22, 23,
    24, 25, 26,
    27, 28, 29,
    30, 31, 32,
    33, 34, 35
};

VertexUV quadVertices[] = {
    VertexUV(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
    VertexUV(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    VertexUV(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    VertexUV(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
    VertexUV(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    VertexUV(glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f))
};

int quadIndices[] = {
    0, 1, 2,
    3, 4, 5
};

Vertex skyboxVertices[] = {
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
    Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f, -1.0f)),
    Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
    Vertex(glm::vec3( 1.0f, -1.0f,  1.0f))
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    framebuffer = std::make_unique<JFramebuffer>(width, height);
}

void window_draw(GLFWwindow* window)
{

    // Use framebuffer.
    framebuffer->Bind();
    glEnable(GL_DEPTH_TEST);

    // Clear background.
    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Camera stuff.
    camera->Update();
    glm::mat4 projection = camera->ProjectionMatrix();
    glm::mat4 view = camera->ViewMatrix();
    matrices->Bind();
    matrices->SendData(0, glm::value_ptr(projection), sizeof(glm::mat4));
    matrices->SendData(sizeof(glm::mat4), glm::value_ptr(view), sizeof(glm::mat4));
    matrices->Unbind();

    // Light cube.
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    lightPos.x += cos(glfwGetTime());
    lightPos.y += sin(glfwGetTime());
    lightPos.z += cos(glfwGetTime()) * sin(glfwGetTime());
    lightPoint->position = lightPos;
    lightShader->Use();
    cubeModel->matrix = glm::mat4(1.0f);
    cubeModel->matrix = glm::translate(glm::mat4(1.0f), lightPos);
    cubeModel->matrix = glm::scale(cubeModel->matrix, glm::vec3(0.2f)); // A smaller cube.
    cubeModel->Render(lightShader.get());
    lightSpot->position = camera->cameraPos;
    lightSpot->direction = camera->cameraFront;

    // Light stuff.
    shader->Use();
    lightPoint->SetVars(*shader);
    lightDirectional->SetVars(*shader);
    //lightSpot->SetVars(*shader);
    shader->SetVec3("viewPos", glm::value_ptr(camera->cameraPos));
    shaderInstanced->Use();
    lightPoint->SetVars(*shaderInstanced);
    lightDirectional->SetVars(*shaderInstanced);
    shaderInstanced->SetVec3("viewPos", glm::value_ptr(camera->cameraPos));

    // Draw level and backpack.
    levelModel->Render();
    //backpackModel->Render();

    // Draw skybox.
    glDepthFunc(GL_LEQUAL);
    skyboxShader->Use();
    skyboxShader->SetMatrix("projection", glm::value_ptr(projection));
    skyboxShader->SetMatrix("view", glm::value_ptr(glm::mat4(glm::mat3(view))));
    skyboxModel->Render();
    glDepthFunc(GL_LESS);

    // Draw cubes.
    cubeModel->matrix = glm::mat4(1.0f);
    cubeModel->Render(shaderInstanced.get(), 100 * 100);

    // Draw framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, EMPTY_FRAMEBUFFER);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    framebufferShader->Use();
    glActiveTexture(GL_TEXTURE0);
    framebuffer->texture.Use();
    quadMesh->Render();

    // Finally update frame.
    JFrame::Update();

}

void window_callback(GLFWwindow* window)
{
    window_draw(window);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    camera->GLFWKeys(window);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    camera->GLFWMouse(window, xposIn, yposIn);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->Forward(yoffset);
}

int main()
{

    // Window init.
    GLFWwindow* window = Window_Init();
    if (!window) return -1;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Setup shaders.
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
    shaderList.push_back(std::pair("res/shd/defaultInstancedVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/defaultFrag.glsl", GL_FRAGMENT_SHADER));
    shaderInstanced = std::make_unique<JShader>(shaderList);
    shaderList.clear();
    shaderList.push_back(std::pair("res/shd/lightVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/lightFrag.glsl", GL_FRAGMENT_SHADER));
    lightShader = std::make_unique<JShader>(shaderList);

    // Setup cube positions.
    shaderInstanced->Use();
    glm::vec3 base = glm::vec3(-1.0f, 0.0f, -100.0f);
    std::vector<glm::vec3> positions;
    for (int x = 0; x < 100; x++)
    {
        for (int z = 0; z < 100; z++)
        {
            positions.push_back(base + glm::vec3(x * 2, (rand() % 1000) / 100.0f - 5.0f, z * 2));
        }
    }
    cubePositions = std::make_unique<JInstanceBuffer>(&positions[0], sizeof(glm::vec3) * positions.size(), GL_STATIC_DRAW);

    // Cube model setup.
    camera = std::make_unique<JFreeCam>();
    camera->cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    std::vector<std::string> textures;
    textures.push_back("res/tex/AsylumServerIcon.png");
    textures.push_back("res/tex/AsylumServerIconSpec.png");
    std::vector<std::unique_ptr<JMaterialTex>> materials;
    materials.push_back(std::make_unique<JMaterialTex>("res/tex/AsylumServerIcon.png", "res/tex/AsylumServerIconSpec.png"));
    std::vector<std::unique_ptr<JMesh>> meshes;
    meshes.push_back(std::make_unique<JMesh>(
        vertices,
        sizeof(vertices),
        GL_STATIC_DRAW,
        indices,
        sizeof(indices),
        GL_STATIC_DRAW,
        GL_TRIANGLES,
        sizeof(indices) / sizeof(indices[0]),
        GL_UNSIGNED_INT,
        0
    ));
    cubeModel = std::make_unique<JModel> (
        meshes,
        textures,
        materials,
        *shader
    );
    VertexNormalUV::SetAttributes();
    glEnableVertexAttribArray(3);
    cubePositions->Bind();
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(3, 1);

    // Level model setups.
    levelModel = std::make_unique<JModel>("res/mdl/GardenPlanet/diskgardenplanet.dae", *shader);
    //levelModel->matrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f) * 5.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    levelModel->matrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f) * 0.0025f), glm::vec3(-5000.0f, 0.0f, 0.0f));

    // Backpack model setup.
    //backpackModel = std::make_unique<JModel>("res/mdl/Backpack/backpack.obj", *shader);
    //backpackModel->matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f));

    // Quad model setup.
    shaderList.clear();
    shaderList.push_back(std::pair("res/shd/framebufferVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/framebufferFrag.glsl", GL_FRAGMENT_SHADER));
    framebufferShader = std::make_unique<JShader>(shaderList);
    quadMesh = std::make_unique<JMesh>(
        quadVertices,
        sizeof(quadVertices),
        GL_STATIC_DRAW,
        quadIndices,
        sizeof(quadIndices),
        GL_STATIC_DRAW,
        GL_TRIANGLES,
        sizeof(quadIndices) / sizeof(quadIndices[0]),
        GL_UNSIGNED_INT,
        0
    );
    VertexUV::SetAttributes();
    framebuffer = std::make_unique<JFramebuffer>(SCR_WIDTH, SCR_HEIGHT);

    // Skybox setup.
    shaderList.clear();
    shaderList.push_back(std::pair("res/shd/skyboxVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/skyboxFrag.glsl", GL_FRAGMENT_SHADER));
    skyboxShader = std::make_unique<JShader>(shaderList);
    ModelCubemapTextures cubemapTextures;
    cubemapTextures.push_back(std::tuple("res/tex/Skybox/left.png", "res/tex/Skybox/right.png", "res/tex/Skybox/top.png", "res/tex/Skybox/bottom.png", "res/tex/Skybox/front.png", "res/tex/Skybox/back.png"));
    materials.clear();
    materials.push_back(std::make_unique<JMaterialTex>(COMBINED_CUBEMAP_NAME(std::string("res/tex/Skybox/left.png"), "res/tex/Skybox/right.png", "res/tex/Skybox/top.png", "res/tex/Skybox/bottom.png", "res/tex/Skybox/front.png", "res/tex/Skybox/back.png"), "res/tex/null.png"));
    meshes.clear();
    meshes.push_back(std::make_unique<JMesh>(
        skyboxVertices,
        sizeof(skyboxVertices),
        GL_STATIC_DRAW,
        indices,
        sizeof(indices),
        GL_STATIC_DRAW,
        GL_TRIANGLES,
        sizeof(indices) / sizeof(indices[0]),
        GL_UNSIGNED_INT,
        0
    ));
    skyboxModel = std::make_unique<JModel> (
        meshes,
        cubemapTextures,
        materials,
        *skyboxShader
    );
    skyboxShader->Use();
    skyboxShader->SetInt("skybox", 0);
    Vertex::SetAttributes();

    // Other setups.
    lightPoint = std::make_unique<JLightPoint>();
    lightDirectional = std::make_unique<JLightDirectional>(glm::vec3(-0.0f, -1.0f, -0.0f));
    lightDirectional->diffuse = glm::vec3(0.95f, 0.95f, 0.7f);
    lightDirectional->ambient = glm::vec3(1.0f, 0.501f, 0.188f) * 0.45f;
    lightSpot = std::make_unique<JLightSpot>(camera->cameraFront);

    // Uniform buffer setup.
    matrices = std::make_unique<JUniformBuffer>(sizeof(glm::mat4) * 2, GL_STATIC_DRAW);
    matrices->ConnectToShader(*shader, "Matrices");
    matrices->ConnectToShader(*shaderInstanced, "Matrices");
    matrices->ConnectToShader(*lightShader, "Matrices");

    // Unbind buffers.
    Buffers_Bind(EMPTY_BUFFER);

    // Run and close.
    Window_Main(window, window_callback);
    Window_Close();
    return 0;

}