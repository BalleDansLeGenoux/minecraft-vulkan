#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

class Camera {
public:
    Camera(glm::vec3 position, float fov, float aspectRatio);

    void update(float deltaTime);
    int  processKeyboard(int key, int action);
    void processMouse(float offsetX, float offsetY);
    void updateViewMatrix();
    void updateProjection(float aspectRatio);

    glm::mat4 getViewMatrix() const { return viewMatrix; }
    glm::mat4 getProjectionMatrix() const { return projectionMatrix; }
    glm::vec3 getPosition() const { return position; }

    void debug();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float fov;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    float axeX;
    float axeY;
    float axeZ;
};

#endif