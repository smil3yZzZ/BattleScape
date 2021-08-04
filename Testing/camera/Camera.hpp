#pragma once
#ifndef CAMERA_H
    #define CAMERA_H
#include "../utils/GLMImporter.hpp"
#include "../utils/GLImporter.hpp"
#include "../constants.hpp"
#endif

class Camera {
private:
    glm::mat4 projection, model, view;
    glm::vec3 cameraPos, cameraUp, cameraTarget;

public:
	Camera(float xOrigin, float viewportWidth, float yOrigin, float viewportHeight, float zNear, float zFar);
    void setView(glm::vec3 cameraPosDiff, glm::vec3 cameraTargetDiff);
    GLfloat* getView();
    GLfloat* getProjection();
    GLfloat* getModel();
};
