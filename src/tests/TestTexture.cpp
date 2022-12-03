//
// Created by blueberry on 2022/12/3.
//
#include "TestTexture.h"

#include "imgui/imgui.h"
namespace test {

    TestTexture::TestTexture()
            : m_Position
                      {
                              {-50.0f, -50.0f, 0.0f, 0.0f,},
                              {50.0f,  -50.0f, 1.0f, 0.0f,},
                              {50.0f,  50.0f,  1.0f, 1.0f,},
                              {-50.0f, 50.0f,  0.0f, 1.0f,}
                      },
              m_Indices{0, 1, 2, 2, 3, 0},
              va(),
              vb(m_Position, 4 * 4 * sizeof(float)),
              ib(m_Indices, 6),
              shader("../../res/shaders/Basic.shader"),
              texture("../../res/textures/ChernoLogo.png") {

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        translationA = glm::vec3(200.0f, 200.0f, 0.0f);
        translationB = glm::vec3(400.0f, 200.0f, 0.0f);


        shader.Bind();
        texture.Bind(0);
        shader.SetUniform1i("u_Texture",0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
    }

    TestTexture::~TestTexture() {

    }

    void TestTexture::OnUpdate(float deltaTime) {
    }

    void TestTexture::OnRender() {
        renderer.Clear();

        {
            auto proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
            glm::mat4 model = glm::translate(glm::mat4(1.0f),translationA);
            mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            shader.Bind();
            renderer.Draw(va,ib,shader);
        }

        {
            auto proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
            glm::mat4 model = glm::translate(glm::mat4(1.0f),translationB);
            mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            shader.Bind();
            renderer.Draw(va,ib,shader);
        }
    }

    void TestTexture::OnImGuiRender() {

        ImGui::SliderFloat3("translationA",&translationA.x,0.0f,960);
        ImGui::SliderFloat3("translationB",&translationB.x,0.0f,960);
    }
}

