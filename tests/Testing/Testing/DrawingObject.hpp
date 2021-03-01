#pragma once

#ifndef DRAWINGOBJECT_H
    #define DRAWINGOBJECT_H

#include "utils/RapidjsonImporter.hpp"
#include "Texture.hpp"
#include "utils/TextureAsset.hpp"

#endif

class DrawingObject {
private:

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;



protected:
    Shader* shader;
    Texture* texture;
    float* vertices;
    unsigned int* indices;
    float z;
    TextureAsset* textureAsset;

public:
    DrawingObject(int dimension, TextureAsset* textureAsset, float z, const char* vertexShaderPath, const char* fragmentShaderPath);
    void initBuffers();
    void updateBuffers(int dimension);
    void render(int dimension, glm::mat4 projection, glm::mat4 view, glm::mat4 model);
};
