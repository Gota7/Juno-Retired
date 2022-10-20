#pragma once

#include "constants.h"
#include "texture.h"
#include <glad/glad.h>

#define EMPTY_FRAMEBUFFER 0

typedef GLuint Framebuffer;
typedef GLuint Renderbuffer;

// Create a framebuffer.
Framebuffer Framebuffer_Create();

// Delete a framebuffer.
void Framebuffer_Delete(Framebuffer framebuffer);

// Standard framebuffer.
struct JFramebuffer
{
    Framebuffer framebuffer;
    Renderbuffer renderbuffer;
    JTexture texture;

    JFramebuffer(int width, int height);
    void Bind();
    ~JFramebuffer();
};