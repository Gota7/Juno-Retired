#include "manager.h"

// Particle mesh data.
VertexUV particleVertices[] = {
    VertexUV(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
    VertexUV(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
    VertexUV(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
    VertexUV(glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f))
};
int particleIndices[] = {
    0, 1, 2,
    0, 2, 3
};

PManager::PManager(int systemsToReserve)
{
    systems.reserve(systemsToReserve);
    mesh = std::make_unique<JMesh>(
        particleVertices,
        sizeof(particleVertices),
        GL_STATIC_DRAW,
        particleIndices,
        sizeof(particleIndices),
        GL_STATIC_DRAW,
        GL_TRIANGLES,
        sizeof(particleIndices) / sizeof(particleIndices[0]),
        GL_UNSIGNED_INT,
        0
    );
    VertexUV::SetAttributes();
}

PSystem& PManager::AddSystem(std::string name, glm::vec3 pos, glm::vec3* dir)
{
    if (definitions.find(name) == definitions.end())
    {
        definitions[name] = PSystemDefinition(textureCache, name);
    }
    return AddSystem(definitions[name], pos, dir);
}

PSystem& PManager::AddSystem(PSystemDefinition& def, glm::vec3 pos, glm::vec3* dir)
{
    return systems.emplace_back(def, pos, dir);
}

PSystemGravity& PManager::AddSystemGravity(std::string name, std::unique_ptr<RGravity> gravity)
{
    if (definitions.find(name) == definitions.end())
    {
        definitions[name] = PSystemDefinition(textureCache, name);
    }
    return AddSystemGravity(definitions[name], std::move(gravity));
}

PSystemGravity& PManager::AddSystemGravity(PSystemDefinition& def, std::unique_ptr<RGravity> gravity)
{
    return gravitySystems.emplace_back(def, std::move(gravity));
}

void PManager::Update()
{
    for (int i = systems.size() - 1; i >= 0; i--)
    {
        PSystem& sys = systems[i];
        if (!sys.paused) sys.Update(this);
        PSpawnInfo& info = sys.definition->spawnInfo;
        if ((info.spawnTime != 0 && sys.age > info.spawnTime || sys.stopped)
            && sys.particles.size() == 0 && sys.glitterParticles.size() == 0)
        {
            sys.stopped = true;
            systems.erase(systems.begin() + i);
        }
    }
    for (int i = gravitySystems.size() - 1; i >= 0; i--)
    {
        PSystem& sys = gravitySystems[i];
        if (!sys.paused) sys.Update(this);
        PSpawnInfo& info = sys.definition->spawnInfo;
        if ((info.spawnTime != 0 && sys.age > info.spawnTime || sys.stopped)
            && sys.particles.size() == 0 && sys.glitterParticles.size() == 0)
        {
            sys.stopped = true;
            gravitySystems.erase(gravitySystems.begin() + i);
        }
    }
}

void PManager::Render(JShader& shader)
{
    shader.Use();
    shader.SetInt("tex", 0);
    shader.SetFloat("aspect", JFrame::aspect);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_CULL_FACE);
    for (auto& system : systems)
        system.Render(this, shader);
    for (auto& system : gravitySystems)
        system.Render(this, shader);
    glEnable(GL_CULL_FACE);
}