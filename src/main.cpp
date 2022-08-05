#include "buffers.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include "vertexModes/vertexColorUV.h"
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unique_ptr<JShader> shader;
std::unique_ptr<JTexture> texture1;
std::unique_ptr<JTexture> texture2;
std::unique_ptr<JBuffers> buffers;

VertexColorUV vertices[] =
{
    VertexColorUV(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f ), glm::vec2(1.0f, 1.0f)), // Top right.
    VertexColorUV(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f ), glm::vec2(1.0f, 0.0f)), // Bottom right.
    VertexColorUV(glm::vec3(-0.5f,  -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f ), glm::vec2(0.0f, 0.0f)), // Bottom left.
    VertexColorUV(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f ), glm::vec2(0.0f, 1.0f)) // Top left.
};

GLuint indices[] =
{
    0, 1, 3,
    1, 2, 3
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void window_draw(GLFWwindow* window)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    texture1->Use();
    glActiveTexture(GL_TEXTURE1);
    texture2->Use();
    shader->Use();
    buffers->Bind();
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
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
    ShaderList shaderList;
    shaderList.push_back(std::pair("res/shd/defaultVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/defaultFrag.glsl", GL_FRAGMENT_SHADER));
    shader = std::make_unique<JShader>(shaderList);

    // Vertex buffers.
    buffers = std::make_unique<JBuffers>(vertices, sizeof(vertices), GL_STATIC_DRAW, indices, sizeof(indices), GL_STATIC_DRAW);
    VertexColorUV::SetAttributes();

    // Unbind buffers.
    Buffers_Bind(EMPTY_BUFFER);

    // Texture stuff.
    texture1 = std::make_unique<JTexture>("res/tex/colfawnGotaPfp.png");
    texture2 = std::make_unique<JTexture>("res/tex/AsylumServerIcon.png");
    shader->Use();
    shader->SetInt("texture1", 0); // Texture units 0 and 1.
    shader->SetInt("texture2", 1);

    // Run and close.
    Window_Main(window, window_callback);
    Window_Close();
    return 0;

}