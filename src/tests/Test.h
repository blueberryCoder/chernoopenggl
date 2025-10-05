//
// Created by blueberry on 2022/12/3.
//

#pragma once

#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace test {
    class Test {
    public :
        Test() {
        }

        virtual ~Test();

        virtual void OnUpdate(float deltaTime) {
        }

        virtual void OnRender() {
        }

        virtual void OnImGuiRender() {
        }

        virtual void ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        }

        virtual void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {}

        virtual void ProcessMouseScroll(GLFWwindow *window, double yoffset) {}

    protected:
        void SetupCursorCallback();

    };

    class TestMenu : public Test {
    public :
        TestMenu(Test *&currentTestPointer);

        void OnImGuiRender() override;

        template<typename T>
        void RegisterTest(const std::string &name) {
            std::cout << "Registering test" << name << std::endl;
            m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
        };

    private:
        Test *&m_CurrentTest;
        std::vector<std::pair<std::string, std::function<Test *()> > > m_Tests;
    };
}
