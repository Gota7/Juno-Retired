#include "shader.h"
#include <fstream>
#include <iostream>

Shader Shader_Compile(std::string filePath, GLenum type)
{
    Shader ret = glCreateShader(type);
    std::ifstream inFile(filePath.c_str());
    std::string data = std::string(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>());
    const char* dataP = data.c_str();
    inFile.close();
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
    glDeleteShader(shader);
}

ShaderProgram Shader_CreateProgram(ShaderList shaders)
{
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
    glDeleteProgram(program);
}

JShader::JShader(ShaderList shaders)
{
    shaderProgram = Shader_CreateProgram(shaders);
}

void JShader::Use()
{
    glUseProgram(shaderProgram);
}

void JShader::SetBool(const std::string& name, bool value)
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void JShader::SetInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void JShader::SetFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void JShader::SetVec3(const std::string& name, const GLfloat* value)
{
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, value);
}

void JShader::SetMatrix(const std::string& name, const GLfloat* value)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, value);
}

JShader::~JShader()
{
    Shader_DeleteProgram(shaderProgram);
}