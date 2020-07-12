#pragma once
#ifndef ENGINE_H
#define ENGINE_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

//#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>


class Engine {
private:

    static const int squareWidth;
    static const int platformHeight;
    static const int wallHeight;

    double framesPerSecond;
    int** map;
    int dimension;

    int isRunning;
    float frameDelay;
    int counter;

    int bufferVertexSize;
    int verticesPerQuad;

public:
    int run();
    int render();
    int update();
    int getIsRunning();
    void setIsRunning(int running);
    Engine(double framesPerSecond, int** map, int dimension, int bufferVertexSize, int verticesPerQuad);
};
#endif