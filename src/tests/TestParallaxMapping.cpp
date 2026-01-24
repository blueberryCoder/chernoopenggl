//
// Created by blueberry on 2026/01/24.
//

#include "TestParallaxMapping.h"

#include "../FileUtil.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {
    TestParallaxMapping::TestParallaxMapping()
        : m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
          m_LightPos(0.5f, 1.0f, 0.3f),
          m_HeightScale(0.1f),
          m_ParallaxMode(0) {
        SetupCursorCallback();

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_parallax.shader"));
        m_SteepShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_steep_parallax.shader"));
        m_OcclusionShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_occlusion_parallax.shader"));
        m_DiffuseTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/bricks2.jpg"));
        m_NormalTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/bricks2_normal.jpg"));
        m_DepthTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/bricks2_disp.jpg"));

        glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3(1.0f, -1.0f, 0.0f);
        glm::vec3 pos4(1.0f, 1.0f, 0.0f);

        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);

        glm::vec3 normal(0.0f, 0.0f, 1.0f);
        glm::vec3 tangent1, bitangent1, tangent2, bitangent2;

        // Triangle 1
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        tangent1 = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
        bitangent1 = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

        // Triangle 2
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;
        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        tangent2 = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
        bitangent2 = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

        float quadVertices[] = {
            // positions            // normals         // texcoords // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, uv1.x, uv1.y,
            tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos2.x, pos2.y, pos2.z, normal.x, normal.y, normal.z, uv2.x, uv2.y,
            tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, uv3.x, uv3.y,
            tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, normal.x, normal.y, normal.z, uv1.x, uv1.y,
            tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,

            pos3.x, pos3.y, pos3.z, normal.x, normal.y, normal.z, uv3.x, uv3.y,
            tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,

            pos4.x, pos4.y, pos4.z, normal.x, normal.y, normal.z, uv4.x, uv4.y,
            tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };

        unsigned int quadIndices[] = {0, 1, 2, 3, 4, 5};

        m_VBO = std::make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices));
        m_IBO = std::make_shared<IndexBuffer>(quadIndices, 6);

        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        layout.Push<float>(3);
        layout.Push<float>(3);
        m_VAO = std::make_shared<VertexArray>();
        m_VAO->AddBuffer(*m_VBO, layout);

        m_Shader->Bind();
        m_Shader->SetUniform1i("diffuseMap", 0);
        m_Shader->SetUniform1i("normalMap", 1);
        m_Shader->SetUniform1i("depthMap", 2);
        m_SteepShader->Bind();
        m_SteepShader->SetUniform1i("diffuseMap", 0);
        m_SteepShader->SetUniform1i("normalMap", 1);
        m_SteepShader->SetUniform1i("depthMap", 2);
        m_OcclusionShader->Bind();
        m_OcclusionShader->SetUniform1i("diffuseMap", 0);
        m_OcclusionShader->SetUniform1i("normalMap", 1);
        m_OcclusionShader->SetUniform1i("depthMap", 2);

        GLCall(glEnable(GL_DEPTH_TEST));
    }

    TestParallaxMapping::~TestParallaxMapping() {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestParallaxMapping::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        glm::mat4 view = m_Camera.GetViewMatrix();
        float fov = glm::radians(m_Camera.GetZoom());
        glm::mat4 projection = glm::perspective<float>(fov, 960.0f / 540.0f, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        Shader *activeShader = m_Shader.get();
        if (m_ParallaxMode == 1) {
            activeShader = m_SteepShader.get();
        } else if (m_ParallaxMode == 2) {
            activeShader = m_OcclusionShader.get();
        }
        activeShader->Bind();
        activeShader->SetUniformMat4f("projection", projection);
        activeShader->SetUniformMat4f("view", view);
        activeShader->SetUniformMat4f("model", model);
        activeShader->SetUniformVec3f("lightPos", m_LightPos);
        activeShader->SetUniformVec3f("viewPos", m_Camera.GetPosition());
        activeShader->SetUniform1f("height_scale", m_HeightScale);
    }

    void TestParallaxMapping::OnRender() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        Renderer renderer;

        Shader *activeShader = m_Shader.get();
        if (m_ParallaxMode == 1) {
            activeShader = m_SteepShader.get();
        } else if (m_ParallaxMode == 2) {
            activeShader = m_OcclusionShader.get();
        }
        activeShader->Bind();
        m_DiffuseTexture->Bind(0);
        m_NormalTexture->Bind(1);
        m_DepthTexture->Bind(2);
        renderer.Draw(*m_VAO, *m_IBO, *activeShader);
    }

    void TestParallaxMapping::OnImGuiRender() {
        ImGui::RadioButton("Parallax", &m_ParallaxMode, 0);
        ImGui::RadioButton("Steep Parallax", &m_ParallaxMode, 1);
        ImGui::RadioButton("Occlusion Parallax", &m_ParallaxMode, 2);
        ImGui::DragFloat3("Light Pos", &m_LightPos.x, 0.05f);
        ImGui::DragFloat("Height Scale", &m_HeightScale, 0.01f, 0.0f, 1.0f);
    }

    void TestParallaxMapping::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestParallaxMapping::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestParallaxMapping::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }
}
