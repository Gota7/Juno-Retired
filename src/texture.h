#pragma once

#include <glad/glad.h>
#include <string>

typedef GLuint Texture;

// Create a texture.
Texture Texture_Create(std::string path, int& width, int& height, int& numChannels);

// Delete a texture.
void Texture_Delete(Texture texture);

// Texture structure.
struct JTexture
{
    Texture texture;
    int width, height, numChannels;

    JTexture(std::string path);
    void Use();
    ~JTexture();
};