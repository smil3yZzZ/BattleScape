#pragma once
#define GLEW_STATIC
#ifndef ENGINE_H
    #define ENGINE_H


#include <iostream>
#include <list>
#include "../utils/RapidjsonImporter.hpp"
#include "../shader/Shader.hpp"
#include "../input/Input.hpp"
#include "../drawings/DrawingObject.hpp"
#include "../texture/TextureAsset.hpp"
#include "../utils/GLMImporter.hpp"
#include "../utils/GLImporter.hpp"
#include "../texture/TextureUtils.hpp"
#include "../utils/GLError.h"
#include "../camera/Camera.hpp"

//#include <Windows.h>

#endif

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float VIEWPORT_WIDTH = 400.0f;
const float VIEWPORT_HEIGHT = 300.0f;
const float Z_NEAR = -100.0f;
const float Z_FAR = 100.0f;

const int NUMBER_OF_RGBA_CHANNELS = 4;
const int PLATFORM_BUFFER_VERTEX_SIZE = 5;
const int WALL_BUFFER_VERTEX_SIZE = 5;
const int PLATFORM_VERTICES_PER_QUAD = 4;
const int WALL_VERTICES_PER_QUAD = 8;
const int PLATFORM_INDICES_PER_QUAD = 6;
const int WALL_INDICES_PER_QUAD = 12;

const int QUAD_WIDTH = 64;
const int QUAD_HEIGHT = 64;

const int PLATFORM_TEXTURE_ROWS = 8;
const int PLATFORM_TEXTURE_COLS = 1;

const int WALL_TEXTURE_ROWS = 1;
const int WALL_TEXTURE_COLS = 12;

const float PLATFORMS_Z = 5.0f;
const float WALLS_Z = 10.0f;

const char* const SQUARE_VERTEX_SHADER_PATH = "resources/shaders/mapQuad.vs";
const char* const SQUARE_FRAGMENT_SHADER_PATH = "resources/shaders/mapQuad.fs";

const char* const WALL_TEXTURE_PATH = "resources/wall.png";

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

    Camera* camera;

    PlatformsDrawingObject* platforms;
    WallsDrawingObject* walls;

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

    Input* input;

    GLFWwindow* window;
    unsigned char* createColorPlatforms(const rapidjson::Document& colorsInfo);


public:
    Engine(double framesPerSecond, int** map, int** wallMap, int dimension,
    float xOrigin, float yOrigin);
    int run();
    int init(const rapidjson::Document& colorsInfo, const rapidjson::Document& wallsInfo);
    int initGL();
    int initMaze(const rapidjson::Document& colorsInfo, const rapidjson::Document& wallsInfo);
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
    int initTextures(const rapidjson::Document& colorsInfo, const rapidjson::Document& wallsInfo);
    void checkCamera();
    void updateInput(int key, int action);

};
