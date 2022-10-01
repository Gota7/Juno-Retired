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
}

void Shader_Delete(Shader shader)
{
    ZoneScopedN("Shader_Delete");
    TracyGpuZone("Shader_Delete");
    glDeleteShader(shader);
}

ShaderProgram Shader_CreateProgram(ShaderList shaders)
{
    ZoneScopedN("Shader_CreateProgram");
    TracyGpuZone("Shader_CreateProgram");
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
}

void Shader_DeleteProgram(ShaderProgram program)
{
    ZoneScopedN("Shader_DeleteProgram");
    TracyGpuZone("Shader_DeleteProgram");
    glDeleteProgram(program);
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
    glUseProgram(shaderProgram);
}

void JShader::SetBool(const std::string& name, bool value)
{
    ZoneScopedN("JShader::SetBool");
    TracyGpuZone("JShader::SetBool");
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void JShader::SetInt(const std::string& name, int value)
{
    ZoneScopedN("JShader::SetInt");
    TracyGpuZone("JShader::SetInt");
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void JShader::SetFloat(const std::string& name, float value)
{
    ZoneScopedN("JShader::SetFloat");
    TracyGpuZone("JShader::SetFloat");
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void JShader::SetVec3(const std::string& name, const GLfloat* value)
{
    ZoneScopedN("JShader::SetVec3");
    TracyGpuZone("JShader::SetVec3");
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, value);
}

void JShader::SetVec4(const std::string& name, const GLfloat* value)
{
    ZoneScopedN("JShader::SetVec4");
    TracyGpuZone("JShader::SetVec4");
    glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, value);
}

void JShader::SetMatrix(const std::string& name, const GLfloat* value)
{
    ZoneScopedN("JShader::SetMatrix");
    TracyGpuZone("JShader::SetMatrix");
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, value);
}

JShader::~JShader()
{
    ZoneScopedN("JShader::~JShader");
    Shader_DeleteProgram(shaderProgram);
}