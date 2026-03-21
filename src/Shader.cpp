//
// Created by blueberry on 2022/11/30.
//


#include "Shader.h"

#include <sstream>
#include <fstream>
#include "Renderer.h"
#include <iostream>
#include <cctype>

Shader::Shader(const std::string &filepath) : m_FilePath(filepath), m_RendererID(0) {

    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
    auto location = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]))
}

void Shader::SetUniformMat3f(const std::string &name, const glm::mat3 &matrix) {
    auto location = GetUniformLocation(name);
    GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]))
}

void Shader::SetUniformVec3f(const std::string &name, const glm::vec3 &vector) {
    auto location = GetUniformLocation(name);
    GLCall(glUniform3fv(location, 1,&vector[0]))
}
void Shader::SetUniformVec2f(const std::string &name, const glm::vec2 &vector) {
    auto location = GetUniformLocation(name);
    GLCall(glUniform2fv(location, 1,&vector[0]))
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
        return {"", "", "", 0, 0, 0};
    }
    enum class ShaderType {
        NONE = -1,
        VERTEXT = 0,
        FRAGMENT = 1,
        GEOMETRY = 2,
    };

    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;
    int lineNumber = 0;
    ShaderProgramSource result;
    while (getline(stream, line)) {
        lineNumber++;
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEXT;
                result.VertexStartLine = lineNumber + 1;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
                result.FragmentStartLine = lineNumber + 1;
            } else if (line.find("geometry") != std::string::npos || line.find("geomerty") != std::string::npos) {
                type = ShaderType::GEOMETRY;
                result.GeometryStartLine = lineNumber + 1;
            }
        } else {
            ss[(int) type] << line << '\n';
        }
    }

    result.VertexSource = ss[0].str();
    result.FragmentSource = ss[1].str();
    result.GeometrySource = ss[2].str();
    return result;
}

static std::string AdjustShaderLog(const std::string &log, int startLine) {
    if (startLine <= 0) {
        return log;
    }
    std::stringstream input(log);
    std::string line;
    std::string result;
    while (std::getline(input, line)) {
        size_t pos = line.find("0:");
        if (pos != std::string::npos) {
            size_t start = pos + 2;
            size_t end = start;
            while (end < line.size() && std::isdigit(static_cast<unsigned char>(line[end]))) {
                end++;
            }
            if (end > start) {
                int lineNumber = std::stoi(line.substr(start, end - start));
                int adjusted = lineNumber + startLine - 1;
                line.replace(start, end - start, std::to_string(adjusted));
            }
        }
        result += line;
        if (!input.eof()) {
            result.push_back('\n');
        }
    }
    return result;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source, int startLine) {

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
        std::string shaderType;
        switch (type) {
            case GL_VERTEX_SHADER:
                shaderType = "vertex";
                break;
            case GL_FRAGMENT_SHADER:
                shaderType = "fragment";
                break;
            case GL_GEOMETRY_SHADER:
                shaderType = "geometry";
                break;
            default:
                shaderType = "unknown";
                break;
        }
        std::cout << "Failed to compile " << shaderType << std::endl;
        std::cout << AdjustShaderLog(message, startLine) << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const ShaderProgramSource &source) {
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource, source.VertexStartLine);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource, source.FragmentStartLine);
    unsigned int gs = 0;

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    if (!source.GeometrySource.empty()) {
        gs = CompileShader(GL_GEOMETRY_SHADER, source.GeometrySource, source.GeometryStartLine);
        if (gs != 0) {
            GLCall(glAttachShader(program, gs));
        }
    }
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    if (gs != 0) {
        GLCall(glDeleteShader(gs));
    }
    return program;
}
