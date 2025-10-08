//
// Created by blueberry on 2025/10/5.
//

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/detail/type_vec3.hpp"


class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    ~Camera();

    void ProcessInputEvent(GLFWwindow *window, float deltaTime);
    void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    void ProcessMouseScroll(double yoffset);

    glm::mat4 GetViewMatrix();

    double GetZoom() {
        return zoom;
    }

private:
    glm::vec3 m_CamPos;
    glm::vec3 m_CamFront;
    glm::vec3 m_CamUp;
    glm::vec3 m_CamRight;
    glm::vec3 m_WorldUp;

    double m_Yaw = -90.0f;
    double m_Pitch = 0;

    float m_Sensitivity = 0.1f;

    double lastX = 960 / 2.0;
    double lastY = 540 / 2.0;

    bool firstMouse = true;
    double zoom = 45.0f;

    void updateCameraCoords();

};
