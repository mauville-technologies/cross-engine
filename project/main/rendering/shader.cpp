//
// Created by ozzadar on 2021-09-13.
//

#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <core/graphics.h>
#include <glm/gtc/type_ptr.hpp>

OZZ::Shader::Shader(const std::string &vertexCode, const std::string& fragmentCode) {
    compileShader(vertexCode, fragmentCode);
}

OZZ::Shader::Shader(const char *vertexPath, const char *fragmentPath) {

    SDL_RWops* vertexFile {SDL_RWFromFile(vertexPath, "r")};
    size_t vertexFileLength {static_cast<size_t>(SDL_RWsize(vertexFile))};
    void* vertexData{SDL_LoadFile_RW(vertexFile, nullptr, 1)};
    std::string vertexCode{static_cast<char*>(vertexData), vertexFileLength};

    SDL_RWops* fragmentFile {SDL_RWFromFile(fragmentPath, "r")};
    size_t fragmentFileLength {static_cast<size_t>(SDL_RWsize(fragmentFile))};
    void* fragmentData{SDL_LoadFile_RW(fragmentFile, nullptr, 1)};
    std::string fragmentCode{static_cast<char*>(fragmentData), fragmentFileLength};

    compileShader(vertexCode, fragmentCode);
}

void OZZ::Shader::Bind() {
    if (_id) {
        glUseProgram(_id);
    }
}

void OZZ::Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), static_cast<int>(value));
}

void OZZ::Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void OZZ::Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void OZZ::Shader::SetMat4(const std::string &name, const glm::mat4 &matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void OZZ::Shader::compileShader(const std::string &vertexCode, const std::string &fragmentCode) {
    uint32_t vertex, fragment;

    int success;
    char infoLog[512];

    // Vertex Shader
    const char* vShaderCode = vertexCode.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    const char* fShaderCode = fragmentCode.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!fragment) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    _id = glCreateProgram();
    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    glLinkProgram(_id);

    glGetProgramiv(_id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(_id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::LINKING FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

