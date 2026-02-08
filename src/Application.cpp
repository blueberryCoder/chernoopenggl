#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "FileUtil.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "TestAssimp.h"
#include "TestBlending.h"
#include "TestCubeMap.h"
#include "TestDepth.h"
#include "TestFaceCulling.h"
#include "TestNormalMapping.h"
#include "TestNormalModel.h"
#include "TestParallaxMapping.h"
#include "TestStencil.h"
#include "Texture.h"

#include "VertexBufferLayout.h"
#include "WindowManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTriangle.h"
#include "tests/TestBatchVertex.h"
#include "tests/TestBatchTexture.h"
#include "tests/TestBatchDynamic.h"
#include "tests/TestMeanBlur.h"
#include "tests/TestAdvanced.h"
#include "tests/TestAdvancedLighting.h"
#include "tests/TestHDR.h"
#include "tests/TestBloom.h"
#include "tests/TestDeferredShading.h"
#include "tests/TestDeferredShadingVolums.h"
#include "tests/TestShadowMapping.h"
#include "tests/TestShadowMappingPerspective.h"
#include "tests/TestLisaShadow.h"
#include "tests/TestPointShadow.h"
#include "tests/TestGeometry.h"
#include "tests/TestExploding.h"
#include "tests/TestInstance.h"
#include "tests/TestAsteroid.h"
#include "tests/TestAntiAliasing.h"
#include "tests/TestAntiAliasingEffect.h"

int main(void) {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_SAMPLES, 4);


    FileUtil::shared().setResourceRootPath("../../res");

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    WindowManager::shared().SetWindow(window);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "glew init error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    // glEnable(GL_MULTISAMPLE);
    // glEnable(GL_FRAMEBUFFER_SRGB);


    {
        GLCall(glEnable(GL_BLEND))
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();


        test::Test *currentTest = nullptr;
        test::TestMenu *testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;
        testMenu->RegisterTest<test::TestDeferredShading>("Deferred shading");
        testMenu->RegisterTest<test::TestDeferredShadingVolums>("Deferred shading volumes");
        testMenu->RegisterTest<test::TestBloom>("Bloom");
        testMenu->RegisterTest<test::TestHDR>("HDR");
        testMenu->RegisterTest<test::TestParallaxMapping>("parallax mapping");
        testMenu->RegisterTest<test::TestNormalMapping>("normal mapping");
        testMenu->RegisterTest<test::TestNormalModel>("normal model");
        testMenu->RegisterTest<test::TestPointShadow>("point shadow");
        testMenu->RegisterTest<test::TestShadowMapping>("shadow mapping");
        testMenu->RegisterTest<test::TestShadowMappingPerspective>("shadow mapping perspective");
        testMenu->RegisterTest<test::TestLisaShadow>("lisa shadow");
        testMenu->RegisterTest<test::TestAdvancedLighting>("advanced lighting");
        testMenu->RegisterTest<test::TestAntiAliasingEffect>("AntiAliasingEffect");
        testMenu->RegisterTest<test::TestAntiAliasing>("AntiAliasing");
        testMenu->RegisterTest<test::TestAsteroid>("Asteroid");
        testMenu->RegisterTest<test::TestInstance>("Instance");
        testMenu->RegisterTest<test::TestGeometry>("geometry");
        testMenu->RegisterTest<test::TestAdvanced>("advanced");
        testMenu->RegisterTest<test::TestExploding>("exploding");
        testMenu->RegisterTest<test::TestCubeMap>("skybox");
        testMenu->RegisterTest<test::TestFaceCulling>("faceculling");
        testMenu->RegisterTest<test::TestBlending>("blending");
        testMenu->RegisterTest<test::TestStencil>("stencil");
        testMenu->RegisterTest<test::TestDepth>("Depth");
        testMenu->RegisterTest<test::TestAssimp>("Assimp");
        testMenu->RegisterTest<test::TestClearColor>("Clear color");
        testMenu->RegisterTest<test::TestTriangle>("Triangle");
        testMenu->RegisterTest<test::TestTexture2D>("2D m_Texture");
        testMenu->RegisterTest<test::TestBatchVertex>("Batch Vertex");
        testMenu->RegisterTest<test::TestBatchTexture>("Batch Texture");
        testMenu->RegisterTest<test::TestBatchDynamic>("Batch Dynamic");
        testMenu->RegisterTest<test::TestMeanBlur>("Mean Blur");
        testMenu->RegisterTest<test::TestAntiAliasing>("Anti Aliasing");

        Renderer renderer;

        double lastFrame = glfwGetTime();

        while (!glfwWindowShouldClose(window)) {
            renderer.Clear();
            ImGui_ImplGlfwGL3_NewFrame();

            if (currentTest) {
                double currentFrame = glfwGetTime();
                double deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                currentTest->OnUpdate(deltaTime);
                currentTest->OnRender();

                currentTest->ProcessInputEvent(window, deltaTime);
                ImGui::Begin("Test");
                if (currentTest != testMenu && (ImGui::Button("<-") || glfwGetKey(window, GLFW_KEY_ESCAPE) ==
                                                GLFW_PRESS)) {
                    delete currentTest;
                    currentTest = testMenu;
                }

                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete currentTest;
        if (currentTest != testMenu) {
            delete testMenu;
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
