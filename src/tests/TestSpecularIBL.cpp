//
// Created by blueberry on 2026/6/7.
//

#include "TestSpecularIBL.h"

#include "../FileUtil.h"
#include "../VertexBufferLayout.h"
#include "../WindowManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

namespace {
    constexpr float kPi = 3.14159265359f;
    constexpr unsigned int kCaptureSize = 512;
    constexpr unsigned int kIrradianceSize = 32;
    constexpr unsigned int kBrdfLutSize = 512;
    constexpr unsigned int kPrefilterSize = 128;
    constexpr unsigned int kMaxPrefilterMipLevels = 5;

    const std::array<glm::mat4, 6> kCaptureViews = {
        glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
    };
}

namespace test {
    TestSpecularIBL::TestSpecularIBL()
        : m_Camera(glm::vec3(0.0f, 0.0f, 20.0f)) {
        SetupCursorCallback();
        m_WasDepthEnabled = glIsEnabled(GL_DEPTH_TEST);
        m_WasCubeMapSeamlessEnabled = glIsEnabled(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));

        m_PbrShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/test_ibl_specular_pbr.shader"));
        m_CubeShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_ibl_cube.shader"));
        m_IrradianceShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/irradiance_convolution.shader"));
        m_PrefilterShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/specular_convolution.shader"));
        m_BrdfShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/brdf.shader"));
        m_BackgroundShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/test_ibl_background.shader"));

        BuildSphereMesh();
        BuildCubeMesh();
        BuildQuadMesh();
        LoadHdrTexture();
        CreateEnvironmentCubemap();
        CaptureEnvironmentCubemap();
        CreateIrradianceCubemap();
        CaptureIrradianceCubemap();
        CreatePrefilterCubemap();
        CapturePrefilterCubemap();
        CreateBrdfLutTexture();
        CaptureBrdfLutTexture();

        m_PbrShader->Bind();
        m_PbrShader->SetUniformVec3f("albedo", m_Albedo);
        m_PbrShader->SetUniform1f("ao", m_Ao);
        m_PbrShader->SetUniform1i("irradianceMap", 0);
        m_PbrShader->SetUniform1i("prefilterMap", 1);
        m_PbrShader->SetUniform1i("brdfLUT", 2);

        m_CubeShader->Bind();
        m_CubeShader->SetUniform1i("equirectangularMap", 0);

        m_IrradianceShader->Bind();
        m_IrradianceShader->SetUniform1i("environmentMap", 0);

        m_PrefilterShader->Bind();
        m_PrefilterShader->SetUniform1i("environmentMap", 0);

