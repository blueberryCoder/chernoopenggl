//
// Created by blueberry on 2025/10/5.
//

#include "Camera.h"

#include "glm/detail/func_geometric.hpp"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front) : m_CamPos(position), m_CamUp(up), m_CamFront(front) {
}

Camera::~Camera() {
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(m_CamPos, m_CamPos + m_CamFront, m_CamUp);
}

void Camera::ProcessInput(GLFWwindow *window, float deltaTime) {
    const float speed = 2.0 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_CamPos += m_CamFront * speed;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_CamPos -= m_CamFront * speed;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_CamPos -= glm::normalize(glm::cross(m_CamFront, m_CamUp)) * speed;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_CamPos += glm::normalize(glm::cross(m_CamFront, m_CamUp)) * speed;
    }
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

    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(m_Yaw) * glm::cos(glm::radians(m_Pitch)));
    direction.y = glm::sin(glm::radians(m_Pitch));
    direction.z = glm::sin(glm::radians(m_Yaw) * glm::cos(glm::radians(m_Pitch)));
    m_CamFront = glm::normalize(direction);

}

void Camera::ProcessMouseScroll(double yoffset) {
    zoom -= yoffset ;
    if (zoom < 1.0f) {
        zoom = 1.0f;
    }
    if (zoom > 45.0f) {
        zoom = 45.0f;
    }
}
