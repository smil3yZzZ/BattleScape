#pragma once
#define GLEW_STATIC
#ifndef ENGINE_H
    #define ENGINE_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "utils/Shader.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Windows.h>


class Engine {
private:

    static const int squareWidth;
    static const int platformHeight;
    static const int wallHeight;

    int up, right, down, left;

    double framesPerSecond;
    int** map;
    int dimension;

    int isRunning;
    float frameDelay;
    int counter;

    int bufferVertexSize;
    int verticesPerQuad;
    int indicesPerQuad;

    int quadWidth;
    int quadHeight;
    int quadOffset;

    float* vertices;
    unsigned int* indices;

    glm::mat4 projection, model, view;

    Shader textureShader;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

public:
    int run();
    int init(const rapidjson::Document& colors);
    int initMaze(const rapidjson::Document& colors);
    int initCamera();
    int update(const rapidjson::Document& colors);
    int updateBuffers();
    int render();
    int getIsRunning();
    void setIsRunning(int running);
    glm::mat4 getProjection();
    void setProjection(glm::mat4 projection);
    Shader getTextureShader();
    void checkMovementStates();
    void updateMovementStates(int key, int action);
    Engine(double framesPerSecond, int** map, int dimension, int bufferVertexSize, int verticesPerQuad, int indicesPerQuad, int quadWidth, int quadHeight, int quadOffset);
};
#endif