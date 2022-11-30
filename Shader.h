//
// Created by blueberry on 2022/11/30.
//

#ifndef MYOPENGL_SHADER_H
#define MYOPENGL_SHADER_H

#include <string>
#include <unordered_map>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string &filepath);

    ~Shader();

    void Bind() const;

    void Unbind() const;

    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void SetUniform1f(const std::string &name, float value);

private:

    ShaderProgramSource ParseShader(const std::string &filepath);

    unsigned int CompileShader(unsigned int type, const std::string &source);

    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

     int GetUniformLocation(const std::string &name);
};


#endif //MYOPENGL_SHADER_H