//
// Created by blueberry on 2022/12/4.
//
#include "Test.h"

#include "../WindowManager.h"
#include "imgui/imgui.h"

namespace test {
    TestMenu::TestMenu(test::Test *&currentTestPointer) : m_CurrentTest(currentTestPointer) {
    }

    void TestMenu::OnImGuiRender() {
        for (auto &test: m_Tests) {
            if (ImGui::Button(test.first.c_str())) {
                m_CurrentTest = test.second();
            }
        }
    }

    void Test::SetupCursorCallback() {
        auto window = WindowManager::shared().getWindow();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (!window) { return; }
        glfwSetWindowUserPointer(window, this);
        glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
            auto *test = static_cast<test::Test *>(glfwGetWindowUserPointer(window));
            if (test) {
                test->ProcessCursorPosCallback(window, xpos, ypos);
            }
        });
        glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
            auto *test = static_cast<test::Test *>(glfwGetWindowUserPointer(window));
            if (test) {
                test->ProcessMouseScroll(window, yoffset);
            }
        });
    }

    Test::~Test() {
        auto window = WindowManager::shared().getWindow();
        if (!window) { return; }
        glfwSetCursorPosCallback(window, nullptr);
        glfwSetWindowUserPointer(window, nullptr);
        glfwSetScrollCallback(window, nullptr);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    }
}
