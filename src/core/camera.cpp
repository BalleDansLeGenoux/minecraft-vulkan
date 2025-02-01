#include "core/camera.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

Camera::Camera(glm::vec3 position, float fov, float aspectRatio)
    : position(position), yaw(-90.0f), pitch(0.0f), movementSpeed(5.0f), 
      mouseSensitivity(0.1f), fov(fov) {

    worldUp = glm::vec3(0.0f, -1.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, 0.0f);

    axeX = 0;
    axeY = 0;
    axeZ = 0;

    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
    updateViewMatrix();
}

void Camera::update(float deltaTime) {
    float velocity = movementSpeed * deltaTime;

    glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));

    position += forward * axeZ * velocity;
    position += worldUp * axeY * velocity;
    position += right * axeX * velocity;

    updateViewMatrix();
}

int Camera::processKeyboard(int key, int action) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) axeZ++;
    if (key == GLFW_KEY_S && action == GLFW_PRESS) axeZ--;
    if (key == GLFW_KEY_A && action == GLFW_PRESS) axeX--;
    if (key == GLFW_KEY_D && action == GLFW_PRESS) axeX++;
    if (key == GLFW_KEY_C && action == GLFW_PRESS) axeY++;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) axeY--;

    if (key == GLFW_KEY_W && action == GLFW_RELEASE) axeZ--;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) axeZ++;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) axeX++;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) axeX--;
    if (key == GLFW_KEY_C && action == GLFW_RELEASE) axeY--;
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) axeY++;

    return key == GLFW_KEY_ESCAPE;
}

void Camera::processMouse(float offsetX, float offsetY) {
    offsetX *= mouseSensitivity;
    offsetY *= mouseSensitivity;

    yaw = fmod(yaw + offsetX, 360.0f);
    pitch += offsetY;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

void Camera::updateViewMatrix() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);

    viewMatrix = glm::lookAt(position, position + front, worldUp);
}

void Camera::updateProjection(float aspectRatio) {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
}


void Camera::debug() {
    std::cout << "Position : " << position.x << " | " << position.y << " | " << position.z << std::endl;
    std::cout << "Yaw : " << yaw << std::endl;
    std::cout << "Pitch : " << pitch << std::endl << std::endl;
}