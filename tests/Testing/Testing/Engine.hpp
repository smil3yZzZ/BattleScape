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
#include "utils/Input.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <Windows.h>

#include "utils/GLError.h"

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
    int bufferVertexTexturesSize;
    int verticesPerQuad;
    int indicesPerQuad;

    int quadWidth;
    int quadHeight;
    int quadOffset;

    float* vertices;
    unsigned int* indices;

    unsigned int texture;

    float* textureVertices;
    unsigned int* textureIndices;

    glm::mat4 projection, model, view;
    glm::vec3 cameraPos, cameraUp, cameraTarget;

    Shader colorShader;
    Shader wallShader;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int textureVAO;
    unsigned int textureVBO;
    unsigned int textureEBO;

    Input input;

    GLFWwindow* window;

public:
    int run();
    int init(const rapidjson::Document& colors);
    int initGL();
    int initMaze(const rapidjson::Document& colors);
    int initCamera();
    int initShaders();
    int clearScreen();
    int generateBuffers();
    int update(const rapidjson::Document& colors);
    int updateBuffers();
    int render();
    int getIsRunning();
    void setIsRunning(int running);
    void setView(glm::mat4 view);
    Shader getColorShader();
    int initTextures();
    void checkCamera();
    void updateInput(int key, int action);
    Engine(double framesPerSecond, int** map, int dimension, int bufferVertexSize, int bufferVertexTexturesSize,
        int verticesPerQuad, int indicesPerQuad, int quadWidth, int quadHeight, int quadOffset);
};
#endif
