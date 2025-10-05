//
// Created by blueberry on 2025/10/7.
//

#pragma once
#include <GLFW/glfw3.h>


class WindowManager {
public:
    static WindowManager &shared();
    GLFWwindow *getWindow();
    void SetWindow(GLFWwindow *window);
private:
    WindowManager();
    ~WindowManager();

    GLFWwindow* m_Window;
};
