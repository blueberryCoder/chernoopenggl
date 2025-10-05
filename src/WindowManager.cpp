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
