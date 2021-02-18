#include "Camera.hpp"

Camera::Camera(float xOrigin, float viewportWidth, float yOrigin, float viewportHeight, float zNear, float zFar) {
    projection = glm::ortho(xOrigin, viewportWidth, yOrigin, viewportHeight, zNear, zFar);
    model = glm::mat4(1.0f);

    cameraPos = glm::vec3(0.0f, 0.0f, 100.0f);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);


    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
}
