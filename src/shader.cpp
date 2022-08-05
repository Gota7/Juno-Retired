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