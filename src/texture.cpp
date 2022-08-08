#include "texture.h"
#include "stb_image.h"
#include <iostream>
#include <GLFW/glfw3.h>

Texture Texture_Create(std::string path, int& width, int& height, int& numChannels)
{
    Texture ret;
    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return ret;
    }
    else
    {
        std::cout << "Failed to load texture " << path << std::endl;
        stbi_image_free(data);
        return 0;
    }
}

Texture Texture_Generate(int width, int height)
{
    Texture ret;
    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return ret;
}

Texture Texture_CubemapCreate(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back)
{
    auto LoadFace = [](std::string& path, int id)
    {
        int width, height, numChannels;
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + id, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << path << std::endl;
        }
        stbi_image_free(data);
    };

    Texture ret;
    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ret);
    LoadFace(right, 1);
    LoadFace(left, 0);
    LoadFace(top, 2);
    LoadFace(bottom, 3);
    LoadFace(front, 4);
    LoadFace(back, 5);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return ret;
}

void Texture_Delete(Texture texture)
{
    glDeleteTextures(1, &texture);
}

JTexture::JTexture(std::string path)
{
    texture = Texture_Create(path, width, height, numChannels);
}

JTexture::JTexture(int width, int height)
{
    texture = Texture_Generate(width, height);
}

JTexture::JTexture(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back)
{
    texture = Texture_CubemapCreate(right, left, top, bottom, front, back);
    cubemap = true;
}

void JTexture::Use()
{
    if (!cubemap) glBindTexture(GL_TEXTURE_2D, texture);
    else glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

JTexture::~JTexture()
{
    Texture_Delete(texture);
}