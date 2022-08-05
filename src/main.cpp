#include "buffers.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include "vec2.h"
#include "vec3.h"
#include "vertexModes/vertexColorUV.h"
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unique_ptr<JShader> shader;
std::unique_ptr<JTexture> texture;
std::unique_ptr<JBuffers> buffers;

VertexColorUV vertices[] =
{
    VertexColorUV(Vec3(0.5f, 0.5f, 0.0f), Vec3(1.0f, 0.0f, 0.0f ), Vec2(1.0f, 1.0f)), // Top right.
    VertexColorUV(Vec3(0.5f, -0.5f, 0.0f), Vec3(0.0f, 1.0f, 0.0f ), Vec2(1.0f, 0.0f)), // Bottom right.
    VertexColorUV(Vec3(-0.5f,  -0.5f, 0.0f), Vec3(0.0f, 0.0f, 1.0f ), Vec2(0.0f, 0.0f)), // Bottom left.
    VertexColorUV(Vec3(-0.5f,  0.5f, 0.0f), Vec3(1.0f, 1.0f, 0.0f ), Vec2(0.0f, 1.0f)) // Top left.
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
    texture->Use();
    shader->Use();
    buffers->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
    Buffers_Bind(VertexBuffer(0, 0, 0));

    // Texture stuff.
    texture = std::make_unique<JTexture>("res/tex/colfawnGotaPfp.png");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Run and close.
    Window_Main(window, window_callback);
    Window_Close();
    return 0;

}