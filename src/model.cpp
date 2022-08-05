#include "model.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

void JModel::Render()
{
    shader.Use();
    for (int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i]->Use();
    }
    shader.SetMatrix("model", glm::value_ptr(matrix));
    buffers->Bind();
    glDrawElements(drawMode, drawCount, drawType, 0);
}