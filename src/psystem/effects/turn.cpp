#include "turn.h"

void PEffectTurn::Process(PParticle& particle, glm::vec3& vel)
{
    glm::mat4 turnMat;
    switch (axis)
    {
        case PEFFECT_TURN_AXIS_X: turnMat = glm::rotate(glm::mat4(1.0f), angleSpeed, glm::vec3(1.0f, 0.0f, 0.0f)); break;
        case PEFFECT_TURN_AXIS_Y: turnMat = glm::rotate(glm::mat4(1.0f), angleSpeed, glm::vec3(0.0f, 1.0f, 0.0f)); break;
        case PEFFECT_TURN_AXIS_Z: turnMat = glm::rotate(glm::mat4(1.0f), angleSpeed, glm::vec3(0.0f, 0.0f, 1.0f)); break;
    }
    particle.offset = glm::vec4(particle.offset, 1.0f) * turnMat;
}