#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

typedef GLuint Shader;
typedef std::vector<std::pair<std::string, GLenum>> ShaderList;
typedef GLuint ShaderProgram;

// Compile a shader from a file.
Shader Shader_Compile(std::string filePath, GLenum type);

// Delete a shader.
void Shader_Delete(Shader shader);

// Create a shader program.
ShaderProgram Shader_CreateProgram(ShaderList shaders);

// Delete a shader program.
void Shader_DeleteProgram(ShaderProgram program);

// Shader class for easier handling.
struct JShader
{
    ShaderProgram shaderProgram = 0;

    JShader(ShaderList shaders);
    void Use();
    void SetBool(const std::string& name, bool value);
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    ~JShader();
};