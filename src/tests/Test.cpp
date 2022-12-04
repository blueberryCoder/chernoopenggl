//
// Created by blueberry on 2022/12/4.
//
#include "Test.h"
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

}

