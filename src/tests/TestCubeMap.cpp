//
// Created by blueberry on 2025/10/26.
//

#include "TestCubeMap.h"

#include "../FileUtil.h"
#include "../src/VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"


namespace test {
    TestCubeMap::TestCubeMap() {
        SetupCursorCallback();
        GLCall(glEnable(GL_DEPTH_TEST));

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_depth.shader"));
        m_CubeTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/marble.jpg"));
        m_ReflectCubeShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_reflect.shader"));

        m_CubeVBO = std::make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
        m_CubeIBO = std::make_shared<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(cubeIndices[0]));

        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_CubeVAO = std::make_shared<VertexArray>();
        m_CubeVAO->AddBuffer(*m_CubeVBO, layout);

        m_ReflectCubeVBO = std::make_shared<VertexBuffer>(cubeReflectVertices, sizeof(cubeReflectVertices));
        m_ReflectCubeIBO = std::make_shared<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(cubeIndices[0]));

        VertexBufferLayout layout2{};
        layout2.Push<float>(3);
        layout2.Push<float>(3);
        m_ReflectCubeVAO = std::make_shared<VertexArray>();
        m_ReflectCubeVAO->AddBuffer(*m_ReflectCubeVBO, layout2);

        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        m_Shader->Bind();

        m_Shader->SetUniform1i("texture1", 0);
        m_CubeTexture->Bind();

        m_ReflectCubeShader->Bind();
        m_ReflectCubeShader->SetUniform1i("skybox", 1);

        // skybox
        m_SkyboxVBO = std::make_shared<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));
        m_SkyboxIBO = std::make_shared<IndexBuffer>(skyboxIndices, sizeof(skyboxIndices) / sizeof(skyboxIndices[0]));
        VertexBufferLayout skyboxLayout{};
        skyboxLayout.Push<float>(3);
        m_SkyboxVAO = std::make_shared<VertexArray>();
        m_SkyboxVAO->AddBuffer(*m_SkyboxVBO, skyboxLayout);
        //
        m_SkyboxShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_skybox.shader"));
        std::vector<std::string> skyboxTextures = {
            FileUtil::shared().GetPath("./textures/skybox/right.jpg"),
            FileUtil::shared().GetPath("./textures/skybox/left.jpg"),
            FileUtil::shared().GetPath("./textures/skybox/top.jpg"),
            FileUtil::shared().GetPath("./textures/skybox/bottom.jpg"),
            FileUtil::shared().GetPath("./textures/skybox/front.jpg"),
            FileUtil::shared().GetPath("./textures/skybox/back.jpg"),
        };
        TextureInitParams initParams = {
            .flip = 0,
            .type = GL_TEXTURE_CUBE_MAP,
            .textureFaces = skyboxTextures
        };
        m_SkyboxTexture = std::make_shared<Texture>("", initParams);
        //
        m_SkyboxTexture->Bind(1);
        m_SkyboxShader->Bind();
        m_SkyboxShader->SetUniform1i("texture1", 1);
    }

    TestCubeMap::~TestCubeMap() {
        GLCall(glDisable(GL_DEPTH_TEST));
    };

    void TestCubeMap::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        Test::ProcessInputEvent(window, deltaTime);
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestCubeMap::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }

    void TestCubeMap::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestCubeMap::OnUpdate(float deltaTime) {
        glm::mat4 view = m_Camera->GetViewMatrix();
        auto fov = glm::radians(m_Camera->GetZoom());
        glm::mat4 proj = glm::perspective(fov, 960.0 / 540.0, 0.1, 50.0);
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", proj);

        m_ReflectCubeShader->Bind();
        m_ReflectCubeShader->SetUniformMat4f("view", view);
        m_ReflectCubeShader->SetUniformMat4f("projection", proj);

        m_SkyboxShader->Bind();
        m_SkyboxShader->SetUniformMat4f("view", glm::mat4(glm::mat3(view)));
        // m_SkyboxShader->SetUniformMat4f("view", view);
        m_SkyboxShader->SetUniformMat4f("projection", proj);
    }

    void TestCubeMap::OnRender() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GLCall(glDepthFunc(GL_LESS));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.8f, 0.0f, -1.0f));
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("model", model);
        renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);


        glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 0.0f, -1.2f));
        m_ReflectCubeShader->Bind();
        m_ReflectCubeShader->SetUniformMat4f("model", model2);
        renderer.Draw(*m_ReflectCubeVAO, *m_ReflectCubeIBO, *m_ReflectCubeShader);

        GLCall(glDepthFunc(GL_EQUAL));
        m_SkyboxShader->Bind();
        m_SkyboxShader->SetUniformMat4f("model", glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0.0)));
        renderer.Draw(*m_SkyboxVAO, *m_SkyboxIBO, *m_SkyboxShader);
    }
}
