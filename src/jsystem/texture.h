#pragma once

#include <glad/glad.h>
#include <string>

#define EMPTY_TEXTURE 0

typedef GLuint Texture;

// Create a texture.
Texture Texture_Create(std::string path, int& width, int& height, int& numChannels);

// Create a sized texture to write to later.
Texture Texture_Generate(int width, int height);

// Create a cubemap texture.
Texture Texture_CubemapCreate(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back);

// Delete a texture.
void Texture_Delete(Texture texture);

// Texture structure.
struct JTexture
{
    Texture texture;
    int width, height, numChannels, id = 0;
    bool cubemap = false;

    JTexture(std::string path);
    JTexture(int width, int height);
    JTexture(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back);
    void Use();
    ~JTexture();
};