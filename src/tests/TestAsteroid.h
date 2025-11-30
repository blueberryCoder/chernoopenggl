//
// Created by blueberry on 2024/7/5.
//

#pragma once

#include "Test.h"
#include "../Camera.h"
#include "../Model.h"
#include "../Shader.h"

#include "glm/glm.hpp"

#include <memory>
#include <vector>

namespace test {

    class TestAsteroid : public Test {
    public:
        TestAsteroid();

        ~TestAsteroid() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
        void updateMatrices();

    private:
        std::shared_ptr<Shader> m_Shader ,m_InstanceShader;
        std::shared_ptr<Model> m_PlanetModel;
        std::shared_ptr<Model> m_AsteroidModel;
        Camera m_Camera;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        std::vector<glm::mat4> m_AsteroidTransforms;
        unsigned int m_AsteroidCount = 10000;

        std::shared_ptr<VertexBuffer> m_RocksVertexBuffer;
    };
}
