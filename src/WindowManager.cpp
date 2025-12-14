//
// Created by blueberry on 2025/10/7.
//

#include "WindowManager.h"

WindowManager::WindowManager() {

}
WindowManager::~WindowManager() {

}


WindowManager &WindowManager::shared() {
    static WindowManager __singleton ;
    return __singleton;
}

GLFWwindow *WindowManager::getWindow() {
    return m_Window;
}

void WindowManager::SetWindow(GLFWwindow *window) {
    this->m_Window = window;
}

void WindowManager::GetWindowSize(int &width, int &height) {
    if (!m_Window) {
        width = 0;
        height = 0;
        return;
    }
    glfwGetWindowSize(m_Window, &width, &height);
}

void WindowManager::GetFramebufferSize(int &width, int &height) {
    if (!m_Window) {
        width = 0;
        height = 0;
        return;
    }
    glfwGetFramebufferSize(m_Window, &width, &height);
}

void WindowManager::GetViewportSize(int &width, int &height) {
    GetFramebufferSize(width, height);
}
