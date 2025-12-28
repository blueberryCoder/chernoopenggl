//
// Created by blueberry on 2025/10/5.
//

#include "Camera.h"

#include <iostream>

#include "glm/detail/func_geometric.hpp"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : m_CamPos(position), m_WorldUp(up),
                                                                    m_CamFront(front) {
    updateCameraCoords();
}

Camera::~Camera() {
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(m_CamPos, m_CamPos + m_CamFront, m_CamUp);
}

glm::vec3 Camera::GetPosition() {
    return m_CamPos;
}

void Camera::SetPosition(const glm::vec3 &position) {
    m_CamPos = position;
    updateCameraCoords();
}

void Camera::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
    const float speed = 2.0 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_CamPos += m_CamFront * speed;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_CamPos -= m_CamFront * speed;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_CamPos -= m_CamRight * speed;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_CamPos += m_CamRight * speed;
    }
    updateCameraCoords();
}


void Camera::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= m_Sensitivity;
    yoffset *= m_Sensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    if (m_Pitch > 89.0f) {
        m_Pitch = 89.0f;
    }
    if (m_Pitch < -89.0f) {
        m_Pitch = -89.0f;
    }

    updateCameraCoords();
}

void Camera::ProcessMouseScroll(double yoffset) {
    zoom -= yoffset;
    if (zoom < 1.0f) {
        zoom = 1.0f;
    }
    if (zoom > 45.0f) {
        zoom = 45.0f;
    }
}

void Camera::updateCameraCoords() {
    glm::vec3 direction;
    // std::cout << "Yaw: " << m_Yaw << ",Pitch:" << m_Pitch << std::endl;
    direction.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
    direction.y = glm::sin(glm::radians(m_Pitch));
    direction.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
    m_CamFront = glm::normalize(direction);

    m_CamRight = glm::normalize(glm::cross(m_CamFront, m_WorldUp));
    m_CamUp = glm::normalize(glm::cross(m_CamRight, m_CamFront));
}
