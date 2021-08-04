#include "Camera.hpp"

Camera::Camera(float xOrigin, float viewportWidth, float yOrigin, float viewportHeight, float zNear, float zFar) {
    projection = glm::ortho(xOrigin, viewportWidth, yOrigin, viewportHeight, zNear, zFar);
    model = glm::mat4(UNITARY_COMPONENT);

    cameraPos = glm::vec3(0.0f, 0.0f, CAMERA_Z_POS);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, UNITARY_COMPONENT, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);


    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
}

void Camera::setView(glm::vec3 cameraPosDiff, glm::vec3 cameraTargetDiff) {
    cameraPos += cameraPosDiff;
    cameraTarget += cameraTargetDiff;
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
}

GLfloat* Camera::getView() {
    return glm::value_ptr(view);
}

GLfloat* Camera::getProjection() {
    return glm::value_ptr(projection);
}

GLfloat* Camera::getModel() {
    return glm::value_ptr(model);
}
