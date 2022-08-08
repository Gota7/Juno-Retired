#pragma once

#include <glad/glad.h>
#include <string>

#define EMPTY_TEXTURE 0

typedef GLuint Texture;

// Create a texture.
Texture Texture_Create(std::string path, int& width, int& height, int& numChannels);

// Create a sized texture to write to later.
Texture Texture_Generate(int width, int height);

// Delete a texture.
void Texture_Delete(Texture texture);

// Texture structure.
struct JTexture
{
    Texture texture;
    int width, height, numChannels, id = 0;

    JTexture(std::string path);
    JTexture(int width, int height);
    void Use();
    ~JTexture();
};