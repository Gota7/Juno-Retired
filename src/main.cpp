#include "buffers.h"
#include "shader.h"
#include "window.h"
#include "vec3.h"

ShaderProgram shaderProgram;
VertexBuffer buffers;

Vec3 vertices[] =
{
    { -0.5f, -0.5f, 0.0f }, // Left.
    { 1.0f, 0.0f, 0.0f }, // Red.
    { 0.5f, -0.5f, 0.0f }, // Right.
    { 0.0f, 1.0f, 0.0f }, // Green.
    { 0.0f,  0.5f, 0.0f },  // Top.
    { 0.0f, 0.0f, 1.0f } // Blue.
};

GLuint indices[] =
{
    0, 1, 2
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void window_draw(GLFWwindow* window)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    Buffers_Bind(buffers);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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
    shaderProgram = Shader_CreateProgram(shaderList);

    // Vertex buffers.
    buffers = Buffers_Generate(vertices, sizeof(vertices), GL_STATIC_DRAW, indices, sizeof(indices), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3) * 2, (void*)0); // Also connects VBO to the VAO, but not EBO to VAO. Dumb.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3) * 2, (void*)sizeof(Vec3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Unbind buffers.
    Buffers_Bind(VertexBuffer(0, 0, 0));

    // Run and close.
    Window_Main(window, window_callback);
    Buffers_Delete(buffers);
    Window_Close();
    return 0;

}