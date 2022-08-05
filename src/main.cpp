#include "buffers.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include "vertexModes/vertexUV.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unique_ptr<JShader> shader;
std::unique_ptr<JModel> model;

VertexUV vertices[] =
{
        VertexUV(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f)),
        VertexUV(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 0.0f)),
        VertexUV(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        VertexUV(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        VertexUV(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        VertexUV(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f)),

        VertexUV(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        VertexUV(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        VertexUV(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f)),
        VertexUV(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f)),
        VertexUV(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 1.0f)),
        VertexUV(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),

        VertexUV(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        VertexUV(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        VertexUV(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        VertexUV(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        VertexUV(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        VertexUV(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),

        VertexUV(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        VertexUV(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        VertexUV(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        VertexUV(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        VertexUV(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        VertexUV(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0)),

        VertexUV(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        VertexUV(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        VertexUV(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        VertexUV(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        VertexUV(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        VertexUV(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),

        VertexUV(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        VertexUV(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        VertexUV(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        VertexUV(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        VertexUV(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        VertexUV(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f))
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void window_draw(GLFWwindow* window)
{

    // Clear background.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Drawing.
    shader->SetMatrix("view", glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))));
    //shader->SetMatrix("view", glm::value_ptr(glm::mat4(1.0f)));
    model->Render();
    model->matrix = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model->matrix = glm::rotate(model->matrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

}

void window_callback(GLFWwindow* window)
{
    window_draw(window);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{

    // Window init.
    GLFWwindow* window = Window_Init(framebuffer_size_callback);
    if (!window) return -1;

    // Setup shaders.
    glEnable(GL_DEPTH_TEST);
    ShaderList shaderList;
    shaderList.push_back(std::pair("res/shd/defaultVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/defaultFrag.glsl", GL_FRAGMENT_SHADER));
    shader = std::make_unique<JShader>(shaderList);
    shader->Use();
    //shader->SetMatrix("projection", glm::value_ptr(glm::mat4(1.0f)));
    shader->SetMatrix("projection", glm::value_ptr(glm::perspective(
        glm::radians(45.0f),
        (float)SCR_WIDTH / SCR_HEIGHT,
        0.1f,
        100.0f
    )));

    // Model setup.
    std::vector<std::string> textures;
    textures.push_back("res/tex/colfawnGotaPfp.png");
    textures.push_back("res/tex/AsylumServerIcon.png");
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
    VertexUV::SetAttributes();

    // Unbind buffers.
    Buffers_Bind(EMPTY_BUFFER);

    // Run and close.
    Window_Main(window, window_callback);
    Window_Close();
    return 0;

}