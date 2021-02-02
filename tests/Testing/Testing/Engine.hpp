#pragma once
#define GLEW_STATIC
#ifndef ENGINE_H
    #define ENGINE_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <list>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "utils/Shader.hpp"
#include "utils/Input.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <Windows.h>

#include "utils/GLError.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float VIEWPORT_WIDTH = 400.0f;
const float VIEWPORT_HEIGHT = 300.0f;

const int NUMBER_OF_PLATFORMS = 8;
const int NUMBER_OF_RGBA_CHANNELS = 4;
const int PLATFORM_BUFFER_VERTEX_SIZE = 5;
const int WALL_BUFFER_VERTEX_SIZE = 8;
const int PLATFORM_INDICES_PER_QUAD = 6;
const int WALL_INDICES_PER_QUAD = 12;
const int PLATFORM_VERTICES_PER_QUAD = 4;
const int WALL_VERTICES_PER_QUAD = 8;
const int QUAD_WIDTH = 64;
const int QUAD_HEIGHT = 64;

class Engine {
private:

    int up, right, down, left;

    double framesPerSecond;
    int** map;
    int** wallMap;
    int dimension;

    float xOrigin;
    float yOrigin;

    int isRunning;
    float frameDelay;
    int counter;

    float* platformVertices;
    unsigned int* platformIndices;

    unsigned int texture;
    unsigned int colorTexture;
    float wallTextureWidth;

    float* wallVertices;
    unsigned int* wallIndices;

    float* wallShadowVertices;
    unsigned int* wallShadowIndices;

    glm::mat4 projection, model, view;
    glm::vec3 cameraPos, cameraUp, cameraTarget;

    Shader wallShader;
    Shader platformShader;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int textureVAO;
    unsigned int textureVBO;
    unsigned int textureEBO;

    Input input;

    GLFWwindow* window;
    unsigned char* createColorPlatforms(const rapidjson::Document& colors);


public:
    Engine(double framesPerSecond, int** map, int** wallMap, int dimension,
    float xOrigin, float yOrigin);
    int run();
    int init(const rapidjson::Document& colors, const rapidjson::Document& walls);
    int initGL();
    int initMaze(const rapidjson::Document& colors, const rapidjson::Document& walls);
    int initCamera();
    int initShaders();
    int clearScreen();
    int generateBuffers();
    int update();
    int updateBuffers();
    int render();
    int getIsRunning();
    void setIsRunning(int running);
    void setView(glm::mat4 view);
    int initTextures(const rapidjson::Document& colors);
    void checkCamera();
    void updateInput(int key, int action);

};
#endif
