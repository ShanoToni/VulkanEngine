#include "Camera.hpp"
#include <iostream>

Camera::Camera(GLFWwindow* window, glm::vec3 camPos, glm::vec3 camTarget,
               glm::vec3 up)
    : cameraPos(camPos), cameraTarget(camTarget), up(up) {
    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    view = glm::lookAt(cameraFront, cameraPos + cameraFront, cameraUp);

    direction = cameraDirection;
    cameraRight = glm::normalize(glm::cross(cameraFront, up));
    cameraUp = glm::normalize(glm::cross(cameraRight, direction));

    firstMouse = true;

    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
}

void Camera::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 0.2f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * direction;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * direction;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= cameraRight * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += cameraRight * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        cameraPos -= cameraUp * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPos += cameraUp * cameraSpeed;
    }

    update();
}

void Camera::update() {
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraRight = glm::normalize(glm::cross(cameraFront, up));
    cameraUp = glm::normalize(glm::cross(cameraRight, direction));

    cameraFront = glm::normalize(direction);
}
