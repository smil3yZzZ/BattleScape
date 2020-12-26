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

class Engine {
private:

    static const int squareWidth;
    static const int platformHeight;
    static const int wallHeight;

    int up, right, down, left;

    double framesPerSecond;
    int** map;
    int** wallMap;
    int dimension;

    int isRunning;
    float frameDelay;
    int counter;

    int bufferVertexSize;
    int bufferVertexTexturesSize;
    int textureVerticesPerQuad;
    int verticesPerQuad;
    int indicesPerQuad;
    int textureIndicesPerQuad;

    int quadWidth;
    int quadHeight;

    float* vertices;
    unsigned int* indices;

    unsigned int texture;
    //unsigned int *wallTextures;
    float wallTextureWidth;

    float* textureVertices;
    unsigned int* textureIndices;
    int numOfWallTextureFiles;

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
    Shader getColorShader();
    int initTextures();
    void checkCamera();
    void updateInput(int key, int action);
    Engine(double framesPerSecond, int** map, int** wallMap, int dimension, int bufferVertexSize,
        int bufferVertexTexturesSize,
        int verticesPerQuad, int textureVerticesPerQuad, int indicesPerQuad, int textureIndicesPerQuad,
         int quadWidth, int quadHeight);
};
#endif
