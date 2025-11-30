//
// Created by blueberry on 2024/7/5.
//

#include "TestAsteroid.h"
#include "../FileUtil.h"

#include <GL/glew.h>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"
#include "imgui/imgui.h"
#include <random>
#include <cmath>
#include  "../src/VertexBufferLayout.h"

namespace test {
    namespace {
        constexpr float kDefaultAspect = 960.0f / 540.0f;
    }

    TestAsteroid::TestAsteroid()
        : m_Camera(glm::vec3(0.0f, 0.0f, 15.0f)),
          m_ViewMatrix(1.0f),
          m_ProjectionMatrix(1.0f) {
        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/assimp.shader"));
        m_InstanceShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/asteroid.shader"));
        m_PlanetModel = std::make_shared<Model>(FileUtil::shared().GetPath("./models/planet/planet.obj"));
        m_AsteroidModel = std::make_shared<Model>(FileUtil::shared().GetPath("./models/rock/rock.obj"));

        std::default_random_engine engine(42);
        // Wider belt and size variance.
        std::uniform_real_distribution<float> offset(-5.0f, 5.0f);
        std::uniform_real_distribution<float> scale(0.002f, 0.85f);
        std::uniform_real_distribution<float> rot(0.0f, glm::two_pi<float>());
        m_AsteroidTransforms.reserve(m_AsteroidCount);
        const float radius = 25.0f;
        for (unsigned int i = 0; i < m_AsteroidCount; ++i) {
            float angle = static_cast<float>(i) / static_cast<float>(m_AsteroidCount) * glm::two_pi<float>();
            float displacement = offset(engine);
            float x = std::sin(angle) * radius + displacement;
            float y = offset(engine) * 0.6f; // keep the belt relatively flat but less uniform
            float z = std::cos(angle) * radius + offset(engine);

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

            float s = scale(engine);
            model = glm::scale(model, glm::vec3(s));

            float rotationAngle = rot(engine);
            model = glm::rotate(model, rotationAngle, glm::normalize(glm::vec3(0.4f, 0.6f, 0.8f)));

            m_AsteroidTransforms.push_back(model);
        }
        m_RocksVertexBuffer = std::make_shared<VertexBuffer>(m_AsteroidTransforms.data(),
                                                             m_AsteroidTransforms.size() * sizeof(glm::mat4));
        VertexBufferLayout layout{};
        // Mat4 is four vec4 attributes; split them so each column gets its own attribute slot.
        layout.Push<float>(4);
        layout.Push<float>(4);
        layout.Push<float>(4);
        layout.Push<float>(4);
        m_InstanceShader->Bind();
        // Attach instance transforms to the asteroid meshes (not the planet).
        for (auto &mesh: m_AsteroidModel->GetMeshes()) {
            auto varr = mesh.GetVertexArray();
            varr->Bind();
            // Planet mesh uses locations 0..2, so instance matrix starts at location 3.
            varr->AddBuffer(*m_RocksVertexBuffer, layout, 3);
            GLCall(glVertexAttribDivisor(3, 1));
            GLCall(glVertexAttribDivisor(4, 1));
            GLCall(glVertexAttribDivisor(5, 1));
            GLCall(glVertexAttribDivisor(6, 1));
        }
        GLCall(glEnable(GL_DEPTH_TEST));
        SetupCursorCallback();
    }

    TestAsteroid::~TestAsteroid() {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestAsteroid::OnUpdate(float deltaTime) {
        (void) deltaTime;
        updateMatrices();
    }

    void TestAsteroid::OnRender() {
        if (!m_Shader || !m_PlanetModel) {
            return;
        }

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("view", m_ViewMatrix);
        m_Shader->SetUniformMat4f("projection", m_ProjectionMatrix);

        glm::mat4 planetModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
        m_Shader->SetUniformMat4f("model", planetModel);
        m_PlanetModel->Draw(*m_Shader);

        if (!m_AsteroidModel) {
            return;
        }

        // Instance shader still needs the camera matrices; per-instance model comes from the attribute buffer.
        m_InstanceShader->Bind();
        m_InstanceShader->SetUniformMat4f("view", m_ViewMatrix);
        m_InstanceShader->SetUniformMat4f("projection", m_ProjectionMatrix);
        m_AsteroidModel->Draw(*m_InstanceShader, m_AsteroidTransforms.size());

    }

    void TestAsteroid::OnImGuiRender() {
    }

    void TestAsteroid::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestAsteroid::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestAsteroid::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }

    void TestAsteroid::updateMatrices() {
        m_ViewMatrix = m_Camera.GetViewMatrix();
        float fov = glm::radians(m_Camera.GetZoom());
        m_ProjectionMatrix = glm::perspective(fov, kDefaultAspect, 0.1f, 100.0f);
    }
}
