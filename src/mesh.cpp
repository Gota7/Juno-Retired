#include "mesh.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

void JMesh::Render()
{
    buffers->Bind();
    glDrawElements(drawMode, drawCount, drawType, 0);
}