#include "shader.h"
#include "../fs.h"
#include <fstream>
#include <iostream>
#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

Shader Shader_Compile(std::string filePath, GLenum type)
{
    ZoneScopedN("Shader_Compile");
    TracyGpuZone("Shader_Compile");

#ifdef VULKAN
    return 0;
#else
    Shader ret = glCreateShader(type);
    std::string data = FLoader::ReadAllLines(filePath);
    const char* dataP = data.c_str();
    glShaderSource(ret, 1, &dataP, NULL);
    glCompileShader(ret);
    GLint success;
    char infoLog[512];
    glGetShaderiv(ret, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ret, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return ret;
#endif
}

void Shader_Delete(Shader shader)
{
    ZoneScopedN("Shader_Delete");
    TracyGpuZone("Shader_Delete");

#ifdef VULKAN
#else
    glDeleteShader(shader);
#endif
}

ShaderProgram Shader_CreateProgram(ShaderList shaders)
{
    ZoneScopedN("Shader_CreateProgram");
    TracyGpuZone("Shader_CreateProgram");

#ifdef VULKAN
    return 0;
#else
    ShaderProgram ret = glCreateProgram();
    std::vector<Shader> shds;
    for (auto& shd : shaders)
    {
        Shader s = Shader_Compile(shd.first, shd.second);
        glAttachShader(ret, s);
        shds.push_back(s);
    }
    glLinkProgram(ret);
    GLint success;
    char infoLog[512];
    glGetProgramiv(ret, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ret, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    for (auto& shd : shds)
    {
        Shader_Delete(shd);
    }
    return ret;
#endif
}

void Shader_DeleteProgram(ShaderProgram program)
{
    ZoneScopedN("Shader_DeleteProgram");
    TracyGpuZone("Shader_DeleteProgram");

#ifdef VULKAN
#else
    glDeleteProgram(program);
#endif
}

JShader::JShader(ShaderList shaders)
{
    ZoneScopedN("JShader::JShader");
    shaderProgram = Shader_CreateProgram(shaders);
}

void JShader::Use()
{
    ZoneScopedN("JShader::Use");
    TracyGpuZone("JShader::Use");

#ifdef VULKAN
#else
    glUseProgram(shaderProgram);
#endif
}

GLint JShader::GetUniformLocation(const std::string& name)
{
#ifdef VULKAN
    return 0;
#else
    auto item = uniformLocations.find(name);
    if (item == uniformLocations.end())
    {
        GLint ret = glGetUniformLocation(shaderProgram, name.c_str());
        uniformLocations[name] = ret;
        return ret;
    }
    else
    {
        return item->second;
    }
#endif
}

void JShader::SetBool(const std::string& name, bool value)
{
    ZoneScopedN("JShader::SetBool");
    TracyGpuZone("JShader::SetBool");

#ifdef VULKAN
#else
    glUniform1i(GetUniformLocation(name), (int)value);
#endif
}

void JShader::SetInt(const std::string& name, int value)
{
    ZoneScopedN("JShader::SetInt");
    TracyGpuZone("JShader::SetInt");

#ifdef VULKAN
#else
    glUniform1i(GetUniformLocation(name), value);
#endif
}

void JShader::SetFloat(const std::string& name, float value)
{
    ZoneScopedN("JShader::SetFloat");
    TracyGpuZone("JShader::SetFloat");

#ifdef VULKAN
#else
    glUniform1f(GetUniformLocation(name), value);
#endif
}

void JShader::SetVec3(const std::string& name, const GLfloat* value)
{
    ZoneScopedN("JShader::SetVec3");
    TracyGpuZone("JShader::SetVec3");

#ifdef VULKAN
#else
    glUniform3fv(GetUniformLocation(name), 1, value);
#endif
}

void JShader::SetVec4(const std::string& name, const GLfloat* value)
{
    ZoneScopedN("JShader::SetVec4");
    TracyGpuZone("JShader::SetVec4");

#ifdef VULKAN
#else
    glUniform4fv(GetUniformLocation(name), 1, value);
#endif
}

void JShader::SetMatrix3(const std::string& name, const GLfloat* value)
{
    ZoneScopedN("JShader::SetMatrix3");
    TracyGpuZone("JShader::SetMatrix3");

#ifdef VULKAN
#else
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, value);
#endif
}

void JShader::SetMatrix(const std::string& name, const GLfloat* value)
{
    ZoneScopedN("JShader::SetMatrix");
    TracyGpuZone("JShader::SetMatrix");

#ifdef VULKAN
#else
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value);
#endif
}

JShader::~JShader()
{
    ZoneScopedN("JShader::~JShader");
    Shader_DeleteProgram(shaderProgram);
}