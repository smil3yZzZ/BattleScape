#pragma once
#ifndef CAMERA_H
    #define CAMERA_H
#include "../utils/GLMImporter.hpp"
#endif

class Camera {
private:
    glm::mat4 projection, model, view;
    glm::vec3 cameraPos, cameraUp, cameraTarget;

public:
	Camera(float xOrigin, float viewportWidth, float yOrigin, float viewportHeight, float zNear, float zFar);
};
