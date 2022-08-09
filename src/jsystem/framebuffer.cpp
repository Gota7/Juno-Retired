#include "framebuffer.h"
#include <iostream>

Framebuffer Framebuffer_Create()
{
    Framebuffer ret;
    glGenFramebuffers(1, &ret);
    glBindFramebuffer(GL_FRAMEBUFFER, ret);
    return ret;
}

void Framebuffer_Delete(Framebuffer framebuffer)
{
    glDeleteFramebuffers(1, &framebuffer);
}

JFramebuffer::JFramebuffer(int width, int height) : texture(width, height)
{
    framebuffer = Framebuffer_Create();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.texture, 0);
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, EMPTY_FRAMEBUFFER);
}

void JFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

JFramebuffer::~JFramebuffer()
{
    glDeleteRenderbuffers(1, &renderbuffer);
    Framebuffer_Delete(framebuffer);
}