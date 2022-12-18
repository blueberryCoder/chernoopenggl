//
// Created by blueberry on 2022/12/14.
//

#include "TestMeanBlur.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "../Renderer.h"
// http://www.songho.ca/opengl/gl_fbo.html
// https://github.com/JYangkai/MediaDemo
// https://learnopengl.com/Advanced-OpenGL/Framebuffers
// https://zh.wikipedia.org/wiki/%E6%AD%A3%E6%80%81%E5%88%86%E5%B8%83


namespace test {

    TestMeanBlur::TestMeanBlur() {

        CreateFBOColorTexture();
        CreateDepthTexture();
        CreateFrameBuffer();

        GLint values[4] = {};
        glGetIntegerv(GL_VIEWPORT,values);
        glViewport(0,0,960,540);

        DrawOnFBO();

        glViewport(values[0],values[1],values[2],values[3]);

        CreateDstProgram();
    }

    void TestMeanBlur::CreateDstProgram() {
        float dstPositions[] = {
                -0.5, -0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.0f, 1.0f,
        };

        unsigned int dstIndices[] = {0, 1, 2, 2, 3, 0};

        GLCall(glGenVertexArrays(1, &m_dstVAO));
        GLCall(glBindVertexArray(m_dstVAO));

        GLCall(glGenBuffers(1, &m_dstVBO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_dstVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), dstPositions, GL_STATIC_DRAW));

        GLCall(glGenBuffers(1, &m_dstIBO));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_dstIBO));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), dstIndices, GL_STATIC_DRAW));
        GLCall(glEnableVertexAttribArray(0));

        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), 0));
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (const void *) (2 * sizeof(float))));

        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorTexture));

        dstShader = std::make_unique<Shader>("../../res/shaders/one_texture.shader");
        dstShader->Bind();
    }

    void TestMeanBlur::DrawOnFBO() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
        float positions[] = {
                -1.0, -1.0f, 0.0f,0.0f,
                1.0f, -1.0f, 1.0f,0.0f,
                1.0f, 1.0f, 1.0f,1.0f,
                -1.0f, 1.0f, 0.0f,1.0f
        };
        unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));
        unsigned int vbo;
        GLCall(glGenBuffers(1, &vbo));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), positions, GL_STATIC_DRAW));
        unsigned int ibo;
        GLCall(glGenBuffers(1, &ibo));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (const void *) 0))
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (const void *) (2 * sizeof(float))))

        m_OrgImgTexture = std::make_unique<Texture>("../../res/textures/liuyifei.png");
        m_OrgImgTexture->Bind(0);

        if(m_BlurType == BlurType::MEAN) {
            fboShader = std::make_unique<Shader>("../../res/shaders/image_mean_blur.shader");
            int radius = 20;
            float offset = 0.005;
            fboShader->Bind();
            fboShader->SetUniform1i("u_BlurRadius",radius);
            fboShader->SetUniform2f("u_BlurOffset",offset, 0);
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            fboShader->SetUniform2f("u_BlurOffset",0, offset);
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
            fboShader->Unbind();
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        }else if(m_BlurType == BlurType::GAUSSIAN) {
            fboShader = std::make_unique<Shader>("../../res/shaders/image_gaussian_blur.shader");

            int radius = 50;
            float offset = 1.0/200.0f;
            float PI = 3.1415926;
            float totalWeight = 0;
            float sigma = 5.0f;
            for (int i = 0; i < radius; i++) {
                float weight = (1.0f / sqrt(2.0f * PI * sigma * sigma)) * exp(-(i * i) / (2.0f * sigma * sigma));
                totalWeight += weight;
                if (i != 0) {
                    totalWeight += weight;
                }
            }

            fboShader->Bind();
            fboShader->SetUniform1i("u_BlurRadius", radius);
            fboShader->SetUniform2f("u_BlurOffset", offset, 0);
            fboShader->SetUniform1f("u_SumWeight",totalWeight);
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
            fboShader->SetUniform2f("u_BlurOffset", 0, offset);
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
            fboShader->Unbind();
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }


    }

    void TestMeanBlur::CreateFrameBuffer() {
        GLCall(glGenFramebuffers(1, &m_FBO))
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO))
        // attach
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0));
        GLCall(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));

        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "[OpenGL Error] FRAMEBUFFER COMPLETE status = " << status << std::endl;
        }
    }

    void TestMeanBlur::CreateDepthTexture() {
        GLCall(glGenTextures(1, &m_DepthTexture));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthTexture));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, fbo_width, fbo_height, 0, GL_DEPTH_COMPONENT,
                            GL_UNSIGNED_BYTE, nullptr));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void TestMeanBlur::CreateFBOColorTexture() {
        GLCall(glGenTextures(1, &m_ColorTexture))
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorTexture))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER))
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER))
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_width, fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    TestMeanBlur::~TestMeanBlur() {
    }

    void TestMeanBlur::OnImGuiRender() {
    }

    void TestMeanBlur::OnUpdate(float deltaTime) {
    }

    void TestMeanBlur::OnRender() {
        GLCall(glBindVertexArray(m_dstVAO));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_dstIBO));
        dstShader->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    }

}
