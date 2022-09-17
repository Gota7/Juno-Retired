#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "audio.h"
#include "game.h"
#include "jsystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define VSYNC 1

std::unique_ptr<JFramebuffer> framebuffer;
std::unique_ptr<JShader> framebufferShader;
std::unique_ptr<JMesh> quadMesh;
std::unique_ptr<GGame> game;

VertexUV quadVertices[] = {
    VertexUV(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
    VertexUV(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    VertexUV(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    VertexUV(glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f))
};

int quadIndices[] = {
    0, 1, 2,
    0, 2, 3
};

void window_draw(GLFWwindow* window);

void window_callback(GLFWwindow* window)
{
    window_draw(window);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    static_cast<JFreeCam*>(game->camera.get())->GLFWKeys(window);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    static_cast<JFreeCam*>(game->camera.get())->GLFWMouse(window, xposIn, yposIn);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    static_cast<JFreeCam*>(game->camera.get())->Forward(yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    framebuffer = std::make_unique<JFramebuffer>(width, height);
    JFrame::aspect = (float)width / height;
}

// Main function.
int main()
{

    // Window init.
    GLFWwindow* window = Window_Init("Gota's Galactic Journey");
    if (!window) return -1;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
#ifndef VSYNC
    glfwSwapInterval(0);
#endif
    ASystem_Init();

    // Setup framebuffer shader.
    ShaderList shaderList;
    shaderList.clear();
    shaderList.push_back(std::pair("res/shd/framebufferVert.glsl", GL_VERTEX_SHADER));
    shaderList.push_back(std::pair("res/shd/framebufferFrag.glsl", GL_FRAGMENT_SHADER));
    framebufferShader = std::make_unique<JShader>(shaderList);

    // Quad model setup for framebuffer.
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

    // Game.
    game = std::make_unique<GGame>(window);
    game->LoadLevelScenario("TestMap", 0);
    //game->particleMgr->AddSystemGravity("GravityTest", std::make_unique<RGravityParallel>(*static_cast<RGravityParallel*>(game->scenario->gravMgr.gravities[0].get())));
    //game->scenario->planets[0]->mat = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -3.0f, -5.0f)), glm::vec3(1.0f, 1.0f, 1.0f) * 0.0025f);
    //game->scenario->Save("res/scn/TestMap/dummy.yaml");

    // Unbind buffers.
    Buffers_Bind(EMPTY_BUFFER);

    // Run and close.
    Window_Main(window, window_callback);
    ASystem_Close();
    Window_Close();
    return 0;

}

void window_draw(GLFWwindow* window)
{

#ifndef __APPLE__
    // Use framebuffer.
    framebuffer->Bind();
    glEnable(GL_DEPTH_TEST);
#endif

    // Clear background.
    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw game.
    game->Render();

#ifndef __APPLE__
    // Draw framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, EMPTY_FRAMEBUFFER);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    framebufferShader->Use();
    glActiveTexture(GL_TEXTURE0);
    framebuffer->texture.Use();
    quadMesh->Render();
#endif

    // Finally update frame.
    JFrame::Update();

}
