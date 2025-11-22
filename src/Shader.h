//
// Created by blueberry on 2022/11/30.
//

#ifndef MYOPENGL_SHADER_H
#define MYOPENGL_SHADER_H

#include <string>
#include <unordered_map>

#include "UniformBuffer.h"
#include "glm/glm.hpp"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
    std::string GeometrySource;
    int VertexStartLine = 0;
    int FragmentStartLine = 0;
    int GeometryStartLine = 0;
};

class Shader {
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
    mutable std::unordered_map<std::string, unsigned int> m_UniformBlockIndexCache;

public:
    Shader(const std::string &filepath);

    ~Shader();

    void Bind() const;

    void Unbind() const;

    void SetUniformMat4f(const std::string &name,const glm::mat4& matrix);
    void SetUniformVec3f(const std::string &name,const glm::vec3& vector);
    void SetUniformVec2f(const std::string &name,const glm::vec2 &vector);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void SetUniform1f(const std::string &name, float value);
    void SetUniform2f(const std::string &name, float v0, float v1);
    void SetUniform1i(const std::string &name, int value);
    void SetUniform1iv(const std::string &name, const int size, const int* value);

    void BindUniformToPoint(const std::string &name, unsigned int);

private:

    ShaderProgramSource ParseShader(const std::string &filepath);

    unsigned int CompileShader(unsigned int type, const std::string &source, int startLine);

    unsigned int CreateShader(const ShaderProgramSource &source);

    int GetUniformLocation(const std::string &name) const;

    unsigned int GetUniformBlockIndex(const std::string &name);
};


#endif //MYOPENGL_SHADER_H
