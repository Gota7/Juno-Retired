#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "buffers.h"
#include "cameras/freeCam.h"
#include "frame.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include "vertexModes/vertexNormal.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unique_ptr<JShader> lightShader;
std::unique_ptr<JShader> shader;
std::unique_ptr<JModel> model;
std::unique_ptr<JFreeCam> camera;
std::unique_ptr<JLight> light;
std::unique_ptr<JMaterial> material;

VertexNormal vertices[] = {
        VertexNormal(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f)),
        VertexNormal(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f)),
        VertexNormal(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f)),
        VertexNormal(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f)),
        VertexNormal(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f)),
        VertexNormal(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f)),
        VertexNormal(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f)),
        VertexNormal(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f)),
        VertexNormal(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f)),
        VertexNormal(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f)),
        VertexNormal(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f)),
        VertexNormal(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f)),
        VertexNormal(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 1.0f,  0.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f,  0.0f)),
        VertexNormal(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f,  0.0f)),
        VertexNormal(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f,  0.0f))
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

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f, 0.0f, 0.0f),
    glm::vec3( 2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f, 2.0f, -2.5f),
    glm::vec3( 1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void window_draw(GLFWwindow* window)
{

    // Clear background.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Camera stuff.
    camera->Update();
    const GLfloat* projection = glm::value_ptr(camera->ProjectionMatrix());
    const GLfloat* view = glm::value_ptr(camera->ViewMatrix());

    // Light cube.
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    lightPos.x += cos(glfwGetTime());
    lightPos.y += sin(glfwGetTime());
    lightPos.z += cos(glfwGetTime()) * sin(glfwGetTime());
    light->position = lightPos;
    lightShader->Use();
    lightShader->SetMatrix("projection", projection);
    lightShader->SetMatrix("view", view);
    model->matrix = glm::mat4(1.0f);
    model->matrix = glm::translate(glm::mat4(1.0f), lightPos);
    model->matrix = glm::scale(model->matrix, glm::vec3(0.2f)); // A smaller cube.
    model->Render();

    // Normal cube.
    shader->Use();
    light->SetVars(*shader);
    material->SetVars(*shader);
    shader->SetVec3("viewPos", glm::value_ptr(camera->cameraPos));
    shader->SetMatrix("projection", projection);
    shader->SetMatrix("view", view);
    model->matrix = glm::mat4(1.0f);
    model->Render();
    // for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
    // {
    //     model->matrix = glm::translate(glm::mat4(1.0f), cubePositions[i]);
    //     model->matrix = glm::rotate(model->matrix, glm::radians(20.0f + i), glm::vec3(1.0f, 0.3f, 0.5f));
    //     model->matrix = glm::rotate(model->matrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    //     model->Render();
    // }
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
    ShaderList shaderList;
    shaderList.push_back(std::pair("res/shd/defaultVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/defaultFrag.glsl", GL_FRAGMENT_SHADER));
    shader = std::make_unique<JShader>(shaderList);
    shaderList.clear();
    shaderList.push_back(std::pair("res/shd/lightVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/lightFrag.glsl", GL_FRAGMENT_SHADER));
    lightShader = std::make_unique<JShader>(shaderList);

    // Model setup.
    camera = std::make_unique<JFreeCam>();
    camera->cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    std::vector<std::string> textures;
    // textures.push_back("res/tex/colfawnGotaPfp.png");
    // textures.push_back("res/tex/AsylumServerIcon.png");
    model = std::make_unique<JModel> (
        vertices,
        sizeof(vertices),
        GL_STATIC_DRAW,
        indices,
        sizeof(indices),
        GL_STATIC_DRAW,
        textures,
        *shader,
        GL_TRIANGLES,
        sizeof(indices) / sizeof(indices[0]),
        GL_UNSIGNED_INT
    );
    VertexNormal::SetAttributes();

    // Other setups.
    light = std::make_unique<JLight>();
    material = std::make_unique<JMaterial>();
    material->ambient = glm::vec3(1.0f, 0.5f, 0.31f);
    material->diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
    material->specular = glm::vec3(0.5f, 0.5f, 0.5f);

    // Unbind buffers.
    Buffers_Bind(EMPTY_BUFFER);

    // Run and close.
    Window_Main(window, window_callback);
    Window_Close();
    return 0;

}