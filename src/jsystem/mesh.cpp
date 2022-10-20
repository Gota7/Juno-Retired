#include "mesh.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

void JMesh::Render(unsigned int instanceCount)
{
    ZoneScopedN("JMesh::Render");
    TracyGpuZone("JMesh::Render");

#ifdef VULKAN
#else
    buffers->Bind();
    if (instanceCount) glDrawElementsInstanced(drawMode, drawCount, drawType, 0, instanceCount);
    else glDrawElements(drawMode, drawCount, drawType, 0);
#endif
}