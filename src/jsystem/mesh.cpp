#include "mesh.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

void JMesh::Render(unsigned int instanceCount)
{
    buffers->Bind();
    if (instanceCount) glDrawElementsInstanced(drawMode, drawCount, drawType, 0, instanceCount);
    else glDrawElements(drawMode, drawCount, drawType, 0);
}