//
// Created by blueberry on 2022/11/30.
//


#include "Shader.h"

#include <sstream>
#include <fstream>
#include "Renderer.h"
#include <iostream>

Shader::Shader(const std::string &filepath) : m_FilePath(filepath), m_RendererID(0) {

    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
    auto location = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]))
}

void Shader::SetUniformVec3f(const std::string &name, const glm::vec3 &vector) {
    auto location = GetUniformLocation(name);
    GLCall(glUniform3fv(location, 1,&vector[0]))
}
void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    auto location = GetUniformLocation(name);
    GLCall(glUniform4f(location, v0, v1, v2, v3))
}

void Shader::SetUniform1f(const std::string &name, float value) {
    auto location = GetUniformLocation(name);
    GLCall(glUniform1f(location, value))
}

void Shader::SetUniform2f(const std::string &name, float v0, float v1) {
    auto location = GetUniformLocation(name);
    GLCall(glUniform2f(location, v0, v1))
}

void Shader::SetUniform1i(const std::string &name, int value) {
    auto location = GetUniformLocation(name);
    GLCall(glUniform1i(location, value))
}

void Shader::SetUniform1iv(const std::string &name, const int size, const int *value) {
    auto location = GetUniformLocation(name);
    GLCall(glUniform1iv(location, size, value))
}

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::BindUniformToPoint(const std::string &name, unsigned int point) {
    auto idx = GetUniformBlockIndex(name);
    GLCall(glUniformBlockBinding(m_RendererID, idx, point));
}

int Shader::GetUniformLocation(const std::string &name) const {

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    GLCall(auto location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}

unsigned int Shader::GetUniformBlockIndex(const std::string &name) {
    if (m_UniformBlockIndexCache.find(name) != m_UniformBlockIndexCache.end()) {
        return m_UniformBlockIndexCache[name];
    }
    GLCall(auto idx = glGetUniformBlockIndex(m_RendererID, name.c_str()));
    if (idx == GL_INVALID_INDEX) {
        std::cout << "Warning: uniform block '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformBlockIndexCache[name] = idx;
    return idx;
}


ShaderProgramSource Shader::ParseShader(const std::string &filepath) {
    std::ifstream stream(filepath);
    if (!stream.is_open()) {
        std::cerr << "Error: could not open shader file '" << filepath << "'" << std::endl;
        return {"", ""};
    }
    enum class ShaderType {
        NONE = -1,
        VERTEXT = 0,
        FRAGMENT = 1,
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEXT;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int) type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {

    GLCall(unsigned int id = glCreateShader(type));
    const char *src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *) alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}