        m_BackgroundShader->Bind();
        m_BackgroundShader->SetUniform1i("environmentMap", 0);
    }

    TestSpecularIBL::~TestSpecularIBL() {
        if (m_BrdfLutTexture != 0) {
            GLCall(glDeleteTextures(1, &m_BrdfLutTexture));
        }
        if (m_PrefilterMap != 0) {
            GLCall(glDeleteTextures(1, &m_PrefilterMap));
        }
        if (m_IrradianceMap != 0) {
            GLCall(glDeleteTextures(1, &m_IrradianceMap));
        }
        if (m_EnvCubemap != 0) {
            GLCall(glDeleteTextures(1, &m_EnvCubemap));
        }
        if (!m_WasDepthEnabled) {
            GLCall(glDisable(GL_DEPTH_TEST));
        }
        if (!m_WasCubeMapSeamlessEnabled) {
            GLCall(glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
        }
        GLCall(glDepthFunc(GL_LESS));
    }

    void TestSpecularIBL::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        int fbWidth = 0;
        int fbHeight = 0;
        WindowManager::shared().GetFramebufferSize(fbWidth, fbHeight);
        float aspect = (fbHeight != 0)
            ? static_cast<float>(fbWidth) / static_cast<float>(fbHeight)
            : (960.0f / 540.0f);

        glm::mat4 view = m_Camera.GetViewMatrix();
        float fov = glm::radians(m_Camera.GetZoom());
        glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);

        m_PbrShader->Bind();
        m_PbrShader->SetUniformMat4f("view", view);
        m_PbrShader->SetUniformMat4f("projection", projection);
        m_PbrShader->SetUniformVec3f("camPos", m_Camera.GetPosition());
        m_PbrShader->SetUniformVec3f("albedo", m_Albedo);
        m_PbrShader->SetUniform1f("ao", m_Ao);
        for (int i = 0; i < 4; ++i) {
            m_PbrShader->SetUniformVec3f("lightPositions[" + std::to_string(i) + "]", m_LightPositions[i]);
            m_PbrShader->SetUniformVec3f("lightColors[" + std::to_string(i) + "]", m_LightColors[i]);
        }

        m_BackgroundShader->Bind();
        m_BackgroundShader->SetUniformMat4f("view", view);
        m_BackgroundShader->SetUniformMat4f("projection", projection);
    }

    void TestSpecularIBL::OnRender() {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_PbrShader->Bind();
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap));
        GLCall(glActiveTexture(GL_TEXTURE1));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap));
        GLCall(glActiveTexture(GL_TEXTURE2));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_BrdfLutTexture));
        for (int row = 0; row < m_RowCount; ++row) {
            float metallic = static_cast<float>(row) / static_cast<float>(std::max(1, m_RowCount - 1));
            m_PbrShader->SetUniform1f("metallic", metallic);

            for (int col = 0; col < m_ColumnCount; ++col) {
                float roughness = static_cast<float>(col) / static_cast<float>(std::max(1, m_ColumnCount - 1));
                roughness = std::max(0.05f, std::min(roughness, 1.0f));

                glm::mat4 model(1.0f);
                model = glm::translate(
                    model,
                    glm::vec3(
                        (static_cast<float>(col) - static_cast<float>(m_ColumnCount) / 2.0f) * m_Spacing,
                        (static_cast<float>(row) - static_cast<float>(m_RowCount) / 2.0f) * m_Spacing,
                        0.0f
                    )
                );
                model = glm::scale(model, glm::vec3(m_SphereScale));
                glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

                m_PbrShader->SetUniformMat4f("model", model);
                m_PbrShader->SetUniformMat3f("normalMatrix", normalMatrix);
                m_PbrShader->SetUniform1f("roughness", roughness);
                DrawSphere();
            }
        }

        GLCall(glDepthFunc(GL_LEQUAL));
        m_BackgroundShader->Bind();
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap));
        m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_BackgroundShader);
        GLCall(glDepthFunc(GL_LESS));
    }

    void TestSpecularIBL::OnImGuiRender() {
        ImGui::ColorEdit3("Albedo", &m_Albedo[0]);
        ImGui::SliderFloat("AO", &m_Ao, 0.0f, 1.0f);
        ImGui::SliderFloat("Spacing", &m_Spacing, 1.2f, 4.0f);
        ImGui::SliderFloat("Sphere Scale", &m_SphereScale, 0.2f, 1.0f);
        ImGui::Text("Specular IBL: diffuse + prefilter + BRDF LUT");
        ImGui::Text("Background: newport_loft.hdr");
    }

    void TestSpecularIBL::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestSpecularIBL::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestSpecularIBL::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }

    void TestSpecularIBL::BuildSphereMesh() {
        const unsigned int xSegments = 64;
        const unsigned int ySegments = 64;

        m_SphereVertices.clear();
        m_SphereIndices.clear();
        m_SphereVertices.reserve((xSegments + 1) * (ySegments + 1) * 8);
        m_SphereIndices.reserve(xSegments * ySegments * 6);

        for (unsigned int y = 0; y <= ySegments; ++y) {
            for (unsigned int x = 0; x <= xSegments; ++x) {
                float xSegment = static_cast<float>(x) / static_cast<float>(xSegments);
                float ySegment = static_cast<float>(y) / static_cast<float>(ySegments);
                float xPos = std::cos(xSegment * 2.0f * kPi) * std::sin(ySegment * kPi);
                float yPos = std::cos(ySegment * kPi);
                float zPos = std::sin(xSegment * 2.0f * kPi) * std::sin(ySegment * kPi);

                m_SphereVertices.push_back(xPos);
                m_SphereVertices.push_back(yPos);
                m_SphereVertices.push_back(zPos);
                m_SphereVertices.push_back(xPos);
                m_SphereVertices.push_back(yPos);
                m_SphereVertices.push_back(zPos);
                m_SphereVertices.push_back(xSegment);
                m_SphereVertices.push_back(ySegment);
            }
        }

        for (unsigned int y = 0; y < ySegments; ++y) {
            for (unsigned int x = 0; x < xSegments; ++x) {
                unsigned int i0 = y * (xSegments + 1) + x;
                unsigned int i1 = i0 + 1;
                unsigned int i2 = (y + 1) * (xSegments + 1) + x;
                unsigned int i3 = i2 + 1;

                m_SphereIndices.push_back(i0);
                m_SphereIndices.push_back(i2);
                m_SphereIndices.push_back(i1);
                m_SphereIndices.push_back(i1);
                m_SphereIndices.push_back(i2);
                m_SphereIndices.push_back(i3);
            }
        }

        m_SphereVBO = std::make_shared<VertexBuffer>(
            m_SphereVertices.data(),
            static_cast<unsigned int>(m_SphereVertices.size() * sizeof(float)));
        m_SphereIBO = std::make_shared<IndexBuffer>(
            m_SphereIndices.data(),
            static_cast<unsigned int>(m_SphereIndices.size()));
        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_SphereVAO = std::make_shared<VertexArray>();
        m_SphereVAO->AddBuffer(*m_SphereVBO, layout);
    }

    void TestSpecularIBL::DrawSphere() const {
        m_PbrShader->Bind();
        m_SphereVAO->Bind();
        m_SphereIBO->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, m_SphereIBO->GetCount(), GL_UNSIGNED_INT, nullptr));
    }

    void TestSpecularIBL::BuildCubeMesh() {
        const float cubeVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
        const unsigned int cubeIndices[] = {
            0, 1, 2, 3, 4, 5,
            6, 7, 8, 9, 10, 11,
            12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23,
            24, 25, 26, 27, 28, 29,
            30, 31, 32, 33, 34, 35
        };

        m_CubeVBO = std::make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
        m_CubeIBO = std::make_shared<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(cubeIndices[0]));
        VertexBufferLayout layout{};
        layout.Push<float>(3);
        m_CubeVAO = std::make_shared<VertexArray>();
        m_CubeVAO->AddBuffer(*m_CubeVBO, layout);
    }

    void TestSpecularIBL::BuildQuadMesh() {
        const float quadVertices[] = {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f
        };

        m_QuadVBO = std::make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices));
        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_QuadVAO = std::make_shared<VertexArray>();
        m_QuadVAO->AddBuffer(*m_QuadVBO, layout);
    }

    void TestSpecularIBL::DrawQuad() {
        m_BrdfShader->Bind();
        m_QuadVAO->Bind();
        GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    }

    void TestSpecularIBL::LoadHdrTexture() {
        TextureInitParams params{};
        params.flip = 1;
        params.type = GL_TEXTURE_2D;
        params.internalFormat = GL_RGB16F;
        params.WRAP_S = GL_CLAMP_TO_EDGE;
        params.WRAP_T = GL_CLAMP_TO_EDGE;
        params.MIN_FILTER = GL_LINEAR;
        params.MAG_FILTER = GL_LINEAR;

        std::string hdrPath = FileUtil::shared().GetPath("./textures/hdr/newport_loft.hdr");
        m_HdrTexture = std::make_shared<Texture>(hdrPath, params);
        if (m_HdrTexture->GetID() == 0) {
            std::cerr << "Failed to load HDR texture: " << hdrPath << std::endl;
        }
    }

    void TestSpecularIBL::CreateEnvironmentCubemap() {
        GLCall(glGenTextures(1, &m_EnvCubemap));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap));
        for (unsigned int i = 0; i < 6; ++i) {
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                                kCaptureSize, kCaptureSize, 0, GL_RGB, GL_FLOAT, nullptr));
        }
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

        RenderBufferInitParams captureParams{};
        captureParams.width = kCaptureSize;
        captureParams.height = kCaptureSize;
        captureParams.format = GL_DEPTH24_STENCIL8;
        m_CaptureRBO = std::make_shared<RenderBuffer>(captureParams);

        m_CaptureFBO = std::make_shared<FrameBuffer>();
        m_CaptureFBO->AttachDepthStencil(m_CaptureRBO);
        m_CaptureFBO->Unbind();
    }

    void TestSpecularIBL::CaptureEnvironmentCubemap() {
        if (!m_HdrTexture || m_HdrTexture->GetID() == 0 || !m_CaptureFBO || m_EnvCubemap == 0) {
            return;
        }

        const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

        int viewportWidth = 0;
        int viewportHeight = 0;
        WindowManager::shared().GetViewportSize(viewportWidth, viewportHeight);

        GLCall(glViewport(0, 0, kCaptureSize, kCaptureSize));
        m_CaptureFBO->Bind();

        m_CubeShader->Bind();
        m_CubeShader->SetUniformMat4f("projection", captureProjection);
        m_HdrTexture->Bind(0);

        for (unsigned int i = 0; i < kCaptureViews.size(); ++i) {
            m_CubeShader->SetUniformMat4f("view", kCaptureViews[i]);
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                          GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvCubemap, 0));
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_CubeShader);
        }

        m_CaptureFBO->Unbind();
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap));
        GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
        if (viewportWidth > 0 && viewportHeight > 0) {
            GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
        }
    }

    void TestSpecularIBL::CreateIrradianceCubemap() {
        GLCall(glGenTextures(1, &m_IrradianceMap));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap));
        for (unsigned int i = 0; i < 6; ++i) {
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                                kIrradianceSize, kIrradianceSize, 0, GL_RGB, GL_FLOAT, nullptr));
        }
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }

    void TestSpecularIBL::CaptureIrradianceCubemap() {
        if (!m_CaptureFBO || !m_CaptureRBO || m_EnvCubemap == 0 || m_IrradianceMap == 0) {
            return;
        }

        const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

        int viewportWidth = 0;
        int viewportHeight = 0;
        WindowManager::shared().GetViewportSize(viewportWidth, viewportHeight);

        RenderBufferInitParams irradianceParams{};
        irradianceParams.width = kIrradianceSize;
        irradianceParams.height = kIrradianceSize;
        irradianceParams.format = GL_DEPTH24_STENCIL8;
        m_CaptureRBO = std::make_shared<RenderBuffer>(irradianceParams);
        m_CaptureFBO->AttachDepthStencil(m_CaptureRBO);

        GLCall(glViewport(0, 0, kIrradianceSize, kIrradianceSize));
        m_CaptureFBO->Bind();

        m_IrradianceShader->Bind();
        m_IrradianceShader->SetUniformMat4f("projection", captureProjection);
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap));

        for (unsigned int i = 0; i < kCaptureViews.size(); ++i) {
            m_IrradianceShader->SetUniformMat4f("view", kCaptureViews[i]);
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                          GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0));
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_IrradianceShader);
        }

        m_CaptureFBO->Unbind();
        if (viewportWidth > 0 && viewportHeight > 0) {
            GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
        }
    }

    void TestSpecularIBL::CreatePrefilterCubemap() {
        GLCall(glGenTextures(1, &m_PrefilterMap));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap));
        for (unsigned int i = 0; i < 6; ++i) {
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                                kPrefilterSize, kPrefilterSize, 0, GL_RGB, GL_FLOAT, nullptr));
        }
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }

    void TestSpecularIBL::CapturePrefilterCubemap() {
        if (!m_CaptureFBO || m_EnvCubemap == 0 || m_PrefilterMap == 0) {
            return;
        }

        const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

        int viewportWidth = 0;
        int viewportHeight = 0;
        WindowManager::shared().GetViewportSize(viewportWidth, viewportHeight);

        m_CaptureFBO->Bind();
        m_PrefilterShader->Bind();
        m_PrefilterShader->SetUniformMat4f("projection", captureProjection);
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap));

        for (unsigned int mip = 0; mip < kMaxPrefilterMipLevels; ++mip) {
            unsigned int mipWidth = static_cast<unsigned int>(kPrefilterSize * std::pow(0.5f, static_cast<float>(mip)));
            unsigned int mipHeight = static_cast<unsigned int>(kPrefilterSize * std::pow(0.5f, static_cast<float>(mip)));

            RenderBufferInitParams prefilterParams{};
            prefilterParams.width = static_cast<int>(mipWidth);
            prefilterParams.height = static_cast<int>(mipHeight);
            prefilterParams.format = GL_DEPTH24_STENCIL8;
            m_CaptureRBO = std::make_shared<RenderBuffer>(prefilterParams);
            m_CaptureFBO->AttachDepthStencil(m_CaptureRBO);
            GLCall(glViewport(0, 0, mipWidth, mipHeight));

            float roughness = static_cast<float>(mip) / static_cast<float>(kMaxPrefilterMipLevels - 1);
            m_PrefilterShader->SetUniform1f("roughness", roughness);

            for (unsigned int i = 0; i < kCaptureViews.size(); ++i) {
                m_PrefilterShader->SetUniformMat4f("view", kCaptureViews[i]);
                GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                              GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMap, mip));
                GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
                m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_PrefilterShader);
            }
        }

        m_CaptureFBO->Unbind();
        if (viewportWidth > 0 && viewportHeight > 0) {
            GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
        }
    }

    void TestSpecularIBL::CreateBrdfLutTexture() {
        GLCall(glGenTextures(1, &m_BrdfLutTexture));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_BrdfLutTexture));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, kBrdfLutSize, kBrdfLutSize, 0, GL_RG, GL_FLOAT, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void TestSpecularIBL::CaptureBrdfLutTexture() {
        if (!m_CaptureFBO || m_BrdfLutTexture == 0) {
            return;
        }

        int viewportWidth = 0;
        int viewportHeight = 0;
        WindowManager::shared().GetViewportSize(viewportWidth, viewportHeight);

        RenderBufferInitParams brdfParams{};
        brdfParams.width = kBrdfLutSize;
        brdfParams.height = kBrdfLutSize;
        brdfParams.format = GL_DEPTH24_STENCIL8;
        m_CaptureRBO = std::make_shared<RenderBuffer>(brdfParams);
        m_CaptureFBO->AttachDepthStencil(m_CaptureRBO);

        const bool wasBlendEnabled = glIsEnabled(GL_BLEND);
        if (wasBlendEnabled) {
            GLCall(glDisable(GL_BLEND));
        }

        GLCall(glViewport(0, 0, kBrdfLutSize, kBrdfLutSize));
        m_CaptureFBO->Bind();
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BrdfLutTexture, 0));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        DrawQuad();
        m_CaptureFBO->Unbind();

        if (wasBlendEnabled) {
            GLCall(glEnable(GL_BLEND));
        }
        if (viewportWidth > 0 && viewportHeight > 0) {
            GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
        }
    }
}
