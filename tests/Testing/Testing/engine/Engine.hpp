#pragma once
#define GLEW_STATIC
#ifndef ENGINE_H
    #define ENGINE_H


#include <iostream>
#include <list>
#include "../constants.hpp"
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
#include "../character/Character.hpp"


//#include <Windows.h>

#endif

class Engine {
private:

    int** map;
    int** wallMap;
    int dimension;

    float xOrigin;
    float yOrigin;

    int isRunning;

    double framesPerSecond;
    float frameDelay;

    Camera* camera;

    PlatformsDrawingObject* platforms;
    WallsDrawingObject* walls;

    Character* testCharacter;

    Input* input;

    GLFWwindow* window;
    unsigned char* createColorPlatforms(const rapidjson::Document& colorsInfo);


public:
    Engine(double framesPerSecond, int** map, int** wallMap, int dimension,
    float xOrigin, float yOrigin);
    int run();
    int init(const rapidjson::Document& colorsInfo, const rapidjson::Document& wallsInfo);
    int initGL();
    int initCamera();
    int clearScreen();
    int generateBuffers();
    int update();
    int updateBuffers();
    int render();
    int getIsRunning();
    void setIsRunning(int running);
    int initTextures(const rapidjson::Document& colorsInfo, const rapidjson::Document& wallsInfo);
    int initCharacters();
    void checkCamera();
    void updateInput(int key, int action);

};
