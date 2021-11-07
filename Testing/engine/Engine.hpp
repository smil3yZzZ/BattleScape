#pragma once
#define GLEW_STATIC
#ifndef ENGINE_H
    #define ENGINE_H

#include <thread>
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
#include "../drawings/FogDrawingObject.hpp"


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
    int mainPlayerLastDirection;

    double framesPerSecond;
    float frameDelay;

    Camera* camera;

    PlatformsDrawingObject* platforms;
    WallsDrawingObject* walls;
    FogDrawingObject* fog;

    Character* testCharacter;

    Input* input;

    GLFWwindow* window;
    unsigned char* createColorPlatformsAndShadows(const rapidjson::Document& colorsInfo);
    unsigned char* createCharacterShadow();


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
    int initMapTextures(const rapidjson::Document& colorsInfo, const rapidjson::Document& wallsInfo);
    int initFog();
    int initCharacters();
    void checkCamera(int frame);
    void updateInput(int key, int action);

};
